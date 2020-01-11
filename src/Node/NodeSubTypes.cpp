#include "NodeSubTypes.h"

namespace Esperatto {
	NodeSubtype::NodeSubtype(void (*dataDeleter)(void *), void *data,
	                         size_t type) {
		this->dataDeleter = dataDeleter;
		this->data = data;
		this->type = type;
	}

	NodeSubtype::~NodeSubtype() {
		if (data != nullptr) {
			this->dataDeleter(this->data);
		}
	}
} // namespace Esperatto