// #include "NodeSubTypes.h"

// namespace Esperatto
// {
// 	NodeSubtype::NodeSubtype(void (*dataDeleter)(shared_ptr<void>), shared_ptr<Subdata> data,
// 							 size_t type)
// 	{
// 		this->dataDeleter = dataDeleter;
// 		this->data = data;
// 		this->type = type;
// 	}

// 	NodeSubtype::~NodeSubtype()
// 	{
// 		if (data != nullptr)
// 		{
// 			this->dataDeleter(this->data);
// 		}
// 	}
// } // namespace Esperatto