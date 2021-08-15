#ifndef ESPERATTO_NODE_DEF
#define ESPERATTO_NODE_DEF

#include "../Types.h"
#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
using namespace std;

namespace Esperatto
{
	struct Coordinates
	{
		double x;
		double y;
		string toString();
	};

	struct SovereignNode;
	class NodeSubtype;
	class Subdata;
	class Node
	{

	protected:
		char *type = (char *)(void *)typeid(*this).name();
		double xPosition = 0;
		double yPosition = 0;
		double xCenterOfRotation = 0;
		double yCenterOfRotation = 0;
		double rotationRadians = 0;
		double xScale = 1;
		double yScale = 1;
		double zIndex = 0;
		unsigned int referenceCount = 0;
		Node* parent = nullptr;
		multiset<Node*> children;

		Transform getTransform();

	public:
		Node();
		~Node();

		void move(double x, double y);
		void rotate(double radians);

		void setPositionInParent(double x, double y);
		Coordinates getPositionInParent();
		void setRotationInParent(double radians);
		double getRotationInParent();
		void setScaleInParent(double xScale, double yScale);
		double getXScaleInParent();
		double getYScaleInParent();
		double getZIndexInParent();
		void setZIndexInParent(double);

		Coordinates getCenterOfRotation();
		void setCenterOfRotation(double x, double y);

		void addChild(Node* child);
		void removeChild(Node* child);
		multiset<Node*> &getChildren();

		Coordinates getGlobalPosition();
		double getGlobalRotation();
		double getGlobalXScale();
		double getGlobalYScale();
		double getGlobalZIndex();

		virtual Bitmap getBitmap();

		Node* getShared();

		multiset<SovereignNode> makeNodeSet(Transform &baseTransform);

		multiset<SovereignNode> makeNodeSet(SovereignNode parent);

		void foreach (function<void(Node)> func);

		friend bool operator<(const Node &first, const Node &second)
		{
			return first.zIndex < second.zIndex;
		};
	};

	struct SovereignNode
	{
		SovereignNode &parent;
		Transform transformation;
		Node* node;
		double globalZIndex;

		friend bool operator<(const SovereignNode &first,
							  const SovereignNode &second)
		{
			return first.globalZIndex < second.globalZIndex;
		};
	};
} // namespace Esperatto

#endif