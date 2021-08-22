#include "../Types.h"
#include "../Node/Node.h"
#include "./Image.h"
#include <iostream>
#include <map>
#include <string>
#include <memory>
using namespace std;

namespace Esperatto
{
	Image::Image(string path)
	{
		try
		{
			data = loadedMap.at(path);
			data->referenceCount++;
		}
		catch (out_of_range &e)
		{
			data = shared_ptr<imageData>(new imageData());
			data->internal = al_load_bitmap(path.c_str());
			if (data->internal == nullptr)
			{
				throw "Could not load image file -- TODO: Standardize errors.";
			}

			data->referenceCount = 1;
			/*
					For some reason, locking the bitmap makes it slower.
				*/
			// al_lock_bitmap(data->internal, ALLEGRO_PIXEL_FORMAT_ANY,
			//                ALLEGRO_LOCK_READONLY);
			loadedMap.insert_or_assign(path, data);
		}
	}

	Bitmap Image::getBitmap() { return this->data->internal; }
	Image::~Image()
	{
		data->referenceCount--;
		if (data->referenceCount == 0)
		{
			al_destroy_bitmap(data->internal);
			loadedMap.erase(data->path);
			// delete data;
		}
	}

} // namespace Esperatto
