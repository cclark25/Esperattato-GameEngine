#ifndef ESPERATTATO_DISPLAY_DEF
#define ESPERATTATO_DISPLAY_DEF
#include "../../Dependencies/AllegroCPPWrappers/src/Bitmap/Bitmap.h"
#include "../../Dependencies/AllegroCPPWrappers/src/Display/Display.h"
#include <mutex>
#include <thread>

using namespace std;
using namespace AllegroWrappers;

namespace Esperattato {
	enum STRETCH_MODE : unsigned char {
		MAINTAIN_ASPECT_RATIO,
		STRETCH_TO_FILL,
		STRETCH_HORIZONTALLY,
		STRETCH_VERTICALLY
	};

	class Display {
	  private:
		AllegroWrappers::Display internalDisplay;
		double pixelStretchX = 1;
		double pixelStretchY = 1;

	  protected:
	  public:
		STRETCH_MODE stretchMode = MAINTAIN_ASPECT_RATIO;

		Display(Display &source);
		Display(int width, int height, int framerate);
		~Display();
		void pushFrame(Bitmap frame);
		void resize(int newWidth, int newHeight);
		void setFullscreen(bool onOff);
		void setFrameless(bool onOff);
		void setMaximized(bool onOff);

		/* 	Used to specify a stretch to apply to an image before stretching it
		   to meet the Display's resolution. Useful for immitating the SNES
		   graphics that stretch pixels horizontally by a ratio of 7.0:6.0 to
		   fit 4:3 TV sets.
		*/
		void setPixelStretch(double stretchX, double stretchY);
	};
} // namespace Esperattato

#endif