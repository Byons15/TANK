#include "Tank.h"
#include "../Scene.h"
#include "../TANKS.h"
#include "../Timer.h"
#include "Ground.h"
#include <sstream>

TankFactory *Tank::sm_factory = 0;

Tank::Tank(Ground * ground, CAMP camp, MODEL &model, const SDL_Point &position)
	:Spirit(ground), m_invincible(false), m_model(model), m_ground(ground), m_power(1), m_reload(false), m_driver(0), m_camp(camp)
{
	//从工厂查找并构造参数。
	auto &dat = sm_factory->findTankData(model);
	m_form = dat.form;
	m_rewardsForm = dat.rewardForm;
	m_invincibleForm = dat.invincibleForm;
	m_HP = dat.maxHP;
	m_defaultSpeeds = m_speeds = dat.speeds;

	//设置坦克当前形态。
	setAnimation(m_form[m_HP - 1]);

	m_position = position;
}

Tank::~Tank()
{
	if (m_driver)
		m_driver->drive(nullptr);
}

int Tank::setGroundPosition(const SDL_Point & pos)
{
	//先尝试在ground更新位置， 如果成功再设置坦克位置。
	SDL_Point pixel;
	pixel.x = pos.x * GRID_SIZE;
	pixel.y = pos.y * GRID_SIZE;
	Tank *colDset;
	auto result = m_ground->positionTest(this, pixel, &colDset);
	if (!result) {
		m_position = pixel;
		m_lastRect.x = pos.x;
		m_lastRect.y = pos.y;
		m_lastRect.w = m_lastRect.h = 2;
	}

	return result;
}

void Tank::fire()
{
	if (m_reload)
		return;

	m_ground->addMissile(new Missile(m_ground, this, m_power));
	m_reload = true;
}

void Tank::reload()
{
	m_reload = false;
}

int Tank::startMove(Mover::DIRECTION direction, Uint32 time)
{
	SDL_Point dest = m_position;
	int maxLenght;
	switch (direction)
	{
	case Mover::UP:
		maxLenght = m_position.y;
		break;
	case Mover::RIGHT:
		maxLenght = GRID_SIZE * MAP_SIZE - GRID_SIZE * 2 - m_position.x;
		break;
	case Mover::DOWN:
		maxLenght = GRID_SIZE * MAP_SIZE - GRID_SIZE * 2 - m_position.y;
		break;
	case Mover::LEFT:
		maxLenght = m_position.x;
		break;
	default:
		break;
	}

	m_direction = direction;
	setAngle(direction);
	
	m_mover.move(m_position, direction, maxLenght, time, m_speeds);
	return 0;
}

void Tank::stopMove()
{
	//如果与网格对其的，则不进入“结束移动状态”，仅简单地结束移动。
	if (onGrid()) {
		m_mover.endMove();
		return;
	}
	
	//获取坦克覆盖的范围。
	auto gridRect = pixelToGroundRect({ m_position.x, m_position.y, colSize, colSize });
	
	//根据当前方向获取停止位置。 
	switch (m_direction)
	{
	case Mover::UP:
		m_position.x = gridRect.x * GRID_SIZE;
		m_position.y = gridRect.y * GRID_SIZE;
		break;
	case Mover::RIGHT:
		m_position.x = (gridRect.x + gridRect.w - 2) * GRID_SIZE;
		m_position.y = gridRect.y * GRID_SIZE;
		break;
	case Mover::DOWN:
		m_position.x = gridRect.x * GRID_SIZE;
		m_position.y = (gridRect.y + gridRect.h - 2) * GRID_SIZE;
		break;
	case Mover::LEFT:
		m_position.x = gridRect.x * GRID_SIZE;
		m_position.y = gridRect.y * GRID_SIZE;
		break;
	default:
		break;
	}

	m_mover.endMove();
}

int Tank::setDriver(Driver * cmder)
{
	cmder->drive(this);

	m_driver = cmder;
	return 0;
}

int Tank::setRewards(int rewarde)
{
	if (m_invincible)
		return -1;

	if (!rewarde) {
		setAnimation(m_form[m_HP - 1]);
		m_rewarde = 0;
		return 0;
	}
	
	m_rewarde = rewarde;
	setAnimation(m_rewardsForm);
	return 0;
}

int Tank::invincible()
{
	if (m_rewarde) {
		return -1;
	}

	m_invincible = true;
	setAnimation(m_invincibleForm);
	return 0;
}

