#include "CollisionTree.h"

namespace Esperatto
{
	CollisionTree::CollisionTree(const Coordinates &a, const Coordinates &b, bool onX)
	{
		this->onX = onX;
		if (this->onX)
		{
			this->first = (a.x <= b.x) ? a : b;
			this->second = (a.x <= b.x) ? b : a;
		}
		else
		{
			this->first = (a.y <= b.y) ? a : b;
			this->first = (a.y <= b.y) ? b : a;
		}

		this->lessThan = nullptr;
		this->greaterThanEqualTo = nullptr;
	}

	void CollisionTree::addOne(const Coordinates &a, const Coordinates &b)
	{
		this->collisionCount++;
		CollisionTree **destination;
		const Coordinates &first = this->onX ? ((a.x <= b.x) ? a : b) : ((a.y <= b.y) ? a : b);
		const Coordinates &second = this->onX ? ((a.x <= b.x) ? b : a) : ((a.y <= b.y) ? b : a);

		if (this->onX)
		{
			if (first.x >= this->first.x)
			{
				destination = &this->greaterThanEqualTo;
			}
			else
			{
				destination = &this->lessThan;
			}
		}
		else
		{
			if (first.y >= this->first.y)
			{
				destination = &this->greaterThanEqualTo;
			}
			else
			{
				destination = &this->lessThan;
			}
		}

		if ((*destination) == nullptr)
		{
			(*destination) = new CollisionTree(first, second, this->onX);
		}
		else
		{
			(*destination)->addOne(first, second);
		}

		this->rebalance();
	}

	shared_ptr<pair<Coordinates, Coordinates>> CollisionTree::popOne()
	{
		int left = (this->lessThan == nullptr) ? 0 : this->lessThan->collisionCount;
		int right = (this->greaterThanEqualTo == nullptr) ? 0 : this->greaterThanEqualTo->collisionCount;
		int diff = left - right;
		this->collisionCount--;

		shared_ptr<pair<Coordinates, Coordinates>> result = nullptr;

		if (diff > 0)
		{
			result = this->lessThan->popOne();
			if (result == nullptr)
			{
				result = this->lessThan->getRootCoordinates();
				delete this->lessThan;
				this->lessThan = nullptr;
			}
		}
		else if (right == 0)
		{
			return nullptr;
		}
		else
		{
			result = this->greaterThanEqualTo->popOne();
			if (result == nullptr)
			{
				result = this->greaterThanEqualTo->getRootCoordinates();
				delete this->greaterThanEqualTo;
				this->greaterThanEqualTo = nullptr;
			}
		}

		this->first = result->first;
		this->second = result->second;

		return this->getRootCoordinates();
	}

	shared_ptr<pair<Coordinates, Coordinates>> CollisionTree::getRootCoordinates()
	{
		return shared_ptr<pair<Coordinates, Coordinates>>(new pair(this->first, this->second));
	}

	void CollisionTree::rebalance()
	{
		int left = (this->lessThan == nullptr) ? 0 : this->lessThan->collisionCount;
		int right = (this->greaterThanEqualTo == nullptr) ? 0 : this->greaterThanEqualTo->collisionCount;

		if (right - left > 1)
		{
			auto popped = this->greaterThanEqualTo->popOne();
			if (popped == nullptr)
			{
				popped = this->greaterThanEqualTo->getRootCoordinates();
				delete this->greaterThanEqualTo;
				this->greaterThanEqualTo = nullptr;
			}
			if (this->lessThan == nullptr)
			{
				this->lessThan = new CollisionTree(popped->first, popped->second, this->onX);
			}
			else
			{
				this->lessThan->addOne(popped->first, popped->second);
			}
		}
		else if (left - right > 1)
		{
			auto popped = this->lessThan->popOne();
			if (popped == nullptr)
			{
				popped = this->lessThan->getRootCoordinates();
				delete this->lessThan;
				this->lessThan = nullptr;
			}
			if (this->greaterThanEqualTo == nullptr)
			{
				this->greaterThanEqualTo = new CollisionTree(popped->first, popped->second, this->onX);
			}
			else
			{
				this->greaterThanEqualTo->addOne(popped->first, popped->second);
			}
		}
	}
} // namespace Esperatto