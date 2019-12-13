#ifndef ALLEGRO_WRAPPERS_USER_EVENT_SOURCE_DEF
#define ALLEGRO_WRAPPERS_USER_EVENT_SOURCE_DEF

#include <allegro5/allegro.h>
#include "EventSource.cpp"
#include "Event.cpp"

namespace AllegroWrappers {
	class UserEventSource : public EventSource {
		public:
			UserEventSource(): EventSource(){
				al_init_user_event_source(data->event_source);
			}

			~UserEventSource(){
				al_destroy_user_event_source(data->event_source);
			}

			bool emit_user_event(Event event){
				return al_emit_user_event(data->event_source, event.data->event, nullptr);
			}

	};
}

#endif