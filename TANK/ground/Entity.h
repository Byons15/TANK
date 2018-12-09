#pragma once
#include <SDL_rect.h>
#include "Object.h"
#include "../Ground.h"

class Entity
{
public:
	Entity(Ground *ground, Object *obj, const SDL_Point &position);
	virtual ~Entity();

	void setPosition(const SDL_Point &pos) {
		m_position = pos;
	}
	const SDL_Point &position() {
		return m_position;
	}
	inline SDL_Rect collisionRect();

	Object *object() {
		return m_obj;
	}
private:
	SDL_Point m_position;
	Ground *m_ground;
	Object *m_obj;
};

