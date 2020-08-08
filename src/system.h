#ifndef ESPERATTO_SYSTEM
#define ESPERATTO_SYSTEM

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>

namespace Esperatto
{
	void initSystem()
	{
		static bool isInitialized(false);
		if (!isInitialized)
		{
			al_init();

			al_init_image_addon();
			al_init_primitives_addon();
			al_install_audio();
		}
	}

	
} // namespace Esperatto

#endif