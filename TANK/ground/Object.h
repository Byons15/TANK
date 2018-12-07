#pragma once
#include "../SDL/AnimationFactory.h"

class Object
{
public:
	Object();
	~Object();
private:
	Animation m_animation;
	SIZE m_collisionSize;
};

