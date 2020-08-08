#ifndef ESPERATTO_TYPES
#define ESPERATTO_TYPES
#include <allegro5/allegro5.h>
#include <mutex>
#include <stdexcept>

#define _Args(...) __VA_ARGS__
#define STRIP_PARENS(X) X
#define RUN_ON_INIT(className, code)                                                                                                                       \
	class __##className##_static_init                                                                                                                      \
	{                                                                                                                                                      \
	public:                                                                                                                                                \
		__##className##_static_init()                                                                                                                      \
		{                                                                                                                                                  \
                                                                                                                                                           \
			static bool isInitialized(false);                                                                                                              \
			if (isInitialized)                                                                                                                             \
			{                                                                                                                                              \
				throw std::logic_error("The class __" #className "_static_init is autogenerated for preprocessing code and should not be used manually."); \
			}                                                                                                                                              \
			isInitialized = true;                                                                                                                          \
			STRIP_PARENS(_Args code)                                                                                                                  \
		}                                                                                                                                                  \
	};                                                                                                                                                     \
	__##className##_static_init __##className##_static_obj;

namespace Esperatto
{
	typedef ALLEGRO_TRANSFORM Transform;
	typedef ALLEGRO_DISPLAY *Display;
	typedef ALLEGRO_BITMAP *Bitmap;
	typedef ALLEGRO_COLOR Color;
} // namespace Esperatto

#endif