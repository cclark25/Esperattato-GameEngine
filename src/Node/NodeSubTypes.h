#ifndef ESPERATTO_NODE_SUBTYPES
#define ESPERATTO_NODE_SUBTYPES

#include <map>
#include <utility>
#include <memory>
using namespace std;

namespace Esperatto {
	class NodeSubtype {
		void (*dataDeleter)(shared_ptr<void>);
		shared_ptr<void> data;
		size_t type;

	  private:
	  	NodeSubtype(void (*dataDeleter)(shared_ptr<void>), shared_ptr<void> data, size_t type);
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