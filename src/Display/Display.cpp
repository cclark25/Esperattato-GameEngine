#include "Display.h"
#include <cmath>
#include <iostream>
using namespace std;

namespace Esperattato {
	void Display::manageDisplay(int width, int height, int framerate) {
		bool fullscreen = false;
		bool frameless = false;
		bool maximized = false;
		AllegroWrappers::Display::set_new_display_refresh_rate(framerate);
		AllegroWrappers::Display internalDisplay(width, height);
		internalDisplay.set_display_flag(ALLEGRO_FULLSCREEN_WINDOW, fullscreen);
		const uint waitTime = 1000000 / (framerate);
		uint count = 0;
		while (!shouldStop) {
			this->frameLock.lock();
			if (count % framerate == 0) {
				cout << "Here: " << count << endl;
			}
			if (this->maximized != maximized) {
				bool result = internalDisplay.set_display_flag(
				    ALLEGRO_MAXIMIZED, this->maximized);
				cout << "Maximized: " << (result ? "true" : "false") << endl;
				maximized = this->maximized;
				this->newFrame = true;
			}
			if (this->fullScreen != fullscreen) {
				bool success = internalDisplay.set_display_flag(
				    ALLEGRO_FULLSCREEN_WINDOW, this->fullScreen);

				this->width = internalDisplay.get_width();
				this->height = internalDisplay.get_height();
				// this->resized = true;
				fullscreen = this->fullScreen;
				this->newFrame = true;
			}
			if (this->frameless != frameless) {
				bool result = internalDisplay.set_display_flag(
				    ALLEGRO_FRAMELESS, this->frameless);
				frameless = this->frameless;
				this->newFrame = true;
			}
			if (this->resized) {
				internalDisplay.resize_display(this->width, this->height);
				this->resized = false;
				this->newFrame = true;
			}
			if (this->newFrame) {
				unsigned char r = 0;
				unsigned char g = 0;
				unsigned char b = 0;
				internalDisplay.get_backbuffer().clear_to_color(Color(r, g, b));

				if (this->stretchMode == STRETCH_TO_FILL) {
					internalDisplay.get_backbuffer().draw_scaled_bitmap(
					    this->currentFrame, 0, 0,
					    this->currentFrame.get_bitmap_width(),
					    this->currentFrame.get_bitmap_height(), 0, 0,
					    internalDisplay.get_width(),
					    internalDisplay.get_height(), 0);
				} else {
					double newWidth = this->currentFrame.get_bitmap_width();
					double newHeight = this->currentFrame.get_bitmap_height();
					newWidth *= this->pixelStretchX;
					newHeight *= this->pixelStretchY;

					const double xScale =
					    ((double)internalDisplay.get_width()) / (newWidth);
					const double yScale =
					    ((double)internalDisplay.get_height()) / (newHeight);

					switch (this->stretchMode) {
					case MAINTAIN_ASPECT_RATIO:
						newWidth *= fmin(xScale, yScale);
						newHeight *= fmin(xScale, yScale);
						break;
					}

					const double newX =
					    (internalDisplay.get_width() - newWidth) / 2;
					const double newY =
					    (internalDisplay.get_height() - newHeight) / 2;
					internalDisplay.get_backbuffer().draw_scaled_bitmap(
					    this->currentFrame, 0, 0,
					    this->currentFrame.get_bitmap_width(),
					    this->currentFrame.get_bitmap_height(), newX, newY,
					    newWidth, newHeight, 0);
				}
				this->newFrame = false;
				internalDisplay.flip_display();
			}
			this->frameLock.unlock();
			usleep(waitTime);
			count++;
		}
	}

	Display::Display(int width, int height, int framerate)
	    : currentFrame(0, 0) {
		managingThread =
		    new thread(&Display::manageDisplay, this, width, height, framerate);
		this->width = width;
		this->height = height;
		al_inhibit_screensaver(true);
	}

	Display::~Display() {
		shouldStop = true;
		managingThread->join();
		al_inhibit_screensaver(false);
	}

	void Display::pushFrame(Bitmap frame) {
		this->frameLock.lock();
		this->currentFrame = frame;
		this->newFrame = true;
		this->frameLock.unlock();
	}

	void Display::resize(int newWidth, int newHeight) {
		this->frameLock.lock();
		this->width = newWidth;
		this->height = newHeight;
		this->resized = true;
		this->frameLock.unlock();
	}

	void Display::setFullscreen(bool onOff) {
		this->frameLock.lock();
		this->fullScreen = onOff;
		this->frameLock.unlock();
	}

	void Display::setFrameless(bool onOff) {
		this->frameLock.lock();
		this->frameless = onOff;
		this->frameLock.unlock();
		if (!onOff) {
			cerr
			    << "Warning: Attempt was made to set a display's frameless flag to OFF.\
			\nSome unknown issue results in the window not being updated with a frame.\
			\nThis error occurs on the Arch Linux OS and may be present on others too.\
			\nTo ensure that your game runs on all platforms, either do not use frameless windows or find a fix for this issue in the game engine's source code."
			    << endl;
		}
	}

	void Display::setMaximized(bool onOff) {
		// this->frameLock.lock();
		// this->maximized = onOff;
		// this->frameLock.unlock();
		cerr << "Warning: Display::setMaximized not implemented because the "
		        "underlying Allegro code does not work."
		     << endl;
	}

	void Display::setPixelStretch(double stretchFactorX,
	                              double stretchFactorY) {
		this->pixelStretchX = stretchFactorX;
		this->pixelStretchY = stretchFactorY;
		this->newFrame = true;
	}
} // namespace Esperattato