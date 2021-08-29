#include "collisionSquare.h"
#include "../Node/Node.h"
#include <allegro5/allegro_primitives.h>
#include <cmath>
namespace Esperatto
{
	bool overlaps(double a1, double a2, double b1, double b2);

	CollisionSquare::CollisionSquare(const Coordinates &a, const Coordinates &b,
									 bool onX)
	{
		this->onX = onX;
		if (a.x <= b.x)
		{
			this->pair.first.x = a.x;
			this->pair.second.x = b.x;
		}
		else
		{
			this->pair.first.x = b.x;
			this->pair.second.x = a.x;
		}

		if (a.y > b.y)
		{
			this->pair.first.y = a.y;
			this->pair.second.y = b.y;
		}
		else
		{
			this->pair.first.y = b.y;
			this->pair.second.y = a.y;
		}
	}

	bool CollisionSquare::Intersects(const CollisionSquare &otherSquare, bool checkFlip) const
	{
		SquarePair thisSquarePair = this->getEncompassingSquare(false);
		SquarePair otherSquarePair = otherSquare.getEncompassingSquare(false);

		if (this->squareParent)
		{
			const Coordinates parentTransform =
				this->squareParent->getGlobalPosition();
			thisSquarePair.first.x += parentTransform.x;
			thisSquarePair.second.x += parentTransform.x;
			thisSquarePair.first.y += parentTransform.y;
			thisSquarePair.second.y += parentTransform.y;
		}
		if (otherSquare.squareParent != nullptr)
		{
			const Coordinates parentTransform =
				otherSquare.squareParent->getGlobalPosition();
			otherSquarePair.first.x += parentTransform.x;
			otherSquarePair.second.x += parentTransform.x;
			otherSquarePair.first.y += parentTransform.y;
			otherSquarePair.second.y += parentTransform.y;
		}

		//         std::cout << "Root First Coords: " + Coordinates({x1,
		//    y1}).toString()
		//                   << std::endl;

		//         std::cout << "Root Second Coords: " + Coordinates({x2,
		//    y2}).toString()
		//                   << std::endl;

		//         std::cout << "Other First Coords: " + Coordinates({x3,
		//    y3}).toString()
		//                   << std::endl;

		//         std::cout << "Other Second Coords: " + Coordinates({x4,
		//    y4}).toString()
		//                   << std::endl;

		bool xOverlap = overlaps(
			thisSquarePair.first.x,
			thisSquarePair.second.x,
			otherSquarePair.first.x,
			otherSquarePair.second.x);
		bool yOverlap = overlaps(
			thisSquarePair.second.y,
			thisSquarePair.first.y,
			otherSquarePair.second.y,
			otherSquarePair.first.y);

		bool overlap = xOverlap && yOverlap;

		if (!overlap || (this->subSquares.size() == 0 && otherSquare.subSquares.size() == 0))
		{
			return overlap;
		}
		else
		{
			// std::cout << "subSquares has records. Testing them." << std::endl;
		}

		for (const CollisionSquare &subSquare : this->subSquares)
		{
			if (subSquare.Intersects(otherSquare))
			{
				return true;
			}
		}

		if (checkFlip)
		{
			return otherSquare.Intersects(*this, false);
		}

		return false;
	}

	void CollisionSquare::declareParent(CollisionSquareNode *parent)
	{
		this->squareParent = parent;
		for (CollisionSquare &sub : this->subSquares)
		{
			sub.declareParent(parent);
		}
	}

	bool overlaps(double a1, double a2, double b1, double b2)
	{
		if (a1 <= b1)
		{
			if (a2 >= b1)
			{
				return true;
			}
		}
		else if (a1 <= b2)
		{
			return true;
		}

		return false;
	}

	void CollisionSquare::setFirstCoords(const Coordinates &coords)
	{
		this->pair.first.x = coords.x;
		this->pair.first.y = coords.y;
	}

	void CollisionSquare::setSecondCoords(const Coordinates &coords)
	{
		this->pair.second.x = coords.x;
		this->pair.second.y = coords.y;
	}

