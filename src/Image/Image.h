#ifndef ESPERATTO_IMAGE
#define ESPERATTO_IMAGE
#include "../Types.h"
#include "../Node/NodeSubTypes.h"
#include <iostream>
#include <map>
#include <string>
#include <memory>
using namespace std;

namespace Esperatto {
	struct imageData {
		Bitmap internal;
		string path;
		unsigned int referenceCount = 0;
	};
	static map<string, shared_ptr<imageData> > loadedMap;
	class Image : public Subdata {
	  protected:
		shared_ptr<imageData> data;

	  public:
		Image(string path){
			try {
				data = loadedMap.at(path);
				data->referenceCount++;
			} catch (out_of_range e) {
				data = shared_ptr<imageData>(new imageData());
				data->internal = al_load_bitmap(path.c_str());
				data->referenceCount = 1;
				/*
					For some reason, locking the bitmap makes it slower.
				*/
				// al_lock_bitmap(data->internal, ALLEGRO_PIXEL_FORMAT_ANY,
				//                ALLEGRO_LOCK_READONLY);
				loadedMap.insert_or_assign(path, data);
			}
		}
		Bitmap getBitmap() { return data->internal; }
		~Image() {
			data->referenceCount--;
			if (data->referenceCount == 0) {
				al_destroy_bitmap(data->internal);
				loadedMap.erase(data->path);
				// delete data;
			}
		}
	};
} // namespace Esperatto

#endif