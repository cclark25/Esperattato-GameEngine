#include "Animation.h"
#include "../system.h"
#include <iostream>

namespace Esperatto
{
	Animation::Animation(string path, unsigned int columnCount,
						 unsigned int rowCount, double framerate)
		: Image(path)
	{
		animData = shared_ptr<animationData>(new animationData());
		animData->referenceCount = 1;
		animData->baseImage = data->internal;
		const unsigned int colWidth =
			al_get_bitmap_width(animData->baseImage) / columnCount;
		const unsigned int rowHeight =
			al_get_bitmap_height(animData->baseImage) / rowCount;

		for (unsigned int i = 0; i < columnCount; i++)
		{
			for (unsigned int j = 0; j < rowCount; j++)
			{
				Bitmap sub =
					al_create_sub_bitmap(animData->baseImage, i * colWidth,
										 j * rowHeight, colWidth, rowHeight);
				animData->frameList.push_back(sub);
			}
		}

		animData->frameRate = framerate / Animation::maxFrameRate;
		animData->frameCount = animData->frameList.size();
		// animData->timer = al_create_timer(1.0 / framerate);
		// al_start_timer(animData->timer);
	}

	Animation::Animation(const Animation &source) : Image(source)
	{
		this->animData = source.animData;
		this->animData->referenceCount++;
	}

	Animation::~Animation()
	{
		static mutex m;
		m.lock();
		animData->referenceCount--;
		if (animData->referenceCount == 0)
		{
			cout << "animDate pointer: " << typeid(animData).name() << endl;
			cout << "frameList: " << animData->frameList.size() << endl;

			for (auto it = animData->frameList.begin(); it != animData->frameList.end(); it++)
			{
				al_destroy_bitmap(*it);
				(*it) = nullptr;
			}
			animData->frameList.clear();
			cout << "After clear: " << animData->frameList.size() << endl;

			data->internal = animData->baseImage;
			// al_destroy_timer(animData->timer);
			// animData->timer = nullptr;
			// delete animData;
			animData = nullptr;
		}
		m.unlock();
	}

	unsigned int Animation::getFrame()
	{
		return int(al_get_timer_count(Animation::timer) * animData->frameRate) % animData->frameCount;
		// return 0;
	}

	void Animation::setFrame(unsigned int frame)
	{
		al_set_timer_count(Animation::timer, frame);
	}

	Bitmap Animation::incrementFrame()
	{
		al_add_timer_count(Animation::timer, 1);
		return animData->frameList.at(getFrame());
	}

	Bitmap Animation::getBitmap()
	{
		Bitmap f = animData->frameList.at(getFrame());
		return f;
	}

	ALLEGRO_TIMER *Animation::timer = nullptr;

	class abcd
	{
	public:
		abcd()
		{
			initSystem();
			Esperatto::Animation::timer = al_create_timer(1.0 / (Animation::maxFrameRate));
			al_start_timer(Esperatto::Animation::timer);
		}
	} blah;

} // namespace Esperatto