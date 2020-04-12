#include "Keyboard.h"
#include <iostream>
using namespace std;

namespace Esperatto
{
	Keyboard::Keyboard(){
		data = shared_ptr<foreign_data>(new foreign_data());
	}
	Keyboard::Keyboard(const Keyboard& original){
		data = original.data;
	}

	Keyboard::~Keyboard(){
		if(data.use_count() == 1){
			data->shouldEnd = true;
		}
	}

void Keyboard::cycleEvents()
{
	while (data->keyboardQueue == nullptr || data->keyboardQueue == NULL)
	{
		data->keyboardQueue = al_create_event_queue();
	}

	bool keyboardInstalled = al_install_keyboard();
	if (!keyboardInstalled)
	{
		cout << "Keyboard failed to install." << endl;
	}
	al_register_event_source(data->keyboardQueue,
							 al_get_keyboard_event_source());
	ALLEGRO_EVENT currentEvent;
	while (!data->shouldEnd)
	{
		// cout << "cycleEvents called.\n";
		bool eventOccurred = al_wait_for_event_timed(data->keyboardQueue, &currentEvent, 5);
		if(!eventOccurred){
			continue;
		}
		// cout << "Event occurred.\n";
		if (data->callbacks.find(currentEvent.keyboard.keycode) !=
			data->callbacks.end())
		{
			auto i = data->callbacks.at(currentEvent.keyboard.keycode);
			if (i->find(currentEvent.keyboard.modifiers) != i->end())
			{
				auto j = i->at(currentEvent.keyboard.modifiers);
				if (j->find((KEY_EVENTS)currentEvent.type) != j->end())
				{
					auto k = j->at((KEY_EVENTS)currentEvent.type);
					for (auto callback : k)
					{
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
							 callback)
{
	auto keycodeIt = data->callbacks.find(keycode);
	if (keycodeIt == data->callbacks.end())
	{
		auto createResult =
			data->callbacks.emplace(keycode, new flagList());
		if (!createResult.second)
		{
			throw "Couldn't emplace the keycode mapping. This should not "
				  "have happened. If this error occurs, please consider "
				  "updating this exception.";
		}
		keycodeIt = createResult.first;
	}

	auto flagIt = (*keycodeIt).second->find(keymodFlags);
	if (flagIt == (*keycodeIt).second->end())
	{
		auto createResult =
			(*keycodeIt).second->emplace(keymodFlags, new eventList());
		if (!createResult.second)
		{
			throw "Couldn't emplace the keymod mapping. This should not "
				  "have happened. If this error occurs, please consider "
				  "updating this exception.";
		}
		flagIt = createResult.first;
	}

	auto eventIt = (*flagIt).second->find(e);
	if (eventIt == (*flagIt).second->end())
	{
		auto createResult = (*flagIt).second->emplace(e, callbackList());
		if (!createResult.second)
		{
			throw "Couldn't emplace the event mapping. This should not "
				  "have happened. If this error occurs, please consider "
				  "updating this exception.";
		}
		eventIt = createResult.first;
	}

	(*eventIt).second.push_back(callback);
}

} // namespace Esperatto