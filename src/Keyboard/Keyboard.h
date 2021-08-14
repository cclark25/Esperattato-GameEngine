#ifndef ESPERATTATO_KEYBOARD_DEF
#define ESPERATTATO_KEYBOARD_DEF

#include "../Types.h"
#include <allegro5/allegro5.h>
#include <functional>
#include <map>
#include <thread>
#include <vector>
#include <memory>

using namespace std;
namespace Esperatto
{
	enum KEY_EVENTS
	{
		KEY_DOWN = ALLEGRO_EVENT_KEY_DOWN,
		KEY_UP = ALLEGRO_EVENT_KEY_UP
	};

	struct KeySubscription
	{
		function<void(KEY_EVENTS e, unsigned int keycode,
					  unsigned int keymodFlags)>
			keyPressed;
		function<void(KEY_EVENTS e, unsigned int keycode,
					  unsigned int keymodFlags)>
			keyReleased;
	};

	class Keyboard
	{

	private:
		typedef vector<function<void(KEY_EVENTS e, unsigned int keycode,
									 unsigned int keymodFlags)>>
			callbackList;

		typedef map<KEY_EVENTS, callbackList> eventList;

		typedef map<unsigned int, eventList> flagList;

		typedef map<unsigned int, flagList> keycodeList;

		struct foreign_data
		{
			ALLEGRO_EVENT_QUEUE *keyboardQueue;
			thread *eventManager;
			unsigned int referenceCount;
			keycodeList callbacks;
			bool shouldEnd = false;
		};

		shared_ptr<foreign_data> data;

	public:
		Keyboard();
		Keyboard(const Keyboard &original);
		~Keyboard();

		void cycleEvents();

		void subscribe(KEY_EVENTS e, unsigned int keycode,
					   unsigned int keymodFlags,
					   function<void(KEY_EVENTS e, unsigned int keycode,
									 unsigned int keymodFlags)>
						   callback);
		void subscribeToggle(unsigned int keycode,
							 unsigned int keymodFlags, KeySubscription &subscription);
	};
} // namespace Esperatto

#endif