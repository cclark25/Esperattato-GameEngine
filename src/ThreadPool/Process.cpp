#include "Process.h"
#include <mutex>
#include <allegro5/allegro.h>
#include "ThreadWorker.h"

namespace Esperattato {
	
	Process::Process(){
		last_timestamp = al_get_time();
	}

	double Process::Execute(ThreadWorker worker){
		call_counts++;
		running = true;
		before_timestamp = al_get_time();
		func_loop(before_timestamp - last_timestamp, worker);
		last_timestamp = al_get_time();
		running = false;
		total_runtime += last_timestamp - before_timestamp;
		last_runtime = last_timestamp - before_timestamp;
		before_timestamp = 0;
		return last_runtime;
	}

	double Process::get_last_runtime(){
		return last_runtime;
	}

	double Process::get_last_timestamp(){
		return last_timestamp;
	}

	double Process::get_total_runtime(){
		return total_runtime;
	}

	double Process::get_current_runtime(){
		if(running){
			return al_get_time() - before_timestamp;
		}
		else {
			return 0;
		}
	}

	double Process::get_average_runtime(){
		return total_runtime / call_counts;
	}

	bool Process::is_running(){
		return running;
	}

	unsigned int Process::get_call_counts(){
		return call_counts;
	}
	
}