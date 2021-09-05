#ifndef ESPERATTO_ANIMATION
#define ESPERATTO_ANIMATION
#include "../Image/Image.h"
#include <vector>
#include <mutex>
#include <memory>

namespace Esperatto
{
	class Animation : public Image
	{
		struct animationData
		{
			unsigned int referenceCount = 0;
			vector<Bitmap> frameList;
			double frameRate = 1;
			unsigned int frameCount = 0;
			Bitmap baseImage;
		};
		std::shared_ptr<animationData> animData;

	public:
		// Use this constructor when constructing a new Animation from a sprite
		// sheet layed out in a grid.
		Animation(string path, unsigned int columnCount, unsigned int rowCount, double framerate);
		Animation(const Animation &source);
		~Animation();

		static ALLEGRO_TIMER *timer;
		static constexpr double maxFrameRate = 60;

		unsigned int getFrame();
		void setFrame(unsigned int frame);
		Bitmap incrementFrame();
		Bitmap getBitmap();
	};

	// RUN_ON_INIT(Animation, (
	// 						   Esperatto::Animation::timer = al_create_timer(1.0 / 120);
	// 						   al_start_timer(Esperatto::Animation::timer);))

} // namespace Esperatto

#endif