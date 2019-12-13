#ifndef ALLEGRO_WRAPPERS_TIMER_DEF
#define ALLEGRO_WRAPPERS_TIMER_DEF

#include <allegro5/allegro.h>
#include "../Events/EventSource.cpp"

namespace AllegroWrappers {
	class Timer {
		public:
			struct foreign_data {
				ALLEGRO_TIMER * timer;
				EventSource event_source;
				unsigned int reference_count;
			} *data = nullptr;

			Timer(double speed_seconds){
				ALLEGRO_TIMER *t = al_create_timer(speed_seconds);
				EventSource s(al_get_timer_event_source(t));
				data = new foreign_data {
					t, 
					s, 
					1
				};
			}

			Timer(Timer &source){
				data = source.data;
				data->reference_count++;
			}

			void start_timer(){
				al_start_timer(data->timer);
			}

			void stop_timer(){
				al_stop_timer(data->timer);
			}

			void resume_timer(){
				al_resume_timer(data->timer);
			}

			bool get_timer_started(){
				return al_get_timer_started(data->timer);
			}

			int64_t get_timer_count(){
				return al_get_timer_count(data->timer);
			}

			void set_timer_count(int64_t new_count){
				return al_set_timer_count(data->timer, new_count);
			}

			void add_timer_count(int64_t difference){
				return al_add_timer_count(data->timer, difference);
			}

			double get_timer_speed(){
				return al_get_timer_speed(data->timer);
			}

			void set_timer_speed(double new_speed_seconds){
				return al_set_timer_speed(data->timer, new_speed_seconds);
			}

			EventSource get_timer_event_source(){
				return data->event_source;
			}

			~Timer(){
				data->reference_count--;
				if(data->reference_count == 0){
					al_destroy_timer(data->timer);
					delete data;
				}
			}

	};
}

#endif