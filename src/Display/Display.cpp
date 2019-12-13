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
				internalDisplay.flip_display();
			}
			usleep(waitTime);
			count++;
		}
	}

	Display::Display(int width, int height, int framerate) {
		managingThread =
		    new thread(&Display::manageDisplay, this, width, height, framerate);
	}

	Display::~Display() {
		shouldStop = true;
		managingThread->join();
	}
} // namespace Esperattato