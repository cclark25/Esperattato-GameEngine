#include <allegro5/allegro.h>
#ifndef ALLEGRO_WRAPPERS_COLOR_DEF
#define ALLEGRO_WRAPPERS_COLOR_DEF
namespace AllegroWrappers {

	class Color : public ALLEGRO_COLOR {
		public:
			Color(ALLEGRO_COLOR base_color) : ALLEGRO_COLOR(base_color){};

			Color(unsigned char red, unsigned char green, unsigned char blue) : ALLEGRO_COLOR(al_map_rgb(red, green, blue)){};
			Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) : ALLEGRO_COLOR(al_map_rgba(red, green, blue, alpha)){};
			Color(float red, float green, float blue) : ALLEGRO_COLOR(al_map_rgb_f(red, green, blue)){};
			Color(float red, float green, float blue, float alpha) : ALLEGRO_COLOR(al_map_rgba_f(red, green, blue, alpha)){};
			
			unsigned char a_char(){
				return a * 255;
			}
			unsigned char r_char(){
				return r * 255;
			}
			unsigned char g_char(){
				return g * 255;
			}
			unsigned char b_char(){
				return b * 255;
			}

			static Color premul_rgba_f(float red, float green, float blue, float alpha){
				return Color(red * alpha, green * alpha, blue * alpha, alpha);
			}

			static Color premul_rgba(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha){
				return Color(red * alpha / 255, green * alpha / 255, blue * alpha / 255, alpha);
			}
	};

}

#endif