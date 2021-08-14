#ifndef ESPERATTATO_NODE_DEF
#define ESPERATTATO_NODE_DEF

#include "../Types.h"
#include "NodeSubTypes.h"
#include <functional>
#include <iostream>
#include <memory>
#include <set>
#include <utility>
using namespace std;

namespace Esperatto {
	struct Coordinates {
		double x;
		double y;
		string toString();
	};

	struct SovereignNode;
	class NodeSubtype;
	class Subdata;

	class Node {

	  protected:
		struct foreign_data {
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
			shared_ptr<NodeSubtype> subdata;
			multiset<Node> children;
			shared_ptr<foreign_data> parent;

			template <typename T,
			          typename = enable_if_t<is_base_of_v<Subdata, T>>>
			foreign_data(shared_ptr<T> data) {
				this->subdata = make_shared<NodeSubtype>(
				    [](shared_ptr<void> toDelete) {
					    // delete (shared_ptr<T>)(toDelete);
				    },
				    data, typeid(T).hash_code());
			}
			foreign_data(){
				
				this->subdata = make_shared<NodeSubtype>(
				    [](shared_ptr<void> toDelete) {
					    // delete (shared_ptr<T>)(toDelete);
				    },
				    nullptr, typeid(void).hash_code());
			}
		};
		shared_ptr<foreign_data> data;

		Transform getTransform();

	  public:
		Node();

		template <typename T, typename = enable_if_t<is_base_of_v<Subdata, T>>>
		Node(shared_ptr<T> d) {
			this->data = make_shared<foreign_data>(d);
			this->data->referenceCount = 1;
			d->declareParent(this);
		}

		Node(const Node &original);
		~Node();

		void move(double x, double y);
		void rotate(double radians);

		size_t getSubType();
		shared_ptr<void> getDataPtr();

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

		multiset<SovereignNode> makeNodeSet(Transform &baseTransform);

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