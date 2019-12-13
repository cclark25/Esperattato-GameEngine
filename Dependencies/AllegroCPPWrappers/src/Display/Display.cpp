#include "Display.h"
#include "../Bitmap/Bitmap.cpp"
#include <allegro5/allegro.h>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>
#include <iostream>

namespace AllegroWrappers {
	std::set<std::thread::id> Display::thread_list;

	bool Display::exists_on_thread() {
		std::set<std::thread::id>::iterator it = Display::thread_list.end();
		it = Display::thread_list.find(std::this_thread::get_id());
		bool result = it != Display::thread_list.end();
		std::cout << "Here: " << result << std::endl;
		return result;
	}

	// Constructors

	Display::Display(int width, int height) {
		// Ensures there is only ever one Display on the thread. This is
		// necessary as there is no way to set the current display of the
		// thread. For now it is just a boolean, but in the future it will
		// be a function taking a thread pointer so that separate threads
		// can have their own Displays.
		if (Display::exists_on_thread()) {
			throw std::logic_error("Tried to create new Display when one "
			                       "already exists on thread!");
		}
		this->thread_list.insert(std::this_thread::get_id());
		data = new foreign_data();
		data->display = al_create_display(width, height);
		data->reference_count = 1;
		backbuffer = new Bitmap(al_get_backbuffer(data->display));
	}

	Display::Display(Display &origin) : backbuffer(origin.backbuffer) {
		data = origin.data;
		data->reference_count++;
	}

	// Backbuffer retreiver

	Bitmap Display::get_backbuffer() { return *backbuffer; }

	// Member functions
	void Display::flip_display(void) { al_flip_display(); }

	bool Display::resize_display(int width, int height) {
		return al_resize_display(data->display, width, height);
	}

	bool Display::acknowledge_resize() {
		return al_acknowledge_resize(data->display);
	}

	void Display::apply_window_constraints(bool on_off) {
		al_apply_window_constraints(data->display, on_off);
	}

	// Properties
	int Display::get_width() { return al_get_display_width(data->display); }

	int Display::get_height() { return al_get_display_height(data->display); }

	auto Display::get_window_position() {
		struct position {
			int x;
			int y;
		} pos;
		al_get_window_position(data->display, &pos.x, &pos.y);
		return pos;
	}

	void Display::set_window_position(int x, int y) {
		al_set_window_position(data->display, x, y);
	}

	auto Display::get_window_constraints(int *min_width, int *min_h, int *max_w,
	                                     int *max_h) {
		struct constraints {
			int min_width, min_height, max_width, max_height;
		} result;
		al_get_window_constraints(data->display, &result.min_width,
		                          &result.min_height, &result.max_width,
		                          &result.max_height);
		return result;
	}

	bool Display::set_window_constraints(int min_width, int min_height,
	                                     int max_width, int max_height) {
		return al_set_window_constraints(data->display, min_width, min_height,
		                                 max_width, max_height);
	}

	int Display::get_display_flags() {
		return al_get_display_flags(data->display);
	}

	bool Display::set_display_flag(int flag, bool on_off) {
		return al_set_display_flag(data->display, flag, on_off);
	}

	int Display::get_display_option(int option) {
		return al_get_display_option(data->display, option);
	}

	void Display::set_display_option(int option, int value) {
		return al_set_display_option(data->display, option, value);
	}

	int Display::get_display_format() {
		return al_get_display_format(data->display);
	}

	int Display::get_display_orientation() {
		return al_get_display_orientation(data->display);
	}

	int Display::get_display_refresh_rate() {
		return al_get_display_refresh_rate(data->display);
	}

	void Display::set_window_title(const std::string &title) {
		al_set_window_title(data->display, title.c_str());
	}

	void Display::set_display_icon(Bitmap icon) {
		al_set_display_icon(data->display, icon.data->bitmap);
	}

	// I'm not sure whether the scenario where this one would be useful is
	// worth the effort to code a wrapper for it. void
	// set_display_icons(std::vector<Bitmap> icons){
	// 	al_set_display_icons(data->display, icons.size(), )
	// }

	// Static Functions
	int Display::get_new_display_refresh_rate() {
		return al_get_new_display_refresh_rate();
	}

	void Display::set_new_display_refresh_rate(int rate) {
		al_set_new_display_refresh_rate(rate);
	}

	// Destructor
	Display::~Display() {
		data->reference_count--;
		if (data->reference_count == 0) {
			al_destroy_display(data->display);
			this->thread_list.erase(std::this_thread::get_id());
			delete data;
			// Display::exists_on_thread = false;
		}
	}

} // namespace AllegroWrappers
