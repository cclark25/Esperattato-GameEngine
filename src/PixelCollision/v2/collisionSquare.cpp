#include "collisionSquare.h"
#include "../../Node/Node.h"
#include "../../Node/NodeSubTypes.h"

namespace Esperatto {
	bool overlaps(double a1, double a2, double b1, double b2);

	CollisionSquare::CollisionSquare(const Coordinates &a, const Coordinates &b,
	                                 bool onX) {

		this->onX = onX;

		this->second = a;
		this->second = b;
	}

	bool CollisionSquare::Intersects(const CollisionSquare &otherSquare) const {

		Coordinates rootCoords = this->parent != nullptr
		                             ? this->parent->getGlobalPosition()
		                             : Coordinates({0, 0});
		Coordinates otherCoords = otherSquare.parent != nullptr
		                              ? otherSquare.parent->getGlobalPosition()
		                              : Coordinates({0, 0});

		// std::cout << "Root Coords: (" << rootCoords.x << "," << rootCoords.y
		//           << ")" << std::endl;
		std::cout << "Root Second Coords Adjusted: ("
		          << rootCoords.x + this->second.x << ","
		          << rootCoords.y + this->second.y << ")" << std::endl;
		// std::cout << "Other Coords: (" << otherCoords.x << "," << otherCoords.y
		//           << ")" << std::endl;
		std::cout << "Other Second Coords Adjusted: ("
		          << otherCoords.x + otherSquare.second.x << ","
		          << otherCoords.y + otherSquare.second.y << ")" << std::endl;

		bool xOverlap =
		    overlaps(rootCoords.x, rootCoords.x + this->second.x, otherCoords.x,
		             otherCoords.x + otherSquare.second.x);
		bool yOverlap =
		    overlaps(rootCoords.y, rootCoords.y + this->second.y, otherCoords.y,
		             otherCoords.y + otherSquare.second.y);

		bool overlap = xOverlap && yOverlap;

		if(!overlap || this->subSquares.size() == 0){
			return overlap;
		}
		else {
			std::cout << "subSquares has records. Testing them." << std::endl;
		}

		for (const CollisionSquare &subSquare : this->subSquares) {
			if(otherSquare.Intersects(subSquare)){
				std::cout << "Matched a sub square. Collision detected." << std::endl;
				return true;
			}
		}

		return false;
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

	void CollisionSquare::setFirstCoords(const Coordinates &coords) {
		this->first.x = coords.x;
		this->first.y = coords.y;
	}

	void CollisionSquare::setSecondCoords(const Coordinates &coords) {
		this->second.x = coords.x;
		this->second.y = coords.y;
	}

	void CollisionSquare::addSubSquare(CollisionSquare sub){
		this->subSquares.push_back(sub);
		this->subSquares.back().declareParent(this->parent);
	}
} // namespace Esperatto