#ifndef ESPERATTATO_DISPLAY_DEF
#define ESPERATTATO_DISPLAY_DEF
#include "../Dependencies/AllegroCPPWrappers/src/Display/Display.h"
// #include <Display.h>
#include <thread>
using namespace std;

namespace Esperattato {
	class Display {
	  private:
		thread *managingThread;
		bool shouldStop = false;
		void manageDisplay(int width, int height, int framerate);

	  protected:
	  public:
		Display(int width, int height, int framerate);
		~Display();
	};
} // namespace Esperattato

#endif