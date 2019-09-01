#ifndef ESPERATTATO_PROCESS_DEF
#define ESPERATTATO_PROCESS_DEF


namespace Esperattato {
	class ThreadWorker;

	class Process {	
		void (*func_loop)(double, ThreadWorker);
		double last_timestamp;
		unsigned int call_counts = 0;
		double total_runtime = 0;
		double last_runtime = 0;
		double before_timestamp = 0;
		bool running = false;
	public:
		Process();

		double Execute(ThreadWorker worker);

		double get_last_runtime();

		double get_last_timestamp();

		double get_total_runtime();

		double get_current_runtime();

		double get_average_runtime();

		bool is_running();

		unsigned int get_call_counts();
	};
}

#endif