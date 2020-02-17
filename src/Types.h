#ifndef ESPERATTO_TYPES
#define ESPERATTO_TYPES
#include <allegro5/allegro5.h>

namespace Esperatto {

	auto AllegroInitiated = al_install_system(ALLEGRO_VERSION_INT, nullptr);
	typedef ALLEGRO_TRANSFORM Transform;
	typedef ALLEGRO_DISPLAY *Display;
	typedef ALLEGRO_BITMAP *Bitmap;
	typedef ALLEGRO_COLOR Color;

} // namespace Esperatto

#endif