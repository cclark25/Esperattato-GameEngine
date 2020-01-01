#ifndef ESPERATTATO_NODE_DEF
#define ESPERATTATO_NODE_DEF

#include "../../Dependencies/AllegroCPPWrappers/src/Transform/Transform.h"
#include <set>
using namespace AllegroWrappers;
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

		void addChild(Node child);
		void removeChild(Node child);
		multiset<Node> &getChildren();

		Coordinates getGlobalPosition();
		double getGlobalRotation();
		double getGlobalXScale();
		double getGlobalYScale();
		double getGlobalZIndex();

		multiset<SovereignNode>
		makeNodeSet(Transform rootTransform = Transform(),
		            double rootZIndex = 0);

		friend bool operator<(const Node &first, const Node &second) {
			return first.data->zIndex < second.data->zIndex;
		};
	};

	struct SovereignNode {
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