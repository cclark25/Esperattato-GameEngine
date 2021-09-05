#include "./Node.h"
#include <iostream>
#include <stdexcept>
using namespace std;

namespace Esperatto
{
	string Coordinates::toString()
	{
		return "{" + to_string(this->x) + "," + to_string(this->y) + "}";
	}

	Node::Node() : parent(this)
	{
		return;
	}

	Node::~Node()
	{
		for (NodeRelationship::Child *child : this->children)
		{
			delete child;
		}
		this->children.clear();
	}

	void Node::move(double x, double y)
	{
		this->xPosition += x;
		this->yPosition += y;
	}

	void Node::rotate(double radians)
	{
		this->rotationRadians += radians;
	}

	void Node::setScaleInParent(double xScale, double yScale)
	{
		this->xScale = xScale;
		this->yScale = yScale;
	}

	double Node::getXScaleInParent() { return this->xScale; }

	double Node::getYScaleInParent() { return this->yScale; }

	void Node::setPositionInParent(double x, double y)
	{
		this->xPosition = x;
		this->yPosition = y;
	}

	Coordinates Node::getPositionInParent()
	{
		return {this->xPosition, this->yPosition};
	}

	void Node::setRotationInParent(double radians)
	{
		this->rotationRadians = radians;
	}

	double Node::getRotationInParent() { return this->rotationRadians; }

	void Node::addChild(Node &child)
	{
		this->children.insert(new NodeRelationship::Child(this, child.parent));
	}

	void Node::removeChild(Node &childToRemove)
	{
		for (NodeRelationship::Child *child : this->children)
		{
			if (child->getPointer() == &childToRemove)
			{
				delete child;
				this->children.erase(child);
				break;
			}
		}
	}

	Coordinates Node::getGlobalPosition()
	{
		Node *level = this;
		Coordinates result = {0, 0};

		while (true)
		{
			result.x += level->xPosition;
			result.y += level->yPosition;
			if (!level->parent.isDeleted())
			{
				level = level->parent.getPointer();
			}
			else
			{
				break;
			}
		}
		return result;
	}

	double Node::getGlobalRotation()
	{
		Node *level = this;
		double result = 0;

		while (true)
		{
			result += level->rotationRadians;
			if (!level->parent.isDeleted())
			{
				level = level->parent.getPointer();
			}
			else
			{
				break;
			}
		}
		return result;
	}

	double Node::getGlobalXScale()
	{
		Node *level = this;
		double result = 0;

		while (true)
		{
			result += level->xScale;
			if (!level->parent.isDeleted())
			{
				level = level->parent.getPointer();
			}
			else
			{
				break;
			}
		}
		return result;
	}

	double Node::getGlobalYScale()
	{
		Node *level = this;
		double result = 0;

		while (true)
		{
			result += level->yScale;
			if (!level->parent.isDeleted())
			{
				level = level->parent.getPointer();
			}
			else
			{
				break;
			}
		}
		return result;
	}

	double Node::getGlobalZIndex()
	{
		Node *level = this;
		double result = 0;

		while (true)
		{
			result += level->zIndex;
			if (!level->parent.isDeleted())
			{
				level = level->parent.getPointer();
			}
			else
			{
				break;
			}
		}
		return result;
	}

	double Node::getZIndexInParent() { return this->zIndex; }
	void Node::setZIndexInParent(double newZ) { this->zIndex = newZ; }

	Coordinates Node::getCenterOfRotation()
	{
		return Coordinates({this->xCenterOfRotation, this->yCenterOfRotation});
	}

	void Node::setCenterOfRotation(double x, double y)
	{
		this->xCenterOfRotation = x;
		this->yCenterOfRotation = y;
	}

	multiset<SovereignNode> Node::makeNodeSet(Transform &baseTransform)
	{
		multiset<SovereignNode> result;
		static Transform thisTransform;
		al_identity_transform(&thisTransform);
		al_translate_transform(
			&thisTransform,
			this->xPosition - this->xCenterOfRotation,
			this->yPosition - this->yCenterOfRotation);
		al_rotate_transform(&thisTransform, this->rotationRadians);
		al_translate_transform(&thisTransform, this->xCenterOfRotation,
							   this->yCenterOfRotation);
		al_scale_transform(&thisTransform, this->xScale,
						   this->yScale);

		al_compose_transform(&thisTransform, &baseTransform);

		static double globalZIndex = this->zIndex;

		static SovereignNode sov = {sov, thisTransform, this, globalZIndex};
		result.insert(sov);

		for (NodeRelationship::Child *child : this->children)
		{
			if (!child->isDeleted())
			{
				result.merge((*child)->makeNodeSet(sov));
			}
		}

		return result;
	}
	multiset<SovereignNode> Node::makeNodeSet(SovereignNode parent)
	{
		multiset<SovereignNode> result;
		static Transform thisTransform;
		al_identity_transform(&thisTransform);
		al_translate_transform(
			&thisTransform,
			this->xPosition - this->xCenterOfRotation,
			-(this->yPosition - this->yCenterOfRotation));
		al_rotate_transform(&thisTransform, this->rotationRadians);
		al_translate_transform(&thisTransform, this->xCenterOfRotation,
							   -this->yCenterOfRotation);
		al_scale_transform(&thisTransform, this->xScale,
						   this->yScale);

		al_compose_transform(&thisTransform, &parent.transformation);
		double globalZIndex = parent.globalZIndex + this->zIndex;

		SovereignNode sov = {parent, thisTransform, this, globalZIndex};
		result.insert(sov);

		for (NodeRelationship::Child *child : this->children)
		{
			if (!child->isDeleted())
			{
				result.merge((*child)->makeNodeSet(sov));
			}
		}
		// cout << "i: " << i-- << endl;

		return result;
	}

	void Node::foreach (function<void(Node)> func)
	{
		// func(*this);

		for (NodeRelationship::Child *child : this->children)
		{
			(*child)->foreach (func);
		}
	}

	const multiset<NodeRelationship::Child *> &Node::getChildren()
	{
		return this->children;
		// return *(const multiset<const Node>*)(void*) &this->children;
	}

	Transform Node::getTransform(){
		Transform transform = Transform();
		al_identity_transform(&transform);
		return transform;
	}

	Node *Node::getShared()
	{
		return this;
	}

	Bitmap Node::getBitmap()
	{
		return nullptr;
	}
} // namespace Esperatto