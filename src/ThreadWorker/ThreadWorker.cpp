#include "ThreadWorker.h"

#include "../Process/Process.h"
#include <allegro5/allegro.h>
#include <condition_variable>
#include <iostream>
#include <shared_mutex>
#include <queue>
#include <thread>
#include <utility>

using namespace std;

namespace Esperatto
{
	ThreadWorker::ThreadWorker(shared_ptr<ThreadWork> processes, std::chrono::duration<long double> stepDuration )
	{
		data = shared_ptr<foreign_data>(new foreign_data());
		data->reference_count = 1;
		data->processes = processes;
	}

	ThreadWorker::ThreadWorker(ThreadWorker const &origin)
	{
		this->data = origin.data;
		data->reference_count++;
	}

	ThreadWorker::~ThreadWorker()
	{
		data->reference_count--;
		if (data->reference_count == 0)
		{
			if (data->inner_thread != nullptr)
			{
				take_off_standby();
				data->state = deleted;
				data->processes->ending = true;
				data->processes->queueHasData.notify_all();
				if (std::this_thread::get_id() !=
					data->inner_thread->get_id())
				{
					if (data->inner_thread->joinable())
					{
						data->inner_thread->join();
					}
					delete data->inner_thread;
					data->inner_thread = nullptr;
				}
			}
		}
	}

	void ThreadWorker::take_off_standby()
	{
		if (data->inner_thread == nullptr)
		{
			data->state = active;
			data->inner_thread = new thread(&ThreadWorker::process_cycle, this);
		}
		if (data->state == standby)
		{
			data->should_start.notify_one();
		}
	}

	void ThreadWorker::kill_worker()
	{
		data->state = deleted;
		data->processes->ending = true;
		data->processes->queueHasData.notify_all();
		if (data->inner_thread != nullptr)
		{
			take_off_standby();
			data->inner_thread->join();
			delete data->inner_thread;
			data->inner_thread = nullptr;
		}
	}

	ThreadWork::ThreadWork()
	{
		this->innerLock.unlock();
	}

	Process *ThreadWork::pop()
	{
		this->innerMutex.lock();

		if (this->innerQueue.size() == 0)
		{
			this->innerMutex.unlock();
			return nullptr;
		}

		Process *process = new Process(this->innerQueue.front());
		this->innerQueue.pop();
		this->innerMutex.unlock();

		return process;
	}

	size_t ThreadWork::push(Process p)
	{
		this->innerMutex.lock();
		this->innerQueue.push(p);
		this->innerMutex.unlock();
		this->queueHasData.notify_one();
		return this->innerQueue.size();
	}

	void ThreadWorker::process_cycle()
	{
		mutex stepMutex;
		unique_lock<mutex> stepLock(stepMutex);
		condition_variable stepCondition;
		unsigned int count = 0, lastCount = count;
		bool endStepper = false;

		thread stepper = thread([&stepMutex, &stepLock, &stepCondition, &count, &endStepper]() {
			while (!endStepper)
			{
				this_thread::sleep_for(1s / 60.00);
				count++;
				stepCondition.notify_all();
			}
		});

		unsigned int processCount = 0;
		while (data->state != deleted)
		{
			if (processCount >= data->processes->innerQueue.size())
			{
				stepCondition.wait(stepLock, [&count, &lastCount]() { return count > lastCount; });
				processCount = 0;
				lastCount = count;
			}

			Process *process = data->processes->pop();

			if (process == nullptr)
			{
				data->processes->innerMutex.lock();
				data->processes->queueHasData.wait(data->processes->innerLock, [this]() { return data->state == deleted || data->processes->innerQueue.size() > 0; });
				data->processes->innerMutex.unlock();
				data->processes->queueHasData.notify_one();
				continue;
			}

			process->Execute(*this);

			data->processes->push(*process);
			delete process;

			if (data->state == cooldown)
			{
				data->state = standby;
			}

			if (data->state == standby)
			{
				unique_lock<mutex> standby_lock;
				standby_lock.lock();
				data->should_start.wait(standby_lock);
				data->state = active;
			}

			processCount++;
		}

		endStepper = true;
		stepper.join();
	}
} // namespace Esperatto