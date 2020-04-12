#ifndef ESPERATTATO_THREAD_WORKER_DEF
#define ESPERATTATO_THREAD_WORKER_DEF
#include <utility>
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>
#include <thread>

using namespace std;

namespace Esperatto
{

class Process;

typedef pair<mutex, queue<Process>> ThreadWork;

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
	ThreadWorker(shared_ptr<ThreadWork> processes);

	ThreadWorker(ThreadWorker const &origin);

	~ThreadWorker();

	void take_off_standby();

	void process_cycle();

	void kill_worker();
};
} // namespace Esperatto

#endif