#include "./ThreadPool/ThreadWorker.h"
#include "./ThreadPool/Process.h"

using namespace Esperattato;

int main(){
	ThreadWork work;

	work.first.lock();
	work.first.unlock();

	return 0;
}