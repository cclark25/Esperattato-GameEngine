#include "./ThreadPool/ThreadWorker.h"
#include "./ThreadPool/Process.h"
#include <iostream>
#include <unistd.h>

using namespace Esperattato;

void f(double, ThreadWorker){	
	static unsigned int f;
	f++;
	std::cout << "Processing F: " << f << std::endl;
	usleep(500000);

}
void g(double, ThreadWorker){
	static unsigned int g;
	g++;
	std::cout << "Processing G: " << g << std::endl;
	usleep(500000);
}

int main(){
	ThreadWork work;

	work.second.push(Process(f));
	work.second.push(Process(g));

	ThreadWorker worker1(&work);
	worker1.take_off_standby();

	ThreadWorker worker2(&work);
	worker2.take_off_standby();
	for(int x = 0; x < 5; x++){
		usleep(5000000);
		work.first.lock();
		std::cout << "Locked the work queue." << std::endl;
		usleep(5000000);
		std::cout << "Unlocking the work queue." << std::endl;
		work.first.unlock();
	}

	worker1.kill_worker();
	worker2.kill_worker();
	return 0;
}