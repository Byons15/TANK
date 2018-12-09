#include "Entity.h"

Entity::Entity(Ground * ground, Object * obj, const SDL_Point & position)
	:m_position(position), m_obj(obj), m_ground(ground)
{

}

Entity::~Entity()
{
}

inline SDL_Rect Entity::collisionRect()
{
	return SDL_Rect{ m_position.x , m_position.y , m_obj->collisionSize().w, m_obj->collisionSize().h };
}
