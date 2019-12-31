#include "./Node.h"
#include <stdexcept>

namespace Esperatto {
	Node::Node() {
		this->data = new foreign_data();
		this->data->referenceCount = 1;
	}

	Node::Node(const Node &original) {
		this->data = original.data;
		data->referenceCount++;
	}

	Node::~Node() {
		this->data->referenceCount--;
		if (this->data->referenceCount == 0) {
			delete this->data;
		}
	}

	void Node::move(double x, double y) {
		this->data->xPosition += x;
		this->data->yPosition += y;
	}

	void Node::rotate(double radians) {
		this->data->rotationRadians += radians;
	}

	void Node::setScaleInParent(double xScale, double yScale) {
		this->data->xScale = xScale;
		this->data->yScale = yScale;
	}

	double Node::getXScaleInParent() { return this->data->xScale; }

	double Node::getYScaleInParent() { return this->data->yScale; }

	void Node::setPositionInParent(double x, double y) {
		this->data->xPosition = x;
		this->data->yPosition = y;
	}

	Coordinates Node::getPositionInParent() {
		return {this->data->xPosition, this->data->yPosition};
	}

	void Node::setRotationInParent(double radians) {
		this->data->rotationRadians = radians;
	}

	double Node::getRotationInParent() { return this->data->rotationRadians; }

	void Node::addChild(Node child) {
		this->data->children.insert((const Node &)child);
		if (child.data->parent == nullptr) {
			child.data->parent = this->data;
		} else {
			throw new logic_error(
			    "This child being added has a parent already. Please remove "
			    "the child from its previous parent before adding it as a "
			    "child here.");
		}
	}

	void Node::removeChild(Node child) { this->data->children.erase(child); }

	Coordinates Node::getGlobalPosition() {
		foreign_data *level = this->data;
		Coordinates result = {0, 0};

		while (true) {
			result.x += level->xPosition;
			result.y += level->yPosition;
			if (level->parent != nullptr) {
				level = level->parent;
			} else {
				break;
			}
		}
		return result;
	}

	multiset<Node> &Node::getChildren() {
		return this->data->children;
		// return *(const multiset<const Node>*)(void*) &this->data->children;
	}
} // namespace Esperatto