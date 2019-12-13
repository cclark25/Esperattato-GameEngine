#ifndef ALLEGRO_WRAPPERS_EVENT_QUEUE_DEF
#define ALLEGRO_WRAPPERS_EVENT_QUEUE_DEF

#include <allegro5/allegro.h>
#include "EventSource.cpp"
#include "Event.cpp"
#include <map>

namespace AllegroWrappers {
    class EventQueue {
        public:
            struct foreign_data {
				ALLEGRO_EVENT_QUEUE* event_queue;
				unsigned int reference_count;
			} *data = nullptr;

            std::map<ALLEGRO_EVENT_SOURCE *, EventSource> registered_event_sources;

            EventQueue(){
                data = new foreign_data {
                    al_create_event_queue(),
                    1
                };
            }

            EventQueue(EventQueue &source){
                data = source.data;
                data->reference_count++;
            }

            void register_event_source(EventSource event_source){
                al_register_event_source(data->event_queue, event_source.data->event_source);
                registered_event_sources[event_source.data->event_source] = event_source;
            }

            void unregister_event_source(EventSource event_source){
                al_unregister_event_source(data->event_queue, event_source.data->event_source);
                registered_event_sources.erase(event_source.data->event_source);
            }

            bool is_event_source_registered(EventSource source){
                return registered_event_sources.count(source.data->event_source) > 0;
            }

            void toggle_pause_event_queue(bool paused){
                al_pause_event_queue(data->event_queue, paused);
            }

            bool is_event_queue_paused(){
                return al_is_event_queue_paused(data->event_queue);
            }

            bool is_event_queue_empty(){
                return al_is_event_queue_empty(data->event_queue);
            }

            Event await_next_event(float timeout = -1){
                ALLEGRO_EVENT *result = new ALLEGRO_EVENT();
                if(timeout <= 0)
                    al_wait_for_event(data->event_queue, result);
                else {
                    if(!al_wait_for_event_timed(data->event_queue, result, timeout)){
                        delete result;
                        throw false;
                    }
                }
                return Event(result);
            }

            ~EventQueue(){
                data->reference_count--;
                if(data->reference_count == 0){
                    al_destroy_event_queue(data->event_queue);
                    delete data;
                }
            }
    };
}

#endif