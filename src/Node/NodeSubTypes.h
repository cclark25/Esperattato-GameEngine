#ifndef ESPERATTO_NODE_SUBTYPES
#define ESPERATTO_NODE_SUBTYPES

#include <map>
#include <utility>
#include <memory>
#include <iostream>
#include "Node.h"
using namespace std;

namespace Esperatto {
	class Node;
	class NodeSubtype;
	class Subdata {
		virtual void declareParent(const Node *parent) {
		}
	};

	class NodeSubtype {
		void (*dataDeleter)(shared_ptr<void>);
		shared_ptr<void> data;
		size_t type;

	  public:
	  	NodeSubtype(void (*dataDeleter)(shared_ptr<void>), shared_ptr<Subdata> data, size_t type);
		~NodeSubtype();

		shared_ptr<void> getData(){
			return data;
		}

		size_t getType(){
			return type;
		}

		friend class Node;
	};
} // namespace Esperatto

#endif