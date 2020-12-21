#include "CollisionTree.h"

namespace Esperatto {
	CollisionTree::CollisionTree(const Coordinates &a, const Coordinates &b,
	                             bool onX) {
		this->onX = onX;
		if (this->onX) {
			this->first = (a.x <= b.x) ? a : b;
			this->second = (a.x <= b.x) ? b : a;
		} else {
			this->first = (a.y <= b.y) ? a : b;
			this->first = (a.y <= b.y) ? b : a;
		}

		this->lessThan = nullptr;
		this->greaterThanEqualTo = nullptr;
	}

	void CollisionTree::addOne(const Coordinates &a, const Coordinates &b) {
		this->collisionCount++;
		CollisionTree **destination;
		const Coordinates &first =
		    this->onX ? ((a.x <= b.x) ? a : b) : ((a.y <= b.y) ? a : b);
		const Coordinates &second =
		    this->onX ? ((a.x <= b.x) ? b : a) : ((a.y <= b.y) ? b : a);

		if (this->onX) {
			if (first.x >= this->first.x) {
				destination = &this->greaterThanEqualTo;
				cout << "Inserted on the right\n";

			} else {
				destination = &this->lessThan;
				cout << "Inserted on the left\n";
			}
		} else {
			if (first.y >= this->first.y) {
				destination = &this->greaterThanEqualTo;
				cout << "Inserted on the right\n";
			} else {
				destination = &this->lessThan;
				cout << "Inserted on the left\n";
			}
		}

		if ((*destination) == nullptr) {
			(*destination) = new CollisionTree(first, second, this->onX);
		} else {
			(*destination)->addOne(first, second);
		}

		this->rebalance();
	}

	pair<bool, shared_ptr<pair<Coordinates, Coordinates>>>
	CollisionTree::popOne() {
		this->collisionCount--;
		auto result = shared_ptr<pair<Coordinates, Coordinates>>(
		    new pair<Coordinates, Coordinates>(this->first, this->second));

		int lessThanLength =
		    this->lessThan == nullptr ? 0 : this->lessThan->collisionCount;
		int greaterThanEqualLength =
		    this->greaterThanEqualTo == nullptr
		        ? 0
		        : this->greaterThanEqualTo->collisionCount;

		pair<bool, shared_ptr<pair<Coordinates, Coordinates>>> subPop(false,
		                                                              nullptr);

		if (greaterThanEqualLength == 0 && lessThanLength == 0) {

		} else if (greaterThanEqualLength >= lessThanLength) {
			subPop = this->greaterThanEqualTo->popOne();
			if (subPop.first) {
				this->greaterThanEqualTo = nullptr;
			}
		} else {
			subPop = this->lessThan->popOne();
			if (subPop.first) {
				this->lessThan = nullptr;
			}
		}

		if (subPop.second != nullptr) {
			this->first = subPop.second->first;
			this->second = subPop.second->second;
		} else {
			delete this;
		}

		return pair<bool, shared_ptr<pair<Coordinates, Coordinates>>>(
		    subPop.second == nullptr, result);
	}

	shared_ptr<pair<Coordinates, Coordinates>>
	CollisionTree::getRootCoordinates() {
		return shared_ptr<pair<Coordinates, Coordinates>>(
		    new pair(this->first, this->second));
	}

	void CollisionTree::rebalance() {
		int leftLength =
		    (this->lessThan == nullptr) ? 0 : this->lessThan->collisionCount;
		int rightLength = (this->greaterThanEqualTo == nullptr)
		                      ? 0
		                      : this->greaterThanEqualTo->collisionCount;

		while (abs(rightLength - leftLength) > 1) {
			auto popped = this->popOne();

			this->addOne(popped.second->first, popped.second->second);

			leftLength = (this->lessThan == nullptr)
			                 ? 0
			                 : this->lessThan->collisionCount;
			rightLength = (this->greaterThanEqualTo == nullptr)
			                  ? 0
			                  : this->greaterThanEqualTo->collisionCount;
		}
	}

	int CollisionTree::getCount() {
		int lessThanLength =
		    this->lessThan == nullptr ? 0 : this->lessThan->getCount();
		int greaterThanEqualLength =
		    this->greaterThanEqualTo == nullptr
		        ? 0
		        : this->greaterThanEqualTo->getCount();
		
		return 1 + lessThanLength + greaterThanEqualLength;
	}
} // namespace Esperatto