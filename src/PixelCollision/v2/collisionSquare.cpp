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
		/*
			TODO: Replace the cout test code with the commented out code below
				and write up a test in the collisionTest.cpp file to test 
				collision functionality.
		*/

		// bool xOverlap = overlaps(this->data->xPosition, this->second.x,
		// otherSquare.data->xPosition, otherSquare.data->xPosition); bool
		// yOverlap = overlaps(this->data->yPosition, this->second.y,
		// otherSquare.data->yPosition, otherSquare.data->yPosition);

		// return xOverlap && yOverlap;
		Coordinates rootCoords = this->parent->getGlobalPosition();
		std::cout << "Intersects called: X=" << rootCoords.x
		          << "  Y=" << rootCoords.y << std::endl;
		return true;
	}

	void CollisionSquare::declareParent(Node *parent) {
		this->parent = parent;
	}

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
} // namespace Esperatto