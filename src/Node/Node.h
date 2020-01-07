#ifndef ESPERATTATO_NODE_DEF
#define ESPERATTATO_NODE_DEF

#include <functional>
#include <set>
#include "../Types.h"
using namespace std;

namespace Esperatto {
	struct Coordinates {
		double x;
		double y;
	};

	struct SovereignNode;

	class Node {
	  private:
		struct foreign_data {
			double xPosition = 0;
			double yPosition = 0;
			double rotationRadians = 0;
			double xScale = 1;
			double yScale = 1;
			double zIndex = 0;
			unsigned int referenceCount = 0;
			multiset<Node> children;
			foreign_data *parent = nullptr;
		} * data;

	  protected:
		Transform getTransform();

	  public:
		Node();
		Node(const Node &original);
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

		void addChild(Node child);
		void removeChild(Node child);
		multiset<Node> &getChildren();

		Coordinates getGlobalPosition();
		double getGlobalRotation();
		double getGlobalXScale();
		double getGlobalYScale();
		double getGlobalZIndex();

		void *getParentPointer();
		void *getSelfPointer();

		multiset<SovereignNode> makeNodeSet();

		multiset<SovereignNode> makeNodeSet(SovereignNode parent);

		void foreach (function<void(Node)> func);

		friend bool operator<(const Node &first, const Node &second) {
			return first.data->zIndex < second.data->zIndex;
		};
	};

	struct SovereignNode {
		SovereignNode &parent;
		Transform transformation;
		Node node;
		double globalZIndex;

		friend bool operator<(const SovereignNode &first,
		                      const SovereignNode &second) {
			return first.globalZIndex < second.globalZIndex;
		};
	};
} // namespace Esperatto

#endif