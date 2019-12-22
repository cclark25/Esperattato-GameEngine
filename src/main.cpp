// #include "./ThreadPool/ThreadWorker.h"
// #include "./ThreadPool/Process.h"
#include "Display/Display.h"
#include <allegro5/allegro_image.h>
#include <cmath>
#include <iostream>
#include <unistd.h>

using namespace Esperatto;

int main(int argc, char **args) {
	al_init();
	al_init_image_addon();

	Bitmap b(args[1]);
	Bitmap canvas(256, 224);
	cout << "Width: " << b.get_bitmap_width()
	     << "\tHeight: " << b.get_bitmap_height() << endl;
	Esperatto::Display d(512, 256, 60);
	d.setPixelStretch(7.0 / 6.0, 1.0);
	Transform t;
	int i = 0;
	while (true) {
		while (i < canvas.get_bitmap_width() - b.get_bitmap_width()) {
			t.translate_transform(1, 0);
			canvas.use_transform(t);
			canvas.clear_to_color(Color((unsigned char)0, 0, 0));
			canvas.draw_bitmap(b, 0, 0, 0);
			d.pushFrame(canvas);

			usleep(1000000.0 / 24.0);
			i += 1;
		}
		cout << "Switching directions." << endl;
		while (i > 0) {
			t.translate_transform(-1, 0);
			canvas.use_transform(t);
			canvas.clear_to_color(Color((unsigned char)0, 0, 0));
			canvas.draw_bitmap(b, 0, 0, 0);
			d.pushFrame(canvas);

			usleep(1000000.0 / 24.0);
			i -= 1;
		}
		// cout << i << endl << sin(i) << endl << endl;

		// i = -1;
	}

	return 0;
}