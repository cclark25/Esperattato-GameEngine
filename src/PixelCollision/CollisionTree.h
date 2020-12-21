#ifndef ESPERATTO_COLLISION_TREE
#define ESPERATTO_COLLISION_TREE

#include <string>
#include <map>
#include "../Image/Image.h"
#include "../Node/Node.h"

namespace Esperatto
{
	class CollisionTree : public pair<Coordinates, Coordinates>
	{
	public:
		CollisionTree *lessThan = nullptr;
		CollisionTree *greaterThanEqualTo = nullptr;
		int collisionCount = 1;
		pair<bool, shared_ptr<pair<Coordinates, Coordinates>>> popOne();
		bool onX = true;
		void rebalance();

	// public:
		CollisionTree(const Coordinates &a, const Coordinates &b, bool onX = true);
		shared_ptr<pair<Coordinates, Coordinates>> getRootCoordinates();
		void addOne(const Coordinates &a, const Coordinates &b);
		int getCount();
	};
} // namespace Esperatto

#endif