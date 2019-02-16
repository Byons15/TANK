#include "Missile.h"
#include "Ground.h"
#include <SDL_timer.h>
#include "Tank.h"
#include "../Director.h"

Missile::Missile(Ground * ground, Tank * sender, int power, const SDL_Point &beginPos, Mover::DIRECTION direction)
	:Spirit(ground, "missile"), m_sender(sender), m_destory(false), m_boom(false), m_ground(ground), m_power(power)
{
	m_startTime = SDL_GetTicks();

	auto endPos = m_position = beginPos;

	switch (direction)
	{
	case Mover::UP:
		endPos.y = 0;
		break;
	case Mover::RIGHT:
		endPos.x = MAP_SIZE * GRID_SIZE - 1;
		break;
	case Mover::DOWN:
		endPos.y = MAP_SIZE * GRID_SIZE - 1;
		break;
	case Mover::LEFT:
		endPos.x = 0;
		break;
	default:
		break;
	}

	m_mover.move(beginPos, endPos, m_startTime, 1);
}

void Missile::update(Uint32 time)
{
	if (m_destory)
		return;

	if (m_mover.state()) { //飞弹移动中。
		m_position = m_mover.current(time);  //更新位置。

		//检查碰撞。
		SDL_Point colTerrainPos;
		Tank *colTank;
		auto result = m_ground->missileColCheck(this, m_position, &colTank, &colTerrainPos);

		if (result) {  //遇到碰撞.
			m_boomTarget = static_cast<TARGET>(result);
			m_mover.endMove(); 

			if (m_boomTarget == TANK) {
				m_ground->attackTank(colTank, m_power);
			}

			if (m_boomTarget == TERRAIN) {
				m_ground->attackTerrain(colTerrainPos, m_power);
			}
		}
	}

	if(!m_mover.state() && !m_boom) {
		//开始爆炸

		m_startTime = time;
		m_boom = true;
		m_position.x -= 15;
		m_position.y -= 15;
		setAnimation("boom");

		SDL_UserEvent user;
		user.type = BEGIN_BOOM;
		user.code = m_boomTarget;
		user.data1 = this;
		user.data2 = m_sender;
		user.timestamp = time;

		director->userEventTrigger(user);
	}

	if (m_boom) {
		if (time - m_startTime >= 1500) {

			SDL_UserEvent user;
			user.type = END_BOOM;
			user.code = m_boomTarget;
			user.data1 = this;
			user.data2 = m_sender;
			user.timestamp = time;

			director->userEventTrigger(user);
			m_destory = true;  //生命期结束，等待对象被销毁。
		}
	}
}

void Missile::render()
{
	Spirit::renderFrame(m_position);
}
