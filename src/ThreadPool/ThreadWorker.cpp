#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <utility>
#include <allegro5/allegro.h>
#include "./Process.cpp"

using namespace std;


namespace Esperattato {

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
			struct foreign_data {
				ThreadState state = ThreadState::standby;
				thread *inner_thread = nullptr;
				unsigned int reference_count = 0;
				ThreadWork *processes;
				condition_variable should_start;
			} *data = nullptr;

		public:
			ThreadWorker(ThreadWork *processes){
				data = new foreign_data();
				data->reference_count = 1;
				data->processes = processes;
			}

			ThreadWorker(ThreadWorker &origin){
				this->data = origin.data;
				data->reference_count++;
			}

			~ThreadWorker(){
				data->reference_count--;
				if(data->reference_count == 0){
					data->state = deleted;
					if(data->inner_thread != nullptr){
						take_off_standby();
						data->inner_thread->join();
						delete data->inner_thread;
					}
					delete data;
				}
			}

			void take_off_standby(){
				if(data->inner_thread == nullptr){
					data->state = active;
					data->inner_thread = new thread(process_cycle);
				}
				if(data->state == standby){
					data->should_start.notify_one();
				}
			}

			void process_cycle(){
				while(data->state != deleted){
					data->processes->first.lock();
					Process process = data->processes->second.front();
					data->processes->second.pop();
					data->processes->first.unlock();

					process.Execute(*this);

					data->processes->first.lock();
					data->processes->second.push(process);
					data->processes->first.unlock();
					
					if(data->state == deleted){
						break;
					}

					if(data->state == cooldown){
						data->state = standby;
					}

					if(data->state == standby){
						unique_lock<mutex> standby_lock;
						standby_lock.lock();
						data->should_start.wait(standby_lock);
						data->state = active;
					}
				}
			}
	};
}