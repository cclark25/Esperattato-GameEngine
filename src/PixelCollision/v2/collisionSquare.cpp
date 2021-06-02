#include "collisionSquare.h"
#include "../../Node/Node.h"
#include "../../Node/NodeSubTypes.h"

namespace Esperatto {
	bool overlaps(double a1, double a2,double b1, double b2);
	
	CollisionSquare::CollisionSquare(const Coordinates &b,
	                             bool onX) {
		
		this->onX = onX;
		
		this->second = b;

	}

	bool CollisionSquare::Intersects(const CollisionSquare &otherSquare){
		// bool xOverlap = overlaps(this->data->xPosition, this->second.x, otherSquare.data->xPosition, otherSquare.data->xPosition);
		// bool yOverlap = overlaps(this->data->yPosition, this->second.y, otherSquare.data->yPosition, otherSquare.data->yPosition);
		
		// return xOverlap && yOverlap;
		return true;
	}

	bool overlaps(double a1, double a2,double b1, double b2){
		if(a1 <= b1){
			if(a2 >= b1){
				return true;
			}
		}
		else if(a1 <= b2) {
			return true;
		}

		return false;
	}
}