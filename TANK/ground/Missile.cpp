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

	if (SDL_PointInRect(&beginPos, &m_ground->rect()) == SDL_FALSE)
		return;

	int lenght;

	switch (direction)
	{
	case Mover::UP:
		endPos.y = 0;
		lenght = beginPos.y;
		break;
	case Mover::RIGHT:
		lenght = (MAP_SIZE * GRID_SIZE) - beginPos.x;
		break;
	case Mover::DOWN:
		lenght = (MAP_SIZE * GRID_SIZE) - beginPos.y;
		break;
	case Mover::LEFT:
		lenght = beginPos.x;
		break;
	default:
		break;
	}
	m_mover.move(beginPos, direction, lenght, m_startTime, 1);
}

void Missile::update(Uint32 time)
{
	if (m_destory)
		return;

	if (m_mover.state()) { //飞弹移动中。
		m_position = m_mover.current(time);  //更新位置。

		//检查碰撞。
		auto result = m_ground->positionTest(this, m_position);
		if (result) {  //遇到碰撞.
			m_boomTarget = static_cast<TARGET>(result);
			m_mover.endMove();
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
		if (time - m_startTime >= 500) {

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
