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
	public:
		CollisionSquare(const Coordinates &b, bool onX);
		bool Intersects(const CollisionSquare &otherSquare);
		/*
			TODO: This declareParent function gets called successfully. Now we need to modify this class to
			store this pointer and reference the parent Node object to test basic collisions.
		*/
		void declareParent(const Node *parent){
			std::cout << "declareParent called." << std::endl;
		}
	};
} // namespace Esperatto

#endif