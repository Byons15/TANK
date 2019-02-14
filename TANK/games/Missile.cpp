#include "Missile.h"
#include "Ground.h"
#include <SDL_timer.h>
#include "Tank.h"
#include "../Director.h"

Missile::Missile(Ground * ground, Tank * sender, const SDL_Point &beginPos, Mover::DIRECTION direction)
	:Spirit(ground, "missile"), m_sender(sender), m_destory(false), m_boom(false)
{
	m_startTime = SDL_GetTicks();

	auto endPos = m_position = beginPos;

	switch (direction)
	{
	case Mover::UP:
		endPos.y = 0;
		break;
	case Mover::RIGHT:
		endPos.x = MAP_SIZE - 1;
		break;
	case Mover::DOWN:
		endPos.y = MAP_SIZE - 1;
		break;
	case Mover::LEFT:
		endPos.x = 0;
		break;
	default:
		break;
	}

	endPos.x = endPos.x * GRID_SIZE;
	endPos.y = endPos.y * GRID_SIZE;
	m_mover.move(beginPos, endPos, m_startTime, 1);
}

void Missile::update(Uint32 time)
{
	if (m_destory)
		return;

	if (m_boom) {
		if (time - m_startTime >= 1500) {

			SDL_UserEvent user;
			user.type = BOOM;
			user.code = m_boomTarget;
			user.data1 = this;
			user.data2 = m_sender;
			user.timestamp = time;

			director->userEventTrigger(user);
			m_destory = true;
		}
	}

	if (m_mover.state()) {
		m_position = m_mover.current(time);

	}

	if(!m_mover.state()) {
		m_startTime = time;
		m_boom = true;
		setAnimation("boom");
	}
}
