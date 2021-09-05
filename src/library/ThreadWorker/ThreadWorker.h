#ifndef ESPERATTATO_THREAD_WORKER_DEF
#define ESPERATTATO_THREAD_WORKER_DEF
#include "../Process/Process.h"
#include <utility>
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>
#include <thread>

using namespace std;

namespace Esperatto
{



class ThreadWork {
	public:
		mutex innerMutex;
		unique_lock<mutex> innerLock = unique_lock<mutex>(innerMutex);
		queue<Process> innerQueue;
		std::chrono::duration<long double> loopMinimum = 1s / 60.00;
	
		condition_variable queueHasData;
		bool ending = false;
		ThreadWork();
		Process* pop();
		size_t push(Process p);
};

enum ThreadState
{
	active = 0,
	cooldown = 1,
	blocked = 2,
	standby = 3,
	deleted = 4
};

class ThreadWorker
{
protected:
	struct foreign_data
	{
		ThreadState state = ThreadState::standby;
		thread *inner_thread;
		unsigned int reference_count = 0;
		shared_ptr<ThreadWork> processes;
		condition_variable should_start;
	};
	shared_ptr<foreign_data> data;

public:
	ThreadWorker(shared_ptr<ThreadWork> processes, std::chrono::duration<long double> stepDuration = 1s / 60.00);

	ThreadWorker(ThreadWorker const &origin);

	~ThreadWorker();

	void take_off_standby();

	void process_cycle();

	void kill_worker();
};
} // namespace Esperatto

#endif