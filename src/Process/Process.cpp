#include "Process.h"
#include "../ThreadWorker/ThreadWorker.h"
#include <allegro5/allegro.h>
#include <mutex>
#include <iostream>
using namespace std;

namespace Esperatto {

	Process::Process(std::function<void(double, ThreadWorker)> fun) {
		data = new foreign_data();
		data->last_timestamp = al_get_time();
		data->func_loop = fun;
		data->referenceCount = 1;
	}

	Process::Process(const Process &source) {
		data = source.data;
		data->referenceCount++;
	}

	Process::~Process(){
		data->referenceCount--;
		if(data->referenceCount == 0){
			delete data;
		}
	}

	double Process::Execute(ThreadWorker &worker) {
		data->call_counts++;
		data->running = true;
		data->before_timestamp = al_get_time();
		data->func_loop(data->before_timestamp - data->last_timestamp, worker);
		data->last_timestamp = al_get_time();
		data->running = false;
		data->total_runtime += data->last_timestamp - data->before_timestamp;
		data->last_runtime = data->last_timestamp - data->before_timestamp;
		data->before_timestamp = 0;
		return data->last_runtime;
	}

	double Process::get_last_runtime() { return data->last_runtime; }

	double Process::get_last_timestamp() { return data->last_timestamp; }

	double Process::get_total_runtime() { return data->total_runtime; }

	double Process::get_current_runtime() {
		if (data->running) {
			return al_get_time() - data->before_timestamp;
		} else {
			return 0;
		}
	}

	double Process::get_average_runtime() {
		return data->total_runtime / data->call_counts;
	}

	bool Process::is_running() { return data->running; }

	unsigned int Process::get_call_counts() { return data->call_counts; }

} // namespace Esperatto