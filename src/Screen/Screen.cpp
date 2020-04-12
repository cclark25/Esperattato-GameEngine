#include "Screen.h"
#include <cmath>
#include <iostream>
using namespace std;

namespace Esperatto
{
Screen::Screen(int width, int height, int framerate)
{
	al_set_new_display_refresh_rate(framerate);
	while (!al_is_system_installed())
	{
		al_init();
	}
	internalDisplay = al_create_display(width, height);
	al_flip_display();
	al_inhibit_screensaver(true);
}

Screen::~Screen()
{
	al_inhibit_screensaver(false);
	// delete this->internalDisplay;
}

void Screen::pushFrame(Bitmap frame)
{
	al_set_target_backbuffer(internalDisplay);
	al_clear_to_color(al_map_rgb(0, 0, 0));

	if (this->stretchMode == STRETCH_TO_FILL)
	{
		al_draw_scaled_bitmap(frame, 0, 0, al_get_bitmap_width(frame),
							  al_get_bitmap_height(frame), 0, 0,
							  al_get_display_width(internalDisplay),
							  al_get_display_height(internalDisplay), 0);
	}
	else
	{
		Transform t;
		al_identity_transform(&t);
		double newWidth = al_get_bitmap_width(frame);
		double newHeight = al_get_bitmap_height(frame);
		double xScale = ((double)al_get_display_width(internalDisplay)) /
						(newWidth * this->pixelStretchX);
		double yScale = ((double)al_get_display_height(internalDisplay)) /
						(newHeight * this->pixelStretchY);

		switch (this->stretchMode)
		{
		case MAINTAIN_ASPECT_RATIO:
		{
			const double scale = fmin(xScale, yScale);
			newWidth *= scale;
			newHeight *= scale;
			xScale = scale;
			yScale = scale;
			break;
		}
		case STRETCH_TO_FILL:
			break;
		case STRETCH_HORIZONTALLY:
			break;
		case STRETCH_VERTICALLY:
			break;
		}

		const double newX = (al_get_display_width(internalDisplay) -
							 (newWidth * this->pixelStretchX)) /
							2.0;
		const double newY = (al_get_display_height(internalDisplay) -
							 (newHeight * this->pixelStretchY)) /
							2.0;

		al_scale_transform(&t, xScale * this->pixelStretchX,
						   yScale * this->pixelStretchY);
		al_translate_transform(&t, newX, newY);

		al_set_target_backbuffer(internalDisplay);
		al_use_transform(&t);

		al_draw_bitmap(frame, 0, 0, 0);
	}
	al_flip_display();
}

void Screen::resize(int newWidth, int newHeight)
{
	al_resize_display(internalDisplay, newWidth, newHeight);
}

void Screen::setFullscreen(bool onOff)
{
	al_set_display_flag(internalDisplay, ALLEGRO_FULLSCREEN_WINDOW, onOff);
}

void Screen::setFrameless(bool onOff)
{
	if (!onOff)
	{
		cerr
			<< "Warning: Attempt was made to set a display's frameless flag to OFF.\
			\nSome unknown issue results in the window not being updated with a frame.\
			\nThis error occurs on the Arch Linux OS and may be present on others too.\
			\nTo ensure that your game runs on all platforms, either do not use frameless windows or find a fix for this issue in the game engine's source code."
			<< endl;
	}
	al_set_display_flag(internalDisplay, ALLEGRO_FRAMELESS, onOff);
}

void Screen::setMaximized(bool onOff)
{
	// cerr << "Warning: Display::setMaximized not implemented because the "
	//         "underlying Allegro code does not work."
	//      << endl;
	al_set_display_flag(internalDisplay, ALLEGRO_MAXIMIZED, onOff);
}

void Screen::setPixelStretch(double stretchFactorX, double stretchFactorY)
{
	this->pixelStretchX = stretchFactorX;
	this->pixelStretchY = stretchFactorY;
}
} // namespace Esperatto