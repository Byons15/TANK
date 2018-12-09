#include "ObjectsPool.h"
#include "../FileLoader.h"
#include "../Ground.h"

ObjectsPool::ObjectsPool(Ground * ground)
	:m_ground(ground)
{
	std::vector<std::string> data;
	if (!fileLoad("objects.data", "object", &data))
		throw std::runtime_error("EOF : object.data");

	
}

ObjectsPool::~ObjectsPool()
{
}
