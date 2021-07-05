#ifndef ESPERATTO_COLLISION
#define ESPERATTO_COLLISION

#include <map>
#include <string>
// #include "../../Image/Image.h"
#include "../../Node/Node.h"
#include "../../Node/NodeSubTypes.h"

namespace Esperatto {
	class CollisionSquare : public Subdata {
	  private:
		bool onX = true;
		Coordinates first;
		Coordinates second;
		/*
		    parent - If present, the coordinates defining this square will be
		   considered offsets from the parent's coordinates.
		*/
		Node *parent = nullptr;
		/*
		    subSquares - If a collision is detected within this square's
		   coordinates, check each of these squares for a collision. If any of
		   the squares in this list collide with the passed square, returns
		   true. Else, it returns false. If the list is empty, true is
		   returned.
		*/
		vector<CollisionSquare> subSquares = vector<CollisionSquare>();

	  public:
		CollisionSquare(const Coordinates &a, const Coordinates &b, bool onX);
		bool Intersects(const CollisionSquare &otherSquare) const;
		void declareParent(Node *parent);
		void setFirstCoords(const Coordinates &coords);
		void setSecondCoords(const Coordinates &coords);
		void addSubSquare(CollisionSquare sub);
	};
} // namespace Esperatto

#endif