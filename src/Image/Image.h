#ifndef ESPERATTO_IMAGE
#define ESPERATTO_IMAGE
#include "../Types.h"
#include "../Node/Node.h"
#include <iostream>
#include <map>
#include <string>
#include <memory>
using namespace std;

namespace Esperatto
{
	struct imageData
	{
		Bitmap internal;
		string path;
		unsigned int referenceCount = 0;
	};
	static map<string, shared_ptr<imageData>> loadedMap;
	class Image : public Node
	{
	protected:
		shared_ptr<imageData> data;

	public:
		Image(string path);
		Image(unsigned int width, unsigned int height, Color fillColor = al_map_rgb(255, 255, 255));
		Bitmap getBitmap();
		~Image();
	};
} // namespace Esperatto

#endif