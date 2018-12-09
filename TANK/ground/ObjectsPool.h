#pragma once
#include <vector>
#include "Object.h"

class Ground;

class ObjectsPool
{
public:
	ObjectsPool(Ground *ground);
	~ObjectsPool();
private:
	std::vector<Object> m_pool;
	Ground *m_ground;
};

