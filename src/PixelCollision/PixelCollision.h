#ifndef ESPERATTO_PIXEL_COLLISION
#define ESPERATTO_PIXEL_COLLISION

#include <string>
#include <map>
#include "../Image/Image.h"
#include "../Node/Node.h"

using namespace std;

namespace Esperatto
{
	
	struct collisionData
	{
		Image image;
		string path;
		unsigned int referenceCount = 0;

		collisionData(string filePath) : image(filePath)
		{
			path = filePath;
		}
	};

	static map<string, shared_ptr<collisionData>> collisionMap;

	class PixelCollision
	{
	protected:
		shared_ptr<collisionData> data;

	public:
		PixelCollision(string filePath);
		~PixelCollision();
	};
} // namespace Esperatto

#endif