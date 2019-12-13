// #include "./ThreadPool/ThreadWorker.h"
// #include "./ThreadPool/Process.h"
#include <iostream>
#include <unistd.h>
#include "Display/Display.h"

using namespace Esperattato;

int main(){
	al_init();
	Display d = Display(256, 128, 60);
	usleep(60000000);
	return 0;
}