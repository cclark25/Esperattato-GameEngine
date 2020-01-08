#include "./Node.h"
#include <iostream>
#include <stdexcept>
using namespace std;

namespace Esperatto {
	Node::Node() {
		this->data = new foreign_data();
		this->data->referenceCount = 1;
	}

	Node::Node(const Node &original) : data(original.data) {
		// this->data = original.data;
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

	double Node::getGlobalRotation() {
		foreign_data *level = this->data;
		double result = 0;

		while (true) {
			result += level->rotationRadians;
			if (level->parent != nullptr) {
				level = level->parent;
			} else {
				break;
			}
		}
		return result;
	}

	double Node::getGlobalXScale() {
		foreign_data *level = this->data;
		double result = 0;

		while (true) {
			result += level->xScale;
			if (level->parent != nullptr) {
				level = level->parent;
			} else {
				break;
			}
		}
		return result;
	}

	double Node::getGlobalYScale() {
		foreign_data *level = this->data;
		double result = 0;

		while (true) {
			result += level->yScale;
			if (level->parent != nullptr) {
				level = level->parent;
			} else {
				break;
			}
		}
		return result;
	}

	double Node::getGlobalZIndex() {
		foreign_data *level = this->data;
		double result = 0;

		while (true) {
			result += level->zIndex;
			if (level->parent != nullptr) {
				level = level->parent;
			} else {
				break;
			}
		}
		return result;
	}

	void *Node::getParentPointer() { return this->data->parent; }
	void *Node::getSelfPointer() { return this->data; }

	double Node::getZIndexInParent() { return this->data->zIndex; }
	void Node::setZIndexInParent(double newZ) { this->data->zIndex = newZ; }

	Coordinates Node::getCenterOfRotation() {
		return Coordinates({data->xCenterOfRotation, data->yCenterOfRotation});
	}

	void Node::setCenterOfRotation(double x, double y) {
		data->xCenterOfRotation = x;
		data->yCenterOfRotation = y;
	}

	multiset<SovereignNode> Node::makeNodeSet() {
		multiset<SovereignNode> result;
		Transform thisTransform;
		al_identity_transform(&thisTransform);
		al_translate_transform(
		    &thisTransform,
		    this->data->xPosition - this->data->xCenterOfRotation,
		    this->data->yPosition - this->data->yCenterOfRotation);
		al_rotate_transform(&thisTransform, this->data->rotationRadians);
		al_translate_transform(&thisTransform, this->data->xCenterOfRotation,
		                       this->data->yCenterOfRotation);
		al_scale_transform(&thisTransform, this->data->xScale,
		                   this->data->yScale);
		double globalZIndex = this->data->zIndex;

		SovereignNode sov = {sov, thisTransform, *this, globalZIndex};
		result.insert(sov);

		for (Node child : this->data->children) {
			result.merge(child.makeNodeSet(sov));
		}
		// cout << "i: " << i-- << endl;

		return result;
	}
	multiset<SovereignNode> Node::makeNodeSet(SovereignNode parent) {
		multiset<SovereignNode> result;
		Transform thisTransform;
		al_identity_transform(&thisTransform);
		al_translate_transform(
		    &thisTransform,
		    this->data->xPosition - this->data->xCenterOfRotation,
		    this->data->yPosition - this->data->yCenterOfRotation);
		al_rotate_transform(&thisTransform, this->data->rotationRadians);
		al_translate_transform(&thisTransform, this->data->xCenterOfRotation,
		                       this->data->yCenterOfRotation);
		al_scale_transform(&thisTransform, this->data->xScale,
		                   this->data->yScale);
		
		al_compose_transform(&thisTransform, &parent.transformation);
		double globalZIndex = parent.globalZIndex + this->data->zIndex;

		SovereignNode sov = {parent, thisTransform, *this, globalZIndex};
		result.insert(sov);

		for (Node child : this->data->children) {
			result.merge(child.makeNodeSet(sov));
		}
		// cout << "i: " << i-- << endl;

		return result;
	}

	void Node::foreach (function<void(Node)> func) {
		func(*this);

		for (Node child : this->data->children) {
			child.foreach (func);
		}
	}

	multiset<Node> &Node::getChildren() {
		return this->data->children;
		// return *(const multiset<const Node>*)(void*) &this->data->children;
	}
} // namespace Esperatto