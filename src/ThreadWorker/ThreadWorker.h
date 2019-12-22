#ifndef ESPERATTATO_THREAD_WORKER_DEF
#define ESPERATTATO_THREAD_WORKER_DEF
#include <utility>
#include <mutex>
#include <queue>

using namespace std;

namespace Esperatto {

	class Process;

	typedef pair<mutex, queue<Process>> ThreadWork;

	enum ThreadState {
		active = 0,
		cooldown = 1,
		blocked = 2,
		standby = 3,
		deleted = 4
	};

	class ThreadWorker {
		protected:
			struct foreign_data;
			foreign_data *data;

		public:
			ThreadWorker(ThreadWork *processes);

			ThreadWorker(ThreadWorker const &origin);

			~ThreadWorker();

			void take_off_standby();

			void process_cycle();

			void kill_worker();
	};
}

#endif