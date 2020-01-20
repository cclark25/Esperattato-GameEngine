#ifndef ESPERATTO_ANIMATION
#define ESPERATTO_ANIMATION
#include "../Image/Image.h"
#include <vector>

namespace Esperatto {
	class Animation : public Image {
		struct animationData {
			unsigned int referenceCount = 0;
			vector<Bitmap> frameList;
			Bitmap baseImage;
			ALLEGRO_TIMER *timer = nullptr;
		} * animData;

	  public:
		// Use this constructor when constructing a new Animation from a sprite
		// sheet layed out in a grid.
		Animation(string path, unsigned int columnCount, unsigned int rowCount, double framerate);
		Animation(const Animation &source);
		~Animation();

		unsigned int getFrame();
		void setFrame(unsigned int frame);
		Bitmap incrementFrame();
		Bitmap getBitmap();
	};
} // namespace Esperatto

#endif