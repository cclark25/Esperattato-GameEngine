#include "ThreadWorker.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <utility>
#include <allegro5/allegro.h>
#include "Process.h"
#include <iostream>

using namespace std;


namespace Esperattato {

	typedef pair<mutex, queue<Process>> ThreadWork;
	struct ThreadWorker::foreign_data {
			ThreadState state = ThreadState::standby;
			thread *inner_thread = nullptr;
			unsigned int reference_count = 0;
			ThreadWork *processes;
			condition_variable should_start;
	} *data = nullptr;

	ThreadWorker::ThreadWorker(ThreadWork *processes){
		data = new foreign_data();
		data->reference_count = 1;
		data->processes = processes;
	}

	ThreadWorker::ThreadWorker(ThreadWorker const &origin){
		this->data = origin.data;
		data->reference_count++;
	}

	ThreadWorker::~ThreadWorker(){
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

	void ThreadWorker::take_off_standby(){
		if(data->inner_thread == nullptr){
			data->state = active;
			data->inner_thread = new thread(&ThreadWorker::process_cycle, this);
		}
		if(data->state == standby){
			data->should_start.notify_one();
		}
	}

	void ThreadWorker::process_cycle(){
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
}