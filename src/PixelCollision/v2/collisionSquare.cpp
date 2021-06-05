#include "collisionSquare.h"
#include "../../Node/Node.h"
#include "../../Node/NodeSubTypes.h"

namespace Esperatto {
	bool overlaps(double a1, double a2, double b1, double b2);

	CollisionSquare::CollisionSquare(const Coordinates &b, bool onX) {

		this->onX = onX;

		this->second = b;
	}

	bool CollisionSquare::Intersects(const CollisionSquare &otherSquare) {
	
		if (this->parent == nullptr, otherSquare.parent == nullptr) {
			throw std::logic_error("CollisionSquare::Intersects is called "
			                       "while parent node is null!");
		}

		Coordinates rootCoords = this->parent->getGlobalPosition();
		Coordinates otherCoords = otherSquare.parent->getGlobalPosition();

		std::cout << "Root Coords: (" << rootCoords.x << "," << rootCoords.y
		          << ")" << std::endl;
		std::cout << "Root Second Coords Adjusted: ("
		          << rootCoords.x + this->second.x << ","
		          << rootCoords.y + this->second.y << ")" << std::endl;
		std::cout << "Other Coords: (" << otherCoords.x << "," << otherCoords.y
		          << ")" << std::endl;
		std::cout << "Other Second Coords Adjusted: ("
		          << otherCoords.x + otherSquare.second.x << ","
		          << otherCoords.y + otherSquare.second.y << ")" << std::endl;

		bool xOverlap =
		    overlaps(rootCoords.x, rootCoords.x + this->second.x, otherCoords.x,
		             otherCoords.x + otherSquare.second.x);
		bool yOverlap =
		    overlaps(rootCoords.y, rootCoords.y + this->second.y, otherCoords.y,
		             otherCoords.y + otherSquare.second.y);

		return xOverlap && yOverlap;
	}

	void CollisionSquare::declareParent(Node *parent) { this->parent = parent; }

	bool overlaps(double a1, double a2, double b1, double b2) {
		if (a1 <= b1) {
			if (a2 >= b1) {
				return true;
			}
		} else if (a1 <= b2) {
			return true;
		}

		return false;
	}

	void CollisionSquare::setSecondCoords(const Coordinates &coords) {
		this->second.x = coords.x;
		this->second.y = coords.y;
	}
} // namespace Esperatto