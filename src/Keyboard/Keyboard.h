#ifndef ESPERATTATO_KEYBOARD_DEF
#define ESPERATTATO_KEYBOARD_DEF

#include "../Types.h"
#include <allegro5/allegro5.h>
#include <functional>
#include <map>
#include <thread>
#include <vector>

using namespace std;
namespace Esperatto {
	class Keyboard {
	  public:
		enum KEY_EVENTS {
			KEY_DOWN = ALLEGRO_EVENT_KEY_DOWN,
			KEY_UP = ALLEGRO_EVENT_KEY_UP
		};

	  private:
		static ALLEGRO_EVENT_QUEUE *keyboardQueue;
		static thread *eventManager;
		static unsigned int referenceCount;
		static bool shouldEnd;

		typedef vector<function<void(KEY_EVENTS e, unsigned int keycode,
		                             unsigned int keymodFlags)>>
		    callbackList;

		typedef map<Keyboard::KEY_EVENTS, callbackList> eventList;

		typedef map<unsigned int, eventList *> flagList;

		typedef map<unsigned int, flagList *> keycodeList;

		static keycodeList callbacks;

		static void cycleEvents();

	  public:
		static void subscribe(KEY_EVENTS e, unsigned int keycode,
		                      unsigned int keymodFlags,
		                      function<void(KEY_EVENTS e, unsigned int keycode,
		                                    unsigned int keymodFlags)>
		                          callback);
	};
} // namespace Esperatto

#endif