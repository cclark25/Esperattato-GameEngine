#include "Keyboard.h"
#include <iostream>
using namespace std;

int print(string msg) {
	cout << msg << endl;
	return 0;
}

namespace Esperatto {
	ALLEGRO_EVENT_QUEUE *Keyboard::keyboardQueue = al_create_event_queue();
	thread *Keyboard::eventManager = new thread(Keyboard::cycleEvents);

	unsigned int Keyboard::referenceCount = 0;
	bool Keyboard::shouldEnd;
	Keyboard::keycodeList Keyboard::callbacks;

	void Keyboard::cycleEvents() {
		bool keyboardInstalled = al_install_keyboard();
		if(!keyboardInstalled){
			cout << "Keyboard failed to install." << endl;
		}
		al_register_event_source(Keyboard::keyboardQueue,
		                         al_get_keyboard_event_source());
		ALLEGRO_EVENT currentEvent;
		while (!Keyboard::shouldEnd) {
			// cout << "cycleEvents called.\n";
			al_wait_for_event(Keyboard::keyboardQueue, &currentEvent);
			// cout << "Event occurred.\n";
			if (Keyboard::callbacks.find(currentEvent.keyboard.keycode) !=
			    Keyboard::callbacks.end()) {
				auto i = Keyboard::callbacks.at(currentEvent.keyboard.keycode);
				if (i->find(currentEvent.keyboard.modifiers) != i->end()) {
					auto j = i->at(currentEvent.keyboard.modifiers);
					if (j->find((KEY_EVENTS)currentEvent.type) != j->end()) {
						auto k = j->at((KEY_EVENTS)currentEvent.type);
						for (auto callback : k) {
							callback(
							    (Keyboard::KEY_EVENTS)currentEvent.type,
							    (unsigned int)currentEvent.keyboard.keycode,
							    (unsigned int)currentEvent.keyboard.modifiers);
						}
					}
				}
			}
		}
	}

	void Keyboard::subscribe(KEY_EVENTS e, unsigned int keycode,
	                         unsigned int keymodFlags,
	                         function<void(KEY_EVENTS e, unsigned int keycode,
	                                       unsigned int keymodFlags)>
	                             callback) {
		auto keycodeIt = Keyboard::callbacks.find(keycode);
		if (keycodeIt == Keyboard::callbacks.end()) {
			auto createResult =
			    Keyboard::callbacks.emplace(keycode, new flagList());
			if (!createResult.second) {
				throw "Couldn't emplace the keycode mapping. This should not "
				      "have happened. If this error occurs, please consider "
				      "updating this exception.";
			}
			keycodeIt = createResult.first;
		}

		auto flagIt = (*keycodeIt).second->find(keymodFlags);
		if (flagIt == (*keycodeIt).second->end()) {
			auto createResult =
			    (*keycodeIt).second->emplace(keymodFlags, new eventList());
			if (!createResult.second) {
				throw "Couldn't emplace the keymod mapping. This should not "
				      "have happened. If this error occurs, please consider "
				      "updating this exception.";
			}
			flagIt = createResult.first;
		}

		auto eventIt = (*flagIt).second->find(e);
		if (eventIt == (*flagIt).second->end()) {
			auto createResult = (*flagIt).second->emplace(e, callbackList());
			if (!createResult.second) {
				throw "Couldn't emplace the event mapping. This should not "
				      "have happened. If this error occurs, please consider "
				      "updating this exception.";
			}
			eventIt = createResult.first;
		}

		(*eventIt).second.push_back(callback);
	}

} // namespace Esperatto