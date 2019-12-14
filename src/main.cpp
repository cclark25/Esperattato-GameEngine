// #include "./ThreadPool/ThreadWorker.h"
// #include "./ThreadPool/Process.h"
#include <iostream>
#include <unistd.h>
#include "Display/Display.h"
#include <allegro5/allegro_image.h>

using namespace Esperattato;

int main(){
	al_init();
	al_init_image_addon();
	Bitmap b("/home/christopher/GIT/Esperattato/Dependencies/AllegroCPPWrappers/test.png");
	Bitmap b2("/home/christopher/GIT/Esperattato/Dependencies/AllegroCPPWrappers/test.bmp");
	Esperattato::Display d(512, 256, 60);
	cout << "Width: " << b.get_bitmap_width() << "\tHeight: " << b.get_bitmap_height() << endl;
	usleep(10000000);
	d.pushFrame(b);
	usleep(10000000);
	d.pushFrame(b2);
	usleep(60000000);
	return 0;
}