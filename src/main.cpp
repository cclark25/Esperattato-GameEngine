#include "./ThreadPool/ThreadWorker.h"
#include "./ThreadPool/Process.h"
#include <iostream>
#include <unistd.h>

using namespace Esperattato;

void f(double, ThreadWorker){
	std::cout << "Processing F\n";
}
void g(double, ThreadWorker){
	std::cout << "Processing G\n";
}

int main(){
	ThreadWork work;

	work.second.push(Process(f));
	work.second.push(Process(g));

	ThreadWorker worker1(&work);
	worker1.take_off_standby();

	ThreadWorker worker2(&work);
	worker2.take_off_standby();

	usleep(5000000);
	return 0;
}