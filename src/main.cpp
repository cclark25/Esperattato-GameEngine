// #include "./ThreadPool/ThreadWorker.h"
// #include "./ThreadPool/Process.h"
#include "Display/Display.h"
#include <allegro5/allegro_image.h>
#include <cmath>
#include <iostream>
#include <unistd.h>

using namespace Esperattato;

int main(int argc, char **args) {
	al_init();
	al_init_image_addon();

	Bitmap b(args[1]);
	// Bitmap canvas(b.get_bitmap_width()+1, b.get_bitmap_height()+1);
	cout << "Width: " << b.get_bitmap_width() << "\tHeight: " << b.get_bitmap_height() << endl;
	Esperattato::Display d(512, 256, 60);
	d.setPixelStretch(7.0/6.0, 1.0);
	int i = 0;
	while (true) {
		// while (i < 1) {
		d.pushFrame(b);
		d.setPixelStretch(1, (((i % 2) == 0) ? (7.0/6.0) : 1));
		usleep(1000000);
		i++;
		// }
		// cout << i << endl << sin(i) << endl << endl;

		// i = -1;
	}

	return 0;
}