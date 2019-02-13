#include "Tank.h"
#include "../Scene.h"
#include "../TANKS.h"
#include "../Director.h"

TankFactory *Tank::sm_factory = 0;

Tank::Tank(Ground * ground, int &model, const SDL_Point &position)
	:Spirit(ground), m_stopMoving(false), m_invincible(false)
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
}

int Tank::setPosition(const SDL_Point & pos)
{
	//先尝试在ground更新位置， 如果成功再设置坦克位置。
	Tank *colDset;
	auto result = m_ground->tankColCheck(this, pos, &colDset);
	if (!result) {
		m_position = pos;
	}

	return result;
}

int Tank::setCommander(Commander * cmder)
{
	m_commander = cmder;
	m_stopMoving = true;
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

void Tank::unInvincible()
{
	m_invincible = false;
	setAnimation(m_form[m_HP - 1]);
}

int Tank::beHit(Tank *aggressor, int power)
{
	if (m_invincible) {
		return m_HP;
	}

	//如果坦克携带有奖励箱，则触发奖励, 而不是减HP。
	if (m_rewarde != 0) {
		
		SDL_UserEvent user;
		user.type = BONUSCHEST;
		user.code = m_rewarde;
		user.data1 = aggressor;
		user.data2 = this;
		director->userEventTrigger(user);

		setRewards(0);
	}
	else {
		m_HP = (m_HP - power <= 0) ? 0 : m_HP - power;
		setAnimation(m_form[m_HP - 1]);
	}
	return m_HP;
}

void Tank::setFactory(TankFactory * factory)
{
	sm_factory = factory;
}

void Tank::update(Uint32 time)
{
	Mover::DIRECTION direction = m_direction;
	bool fire = false;
	auto result = m_commander->command(m_ground, this, time, fire, direction);


	if (!m_stopMoving) { 
		//当前不是“结束移动”状态，所以要处理移动命令.

		if (result == -1 || direction != m_direction) {
			stopMove();
		}
	}

	if (fire) {
		//TODO::开火!!
	}

	//当处于“结束移动”状态时，我们同“非结束移动”状态一样都从移动器中更新位置，直到移动器结束移动。

	if (m_mover.state()) {
		SDL_Point p = m_mover.current(time);

		if (m_stopMoving) { //当处于“结束移动”状态时，跳过碰撞检查，直接设置新位置。
			m_position = p;
			return;
		}

		//检测碰撞并回应。
		Tank *colDest;
		auto result = m_ground->tankColCheck(this, p, &colDest);
		if (result == -1) { //碰到边界或地形。
			stopMove();
		}
		else if (result == 0) {
			m_position = p;  //没有碰到东西
		}
		else if(result == -2){ //碰到坦克
			stopMove();
			if (colDest->m_direction == m_direction) { //追尾处理
				m_speeds = colDest->m_speeds;
				startMove(m_direction);
			}
		}
	}
	else { //移动器结束时证明坦克已经对齐了网格，我们可以开始执行下一个命令了。
		startMove(direction);
	}
}

bool Tank::onGrid()
{
	auto rect = pixelToGroundRect({ m_position.x, m_position.y, colSize, colSize });
	if(rect.w > 2 || rect.h > 2)
		return false;

	return true;
}
