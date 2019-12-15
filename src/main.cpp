// #include "./ThreadPool/ThreadWorker.h"
// #include "./ThreadPool/Process.h"
#include "Display/Display.h"
#include <allegro5/allegro_image.h>
#include <iostream>
#include <unistd.h>

using namespace Esperattato;

int main() {
	al_init();
	al_init_image_addon();
	Bitmap b("/home/christopher/GIT/Esperattato/TestSprite.png");
	Bitmap b2("/home/christopher/GIT/Esperattato/TestSprite2.png");
	Esperattato::Display d(512, 256, 60);
	cout << "Width: " << b.get_bitmap_width()
	     << "\tHeight: " << b.get_bitmap_height() << endl;

	while (true) {
		d.pushFrame(b);
		d.setPixelStretch(1, 1);
		usleep(1000000);
		d.pushFrame(b2);
		d.setPixelStretch(7.0 / 6.0, 1);
		usleep(1000000);
	}
	return 0;
}