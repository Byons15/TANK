#include "Object.h"

Object::Object(Scene * scene, const std::string & objectName, int height, const SIZE &collisionSize)
	:Spirit(scene, objectName), m_collisionSize(collisionSize), m_height(height)
{
	
}

Object::~Object()
{
}
