#ifndef ESPERATTO_COLLISION
#define ESPERATTO_COLLISION

#include <map>
#include <string>
#include "../Node/Node.h"

namespace Esperatto
{
	class CollisionSquareNode;
	struct SquarePair
	{
		Coordinates first, second;
	};
	class CollisionSquare
	{
	protected:
		bool onX = true;
		SquarePair pair;

		/*
		    parent - If present, the coordinates defining this square will be
		   considered offsets from the parent's coordinates.
		*/

		CollisionSquareNode *squareParent = nullptr;
		/*
		    subSquares - If a collision is detected within this square's
		   coordinates, check each of these squares for a collision. If any of
		   the squares in this list collide with the passed square, returns
		   true. Else, it returns false. If the list is empty, true is
		   returned.
		*/
		vector<CollisionSquare> subSquares = vector<CollisionSquare>();

		SquarePair getEncompassingSquare(bool includeSubSquares = true) const;

		void drawToBitmap(Bitmap bitmap, Transform& inverseTransform, unsigned int level = 1);

	public:
		CollisionSquare(const Coordinates &a, const Coordinates &b, bool onX);
		bool Intersects(const CollisionSquare &otherSquare, bool checkFlip = true) const;
		void declareParent(CollisionSquareNode *parent);
		void setFirstCoords(const Coordinates &coords);
		void setSecondCoords(const Coordinates &coords);
		void addSubSquare(CollisionSquare sub);
		string toString();
		friend class CollisionSquareNode;
	};

	class CollisionSquareNode : public Node, public CollisionSquare
	{
	private:
		Transform bitmapTransform;
		Bitmap collisionBitmap = nullptr;
		bool drawBoxes = false;
	public:
		CollisionSquareNode(const Coordinates &a, const Coordinates &b, bool onX) : CollisionSquare(a, b, onX)
		{
			this->declareParent(this);
			al_identity_transform(&this->bitmapTransform);
		};
		~CollisionSquareNode()
		{
			this->declareParent(nullptr);
			if(this->collisionBitmap != nullptr){
				al_destroy_bitmap(this->collisionBitmap);
			}
		}
		Transform getTransform();

		Bitmap getBitmap();

		void toggleDrawBoxes(bool shouldDraw);

		friend class CollisionSquare;
	};
} // namespace Esperatto

#endif