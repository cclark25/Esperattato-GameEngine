#ifndef ESPERATTATO_DISPLAY_DEF
#define ESPERATTATO_DISPLAY_DEF
#include "../Dependencies/AllegroCPPWrappers/src/Bitmap/Bitmap.h"
#include "../Dependencies/AllegroCPPWrappers/src/Display/Display.h"
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
		thread *managingThread;
		bool shouldStop = false;
		bool newFrame = false;
		bool resized = false;
		bool fullScreen = false;
		bool frameless = false;
		bool maximized = false;
		int width, height;
		mutex frameLock;
		Bitmap currentFrame;
		void manageDisplay(int width, int height, int framerate);

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
	};
} // namespace Esperattato

#endif