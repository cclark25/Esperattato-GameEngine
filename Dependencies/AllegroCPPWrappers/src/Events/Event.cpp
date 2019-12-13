#ifndef ALLEGRO_WRAPPERS_EVENT_DEF
#define ALLEGRO_WRAPPERS_EVENT_DEF

#include <allegro5/allegro.h>

namespace AllegroWrappers {
    class Event {
        public:
            struct foreign_data {
				ALLEGRO_EVENT* event;
				unsigned int reference_count;
			} *data = nullptr;

            Event(ALLEGRO_EVENT* source){
                data = new foreign_data();
                data->event = source;
                data->reference_count = 1;
            }

            ALLEGRO_EVENT_TYPE get_type(){
                return data->event->type;
            }

            ALLEGRO_EVENT_SOURCE* get_source(){
                return data->event->any.source;
            }

            double get_timestamp(){
                return data->event->any.timestamp;
            }

            ~Event(){
                data->reference_count--;
                if(data->reference_count == 0){
                    delete data->event;
                    delete data;
                }
            }
    };
}

#endif