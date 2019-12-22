#include "Display.h"
#include <cmath>
#include <iostream>
using namespace std;

namespace Esperattato {
	Display::Display(int width, int height, int framerate)
	    : internalDisplay(
	          (AllegroWrappers::Display::set_new_display_refresh_rate(
	               framerate),
	           width),
	          height) {

		al_inhibit_screensaver(true);
	}

	Display::~Display() { al_inhibit_screensaver(false); }

	void Display::pushFrame(Bitmap frame) {

		internalDisplay.get_backbuffer().clear_to_color(
		    Color((unsigned char)0, 0, 0));

		if (this->stretchMode == STRETCH_TO_FILL) {
			internalDisplay.get_backbuffer().draw_scaled_bitmap(
			    frame, 0, 0, frame.get_bitmap_width(),
			    frame.get_bitmap_height(), 0, 0, internalDisplay.get_width(),
			    internalDisplay.get_height(), 0);
		} else {
			Transform t;
			double newWidth = frame.get_bitmap_width();
			double newHeight = frame.get_bitmap_height();

			double xScale = ((double)internalDisplay.get_width()) /
			                (newWidth * this->pixelStretchX);
			double yScale = ((double)internalDisplay.get_height()) /
			                (newHeight * this->pixelStretchY);

			switch (this->stretchMode) {
			case MAINTAIN_ASPECT_RATIO:
				const double scale = fmin(xScale, yScale);
				newWidth *= scale;
				newHeight *= scale;
				xScale = scale;
				yScale = scale;
				break;
			}

			const double newX = (internalDisplay.get_width() -
			                     (newWidth * this->pixelStretchX)) /
			                    2.0;
			const double newY = (internalDisplay.get_height() -
			                     (newHeight * this->pixelStretchY)) /
			                    2.0;

			t.scale_transform(xScale * this->pixelStretchX,
			                  yScale * this->pixelStretchY);
			t.translate_transform(newX, newY);

			internalDisplay.get_backbuffer().use_transform(t);
			internalDisplay.get_backbuffer().draw_bitmap(frame, 0, 0, 0);
		}
		internalDisplay.flip_display();
	}

	void Display::resize(int newWidth, int newHeight) {
		this->internalDisplay.resize_display(newWidth, newHeight);
	}

	void Display::setFullscreen(bool onOff) {
		this->internalDisplay.set_display_flag(ALLEGRO_FULLSCREEN_WINDOW,
		                                       onOff);
	}

	void Display::setFrameless(bool onOff) {
		if (!onOff) {
			cerr
			    << "Warning: Attempt was made to set a display's frameless flag to OFF.\
			\nSome unknown issue results in the window not being updated with a frame.\
			\nThis error occurs on the Arch Linux OS and may be present on others too.\
			\nTo ensure that your game runs on all platforms, either do not use frameless windows or find a fix for this issue in the game engine's source code."
			    << endl;
		}
		this->internalDisplay.set_display_flag(ALLEGRO_FRAMELESS, onOff);
	}

	void Display::setMaximized(bool onOff) {
		// cerr << "Warning: Display::setMaximized not implemented because the "
		//         "underlying Allegro code does not work."
		//      << endl;
		this->internalDisplay.set_display_flag(ALLEGRO_MAXIMIZED, onOff);
	}

	void Display::setPixelStretch(double stretchFactorX,
	                              double stretchFactorY) {
		this->pixelStretchX = stretchFactorX;
		this->pixelStretchY = stretchFactorY;
	}
} // namespace Esperattato