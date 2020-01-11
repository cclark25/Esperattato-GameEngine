#ifndef ESPERATTO_NODE_SUBTYPES
#define ESPERATTO_NODE_SUBTYPES

#include <map>
#include <utility>
using namespace std;

namespace Esperatto {
	class NodeSubtype {
		void (*dataDeleter)(void *);
		void *data;
		size_t type;

	  private:
	  	NodeSubtype(void (*dataDeleter)(void *), void* data, size_t type);
		~NodeSubtype();

		void* getData(){
			return data;
		}

		size_t getType(){
			return type;
		}

		friend class Node;
	};
} // namespace Esperatto

#endif