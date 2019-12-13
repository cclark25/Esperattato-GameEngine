#ifndef ALLEGRO_WRAPPERS_DISPLAY_DEF
#define ALLEGRO_WRAPPERS_DISPLAY_DEF
#include "../Bitmap/Bitmap.cpp"
#include <allegro5/allegro.h>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>

namespace AllegroWrappers {
	class Display {
	  private:
		static std::set<std::thread::id> thread_list;

		bool exists_on_thread();

	  protected:
		Bitmap *backbuffer = nullptr;

	  public:
		struct foreign_data {
			ALLEGRO_DISPLAY *display;
			unsigned int reference_count;
		} *data;

		// Constructors

		Display(int width, int height);

		Display(Display &origin);

		// Backbuffer retreiver

		Bitmap get_backbuffer();

		// Member functions
		void flip_display(void);

		bool resize_display(int width, int height);

		bool acknowledge_resize();

		void apply_window_constraints(bool on_off);

		// Properties
		int get_width();

		int get_height();

		auto get_window_position();

		void set_window_position(int x, int y);

		auto get_window_constraints(int *min_width, int *min_h, int *max_w,
		                            int *max_h);

		bool set_window_constraints(int min_width, int min_height,
		                            int max_width, int max_height);

		int get_display_flags();

		bool set_display_flag(int flag, bool on_off);

		int get_display_option(int option);

		void set_display_option(int option, int value);

		int get_display_format();

		int get_display_orientation();

		int get_display_refresh_rate();

		void set_window_title(const std::string &title);

		void set_display_icon(Bitmap icon);

		// I'm not sure whether the scenario where this one would be useful is
		// worth the effort to code a wrapper for it. void
		// set_display_icons(std::vector<Bitmap> icons){
		// 	al_set_display_icons(data->display, icons.size(), )
		// }

		// Static Functions
		static int get_new_display_refresh_rate();

		static void set_new_display_refresh_rate(int rate);

		// Destructor
		~Display();
	};

} // namespace AllegroWrappers

#endif