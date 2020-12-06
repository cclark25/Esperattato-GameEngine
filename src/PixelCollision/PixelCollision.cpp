#include "PixelCollision.h"

namespace Esperatto
{
	PixelCollision::PixelCollision(string filePath)
	{
		try
		{
			data = collisionMap.at(filePath);
			data->referenceCount++;
		}
		catch (out_of_range e)
		{
			data = shared_ptr<collisionData>(new collisionData(filePath));
			data->image = Image(filePath);
			data->referenceCount = 1;
		}
	}

	PixelCollision::~PixelCollision()
	{
		data->referenceCount--;
		if (data->referenceCount == 0)
		{
			collisionMap.erase(data->path);
		}
	}
} // namespace Esperatto