int Tank::attack(Driver * aggressor, Tank * target, int power)
{
	if (target->m_invincible) {
		return target->m_HP;
	}

	SDL_UserEvent user;
	user.data1 = aggressor;
	//如果坦克携带有奖励箱，则触发奖励, 而不是减HP。
	if (target->m_rewarde != 0) {
		user.type = BONUSCHEST;
		user.code = target->m_rewarde;
		user.data2 = target;
		director->userEventTrigger(user);

		target->setRewards(0);
	}
	else {
		target->m_HP = (target->m_HP - power <= 0) ? 0 : target->m_HP - power;
		target->setAnimation(target->m_form[target->m_HP ? target->m_HP - 1 : 0]);
		user.type = ATTACK;
		user.code = target->m_camp;
		user.data2 = reinterpret_cast<void *> (target);
		director->userEventTrigger(user);
	}

	if (!target->m_HP) {

		user.type = KILLED;
		user.code = target->m_camp;
		user.data2 = target->driver();
		user.timestamp = target->m_model;
		director->userEventTrigger(user);
	}

	return target->m_HP;
}

void Tank::unInvincible()
{
	m_invincible = false;
	setAnimation(m_form[m_HP - 1]);
}

void Tank::setFactory(TankFactory * factory)
{
	sm_factory = factory;
}

void Tank::update(Uint32 time)
{
	if(!m_reload)
		if (m_driver->requestFire())
			fire();

	if (m_mover.state()) {
		auto newPos = m_mover.current(time);

		//检查新旧两个位置的范围，如果两个范围不一样，则坦克准备进入了新的位置，此时要检查输入以及碰撞来确定是否进入新的位置。
		SDL_Rect r1 = pixelToGroundRect({ newPos.x, newPos.y, colSize, colSize });
		if (SDL_RectEquals(&r1, &m_lastRect) == SDL_FALSE) {

			SDL_Point gridPos;
			switch (m_direction)
			{
			case Mover::UP:
			case Mover::LEFT:
				gridPos.x = m_lastRect.x;
				gridPos.y = m_lastRect.y;
				break;
			case Mover::RIGHT:
				gridPos.x = m_lastRect.x + m_lastRect.w - 2;
				gridPos.y = m_lastRect.y;
				break;
			case Mover::DOWN:
				gridPos.x = m_lastRect.x;
				gridPos.y = m_lastRect.y + m_lastRect.h - 2;
				break;
			default:
				break;
			}

			//新位置占用的网格与当前网格不同时，检查输入。
			Mover::DIRECTION direction = m_direction;

			if (m_driver->tank() != this)
				__debugbreak;
			auto result = m_driver->command(gridPos, time, direction);
			if (result == 0) { //有移动命令.

				//检查新位置是否碰撞.
				Tank *colDest = nullptr;
				auto checkResult = m_ground->positionTest(this, newPos, &colDest);
				if (checkResult == 0) { //新位置没有发生碰撞 

					//没有改变方向时，则继续前进，更新位置。
					if (m_direction == direction) {
						m_position = newPos;
						m_lastRect = r1;
					}
					else {  //新命令改变了方向，停止当前移动。
						stopMove();
					}
				}
				else {  //新位置发生碰撞。
					stopMove();
				}
			}
			else if(result == -1){
				stopMove();
			}
		}
		else {
			m_position = newPos;
			m_lastRect = r1;
		}
		
	}
	else {
		Mover::DIRECTION direction = m_direction;
		auto result = m_driver->command({m_position.x / GRID_SIZE, m_position.y / GRID_SIZE}, time, direction);
		startMove(direction, time);
	}

}

void Tank::render()
{
	Spirit::renderFrame(m_position);
}

bool Tank::onGrid()
{
	if (m_position.x % GRID_SIZE || m_position.y % GRID_SIZE)
		return false;

	return true;
}

Driver::Driver()
	:m_tank(nullptr), m_total(0)
{
	if (sm_scoreTable.empty()) {
		std::vector<std::string> data;
		sm_scoreTable.resize(8, 0);
		fileLoad("scoreTable", &data);
		std::istringstream is(data[0]);
		for (int i = 0; i != 8; ++i) {
			is >> sm_scoreTable[i];
		}
	}
}

void Driver::record(Tank::MODEL model)
{
	m_socrecards[model].killCount++;
	m_socrecards[model].total += sm_scoreTable[model];
	m_total += sm_scoreTable[model];
}
