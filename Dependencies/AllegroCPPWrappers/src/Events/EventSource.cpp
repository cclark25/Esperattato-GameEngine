#ifndef ALLEGRO_WRAPPERS_EVENT_SOURCE_DEF
#define ALLEGRO_WRAPPERS_EVENT_SOURCE_DEF

#include <allegro5/allegro.h>

namespace AllegroWrappers {
    class EventSource {
        public:
            struct foreign_data {
				ALLEGRO_EVENT_SOURCE* event_source;
				unsigned int reference_count;
			} *data = nullptr;

            EventSource(){
                data = new foreign_data();
                data->event_source = new ALLEGRO_EVENT_SOURCE();
                data->reference_count = 1;
            }

            EventSource(ALLEGRO_EVENT_SOURCE* source){
                data = new foreign_data();
                data->event_source = source;
                data->reference_count = 1;
            }

            EventSource(EventSource const&source){
                data = source.data;
                data->reference_count++;
            }

            ~EventSource(){
                data->reference_count--;
                if(data->reference_count == 0){
                    delete data->event_source;
                    delete data;
                }
            }
    };
}

#endif