	void CollisionSquare::addSubSquare(CollisionSquare sub)
	{
		this->subSquares.push_back(sub);
		this->subSquares.back().declareParent(this->squareParent);
	}

	string CollisionSquare::toString()
	{
		return "(" + this->pair.first.toString() + "," + this->pair.second.toString() +
			   ")";
	}

	SquarePair CollisionSquare::getEncompassingSquare(bool includeSubSquares) const
	{
		double leftmost, rightmost, topmost, bottommost;
		leftmost = this->pair.first.x;
		topmost = this->pair.first.y;
		rightmost = this->pair.second.x;
		bottommost = this->pair.second.y;
		if (includeSubSquares)
		{
			for (const CollisionSquare &child : this->subSquares)
			{
				SquarePair childBounds = child.getEncompassingSquare();

				if (childBounds.first.x < leftmost)
				{
					leftmost = childBounds.first.x;
				}
				if (childBounds.first.y > topmost)
				{
					topmost = childBounds.first.y;
				}
				if (childBounds.second.x > rightmost)
				{
					rightmost = childBounds.second.x;
				}
				if (childBounds.second.y < bottommost)
				{
					bottommost = childBounds.second.y;
				}
			}
		}
		Coordinates first, second;
		first.x = leftmost;
		first.y = topmost;
		second.x = rightmost;
		second.y = bottommost;
		return {first, second};
	}

	void CollisionSquare::drawToBitmap(Bitmap bitmap, Transform &inverseTransform, unsigned int level)
	{
		Bitmap previous = al_get_target_bitmap();
		char curveBase = 4;
		unsigned char levelAdjust = 256 * ((pow(curveBase, level) - 1) / pow(curveBase, level)) - 1;

		al_set_target_bitmap(bitmap);
		al_use_transform(&inverseTransform);
		// Transform t;
		// al_identity_transform(&t);
		// al_use_transform(&t);
		al_draw_filled_rectangle(
			this->pair.first.x, -this->pair.first.y, this->pair.second.x, -this->pair.second.y,
			al_map_rgba(((level - 1) % 3 == 0) ? levelAdjust : 0, ((level - 1) % 3 == 1) ? levelAdjust : 0, ((level - 1) % 3 == 2) ? levelAdjust : 0, 1));
		// al_draw_filled_rectangle(
		// 	0,0,10,10,
		// 	al_map_rgba(0,0,255, 255));

		for (CollisionSquare &child : this->subSquares)
		{
			child.drawToBitmap(bitmap, inverseTransform, level + 1);
		}

		al_set_target_bitmap(previous);
	}

	Transform CollisionSquareNode::getTransform()
	{
		return this->bitmapTransform;
	}

	Bitmap CollisionSquareNode::getBitmap()
	{
		if(!this->drawBoxes){
			return nullptr;
		}
		
		SquarePair enclosingBox = this->getEncompassingSquare();
		al_identity_transform(&this->bitmapTransform);
		al_translate_transform(&this->bitmapTransform, enclosingBox.first.x, enclosingBox.first.y);
		if (this->collisionBitmap == nullptr)
		{
			this->collisionBitmap = al_create_bitmap(abs(enclosingBox.second.x - enclosingBox.first.x), abs(enclosingBox.second.y - enclosingBox.first.y));
		}
		Bitmap previous = al_get_target_bitmap();
		al_set_target_bitmap(this->collisionBitmap);
		al_clear_to_color(al_map_rgba(0, 0, 0, 100));

		Transform inverseTransform;
		al_identity_transform(&inverseTransform);
		al_translate_transform(&inverseTransform, enclosingBox.first.x, -enclosingBox.first.y);
		al_invert_transform(&inverseTransform);
		this->drawToBitmap(this->collisionBitmap, inverseTransform);

		al_set_target_bitmap(previous);

		return this->collisionBitmap;
	}

	void CollisionSquareNode::toggleDrawBoxes(bool shouldDraw)
	{
		this->drawBoxes = shouldDraw;
	}

} // namespace Esperatto