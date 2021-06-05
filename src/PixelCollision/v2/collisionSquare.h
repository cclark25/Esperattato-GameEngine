#ifndef ESPERATTO_COLLISION
#define ESPERATTO_COLLISION

#include <string>
#include <map>
// #include "../../Image/Image.h"
#include "../../Node/Node.h"
#include "../../Node/NodeSubTypes.h"

namespace Esperatto
{
	class CollisionSquare : public Subdata
	{
	private:
		bool onX = true;
		Coordinates second;
		Node *parent = nullptr;
	public:
		CollisionSquare(const Coordinates &b, bool onX);
		bool Intersects(const CollisionSquare &otherSquare);
		void declareParent(Node *parent);
		void setSecondCoords(const Coordinates& coords);
	};
} // namespace Esperatto

#endif