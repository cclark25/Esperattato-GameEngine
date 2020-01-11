#include "../Types.h"
#include <string>
using namespace std;

namespace Esperatto {
	class Image {
		Bitmap internal;

	  public:
		Image(string path) {
			internal = al_load_bitmap(path.c_str());
		}
		Bitmap getBitmap(){
			return internal;
		}
		~Image() { al_destroy_bitmap(internal); }
	};
} // namespace Esperatto