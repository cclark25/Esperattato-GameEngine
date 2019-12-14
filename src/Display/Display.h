#ifndef ESPERATTATO_DISPLAY_DEF
#define ESPERATTATO_DISPLAY_DEF
#include "../Dependencies/AllegroCPPWrappers/src/Bitmap/Bitmap.h"
#include "../Dependencies/AllegroCPPWrappers/src/Display/Display.h"
#include <mutex>
#include <thread>

using namespace std;
using namespace AllegroWrappers;

namespace Esperattato {
	class Display {
	  private:
		thread *managingThread;
		bool shouldStop = false;
		bool newFrame = false;
		mutex frameLock;
		Bitmap currentFrame;
		void manageDisplay(int width, int height, int framerate);

	  protected:
	  public:
		Display(Display &source);
		Display(int width, int height, int framerate);
		~Display();
		void pushFrame(Bitmap frame);
	};
} // namespace Esperattato

#endif