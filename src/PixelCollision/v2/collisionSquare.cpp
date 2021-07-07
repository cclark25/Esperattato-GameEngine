#include "collisionSquare.h"
#include "../../Node/Node.h"
#include "../../Node/NodeSubTypes.h"

namespace Esperatto {
	bool overlaps(double a1, double a2, double b1, double b2);

	CollisionSquare::CollisionSquare(const Coordinates &a, const Coordinates &b,
	                                 bool onX) {

		this->onX = onX;

		this->first = a;
		this->second = b;
	}

	bool CollisionSquare::Intersects(const CollisionSquare &otherSquare) const {
		double x1, x2, x3, x4;
		double y1, y2, y3, y4;

		x1 = this->first.x;
		x2 = this->second.x;
		x3 = otherSquare.first.x;
		x4 = otherSquare.second.x;
		y1 = this->first.y;
		y2 = this->second.y;
		y3 = otherSquare.first.y;
		y4 = otherSquare.second.y;

		if (this->parent != nullptr) {
			const Coordinates parentTransform =
			    this->parent->getGlobalPosition();
			x1 += parentTransform.x;
			x2 += parentTransform.x;
			y1 += parentTransform.y;
			y2 += parentTransform.y;
		}
		if (otherSquare.parent != nullptr) {
			const Coordinates parentTransform =
			    otherSquare.parent->getGlobalPosition();
			x3 += parentTransform.x;
			x4 += parentTransform.x;
			y3 += parentTransform.y;
			y4 += parentTransform.y;
		}

		/*
		        std::cout << "Root First Coords: " + Coordinates({x1,
		   y1}).toString()
		                  << std::endl;

		        std::cout << "Root Second Coords: " + Coordinates({x2,
		   y2}).toString()
		                  << std::endl;

		        std::cout << "Other First Coords: " + Coordinates({x3,
		   y3}).toString()
		                  << std::endl;

		        std::cout << "Other Second Coords: " + Coordinates({x4,
		   y4}).toString()
		                  << std::endl;
		*/

		bool xOverlap = overlaps(x1, x2, x3, x4);
		bool yOverlap = overlaps(y1, y2, y3, y4);

		bool overlap = xOverlap && yOverlap;

		if (!overlap || this->subSquares.size() == 0) {
			return overlap;
		} else {
			std::cout << "subSquares has records. Testing them." << std::endl;
		}

		for (const CollisionSquare &subSquare : this->subSquares) {
			if (otherSquare.Intersects(subSquare)) {
				std::cout << "Matched a sub square. Collision detected."
				          << std::endl;

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

	void CollisionSquare::addSubSquare(CollisionSquare sub) {
		this->subSquares.push_back(sub);
		this->subSquares.back().declareParent(this->parent);
	}

	string CollisionSquare::toString() {
		return "(" + this->first.toString() + "," + this->second.toString() +
		       ")";
	}

} // namespace Esperatto