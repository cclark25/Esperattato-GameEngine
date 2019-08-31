#include <mutex>
#include <allegro5/allegro.h>

#ifndef ESPERATTATO_PROCESS_DEF
#define ESPERATTATO_PROCESS_DEF

namespace Esperattato {
	class Process {
		void (func_loop)(double, ThreadWorker);
		double last_timestamp;
		unsigned int call_counts = 0;
		double total_runtime = 0;
		double last_runtime = 0;
		double before_timestamp = 0;
		bool running = false;
	
	public:
		Process(){
			last_timestamp = al_get_time();
		}

		double Execute(ThreadWorker worker){
			call_counts++;
			before_timestamp = al_get_time();
			running = true;
			func_loop(before_timestamp - last_timestamp, worker);
			running = false;
			last_timestamp = al_get_time();
			total_runtime += last_timestamp - before_timestamp;
			last_runtime = last_timestamp - before_timestamp;
			before_timestamp = 0;
			return last_runtime;
		}

		double get_last_runtime(){
			return last_runtime;
		}

		double get_last_timestamp(){
			return last_timestamp;
		}

		double get_total_runtime(){
			return total_runtime;
		}

		double get_current_runtime(){
			if(running){
				return al_get_time() - before_timestamp;
			}
			else {
				return 0;
			}
		}

		double get_average_runtime(){
			return total_runtime / call_counts;
		}

		bool is_running(){
			return running;
		}

		unsigned int get_call_counts(){
			return call_counts;
		}
	};
}

#endif