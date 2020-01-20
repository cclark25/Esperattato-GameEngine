#include "../Types.h"
#include <string>
using namespace std;

namespace Esperatto {
	class Image {
		Bitmap internal;

	  public:
		Image(string path) {
			internal = al_load_bitmap(path.c_str());
			al_lock_bitmap(internal, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
		}
		Bitmap getBitmap(){
			return internal;
		}
		~Image() { al_destroy_bitmap(internal); }
	};
} // namespace Esperatto