#include "Display.h"
#include <iostream>
using namespace std;

namespace Esperattato {
	void Display::manageDisplay(int width, int height, int framerate) {
		AllegroWrappers::Display internalDisplay(width, height);
		const uint waitTime = 1000000 / (framerate);
		uint count = 0;
		while (!shouldStop) {
			if (count % framerate == 0) {
				cout << "Here: " << count << endl;
			}
			if (this->newFrame) {
				this->frameLock.lock();
				
				unsigned char r = 0;
				unsigned char g = 0;
				unsigned char b = 0;

				internalDisplay.get_backbuffer().clear_to_color(Color(r, g, b));
				internalDisplay.get_backbuffer().draw_bitmap(this->currentFrame,
				                                             0, 0, 0);
				this->frameLock.unlock();
			}
			internalDisplay.flip_display();
			usleep(waitTime);
			count++;
		}
	}

	Display::Display(int width, int height, int framerate)
	    : currentFrame(0, 0) {
		managingThread =
		    new thread(&Display::manageDisplay, this, width, height, framerate);
	}

	Display::~Display() {
		shouldStop = true;
		managingThread->join();
	}

	void Display::pushFrame(Bitmap frame) {
		this->frameLock.lock();
		this->currentFrame = frame;
		this->newFrame = true;
		this->frameLock.unlock();
	}
} // namespace Esperattato