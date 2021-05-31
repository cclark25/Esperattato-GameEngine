#ifndef ESPERATTO_NODE_SUBTYPES
#define ESPERATTO_NODE_SUBTYPES

#include <map>
#include <utility>
#include <memory>
using namespace std;

namespace Esperatto {
	class NodeSubtype;
	/*
		Sub data should always implement the declareParent function so that child objects
			can get and manipulate their node parents.
		TODO: Finish writing the code to pass this data around.
	*/
	class Subdata {
		virtual void declareParent(const NodeSubtype &parent);
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