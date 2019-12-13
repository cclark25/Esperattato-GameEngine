#include "./Bitmap/Bitmap.cpp"
#include "./Color/Color.cpp"
#include "./Display/Display.h"
#include "./Events/EventQueue.cpp"
#include "./Timer/Timer.cpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

double framerate = 90.00;

void create_display(std::string name) {
	AllegroWrappers::Display test_display(160, 90);
	for (int i = 0; i < 5; i++) {
		test_display.set_window_title(name + " " + std::to_string(i + 1));
		usleep(1000000);
	}
	usleep(1000000);
}

int main(int argc, char **argv) {
	al_init();
	al_init_image_addon();
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	AllegroWrappers::Bitmap backbuffer(1080, 720);
	std::thread t1(create_display, "Main"), t2(create_display, "Secondary");

	usleep(3000000);
	t1.swap(t2);

	t1.join();
	t2.join();
}
