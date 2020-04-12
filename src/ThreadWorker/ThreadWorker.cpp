#include "ThreadWorker.h"

#include "../Process/Process.h"
#include <allegro5/allegro.h>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

using namespace std;

namespace Esperatto
{

typedef pair<mutex, queue<Process>> ThreadWork;

ThreadWorker::ThreadWorker(shared_ptr<ThreadWork> processes)
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
		// m.lock();
		if (data->inner_thread != nullptr)
		{
			take_off_standby();
			data->state = deleted;
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
		// delete data;
		// m.unlock();
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
	if (data->inner_thread != nullptr)
	{
		take_off_standby();
		data->inner_thread->join();
		delete data->inner_thread;
		data->inner_thread = nullptr;
	}
}

void ThreadWorker::process_cycle()
{
	auto data = this->data;
	while (data->state != deleted)
	{
		data->processes->first.lock();
		Process process = data->processes->second.front();
		data->processes->second.pop();
		data->processes->first.unlock();

		process.Execute(*this);

		// cout << "first: " << typeid(data->processes->first).name() << endl;
		data->processes->first.lock();
		data->processes->second.push(process);
		data->processes->first.unlock();

		if (data->state == deleted)
		{
			break;
		}

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
	}
}
} // namespace Esperatto