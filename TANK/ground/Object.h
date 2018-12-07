#pragma once
#include "../SDL/AnimationFactory.h"
#include "../Spirit.h"

class Object
	:public Spirit
{
public:
	Object(Scene *scene, const std::string &objectName, int height, const SIZE &collisionSize);
	~Object();
private:
	SIZE m_collisionSize;
	Uint32 m_height;
};

