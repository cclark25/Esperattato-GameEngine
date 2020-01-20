#ifndef ESPERATTATO_PROCESS_DEF
#define ESPERATTATO_PROCESS_DEF

#include <functional>

namespace Esperatto {
	class ThreadWorker;

	class Process {
		struct foreign_data {
			std::function<void(double, ThreadWorker)> func_loop;
			double last_timestamp;
			unsigned int call_counts = 0;
			double total_runtime = 0;
			double last_runtime = 0;
			double before_timestamp = 0;
			bool running = false;
			unsigned int referenceCount = 0;
		} * data;

	  public:
		Process(std::function<void(double, ThreadWorker)> fun);
		Process(const Process &source);
		~Process();

		double Execute(ThreadWorker &worker);

		double get_last_runtime();

		double get_last_timestamp();

		double get_total_runtime();

		double get_current_runtime();

		double get_average_runtime();

		bool is_running();

		unsigned int get_call_counts();
	};
} // namespace Esperatto

#endif