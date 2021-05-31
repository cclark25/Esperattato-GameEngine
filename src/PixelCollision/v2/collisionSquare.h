#ifndef ESPERATTO_COLLISION
#define ESPERATTO_COLLISION

#include <string>
#include <map>
// #include "../../Image/Image.h"
#include "../../Node/Node.h"

namespace Esperatto
{
	class CollisionSquare : public Subdata
	{
	private:
		bool onX = true;
		Coordinates second;
	public:
		CollisionSquare(const Coordinates &b, bool onX);
		bool Intersects(const CollisionSquare &otherSquare);
		void declareParent(const NodeSubtype &parent){
			std::cout << "declareParent called" << std::endl;
		}
	};
} // namespace Esperatto

#endif