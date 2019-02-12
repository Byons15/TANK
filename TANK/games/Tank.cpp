#include "Tank.h"
#include "../Scene.h"
#include "../TANKS.h"
#include "../Director.h"

TankFactory *Tank::sm_factory = 0;

Tank::Tank(Ground * ground, int &model, const SDL_Point &position)
	:Spirit(ground)
{
	//从工厂查找并构造参数。
	auto &dat = sm_factory->findTankData(model);
	m_form = dat.form;
	m_rewardsForm = dat.rewardForm;
	m_invincibleForm = dat.invincibleForm;
	m_HP = dat.maxHP;
	m_speeds = dat.speeds;

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
	auto result = m_ground->tankColCheck(this, pos);
	if (!result) {
		m_position = pos;
	}

	return result;
}

void Tank::setRewards(REWARDS r)
{
	m_rewarde = r;
	if (m_rewarde != DEFAULT) {
		setAnimation(m_rewardsForm);
	}
	else {
		setAnimation(m_form[m_HP - 1]);
	}
}

int Tank::beHit(Tank *aggressor, int power)
{
	//如果坦克携带有奖励箱，则触发奖励, 而不是减HP。
	if (m_rewarde != DEFAULT) {
		
		

		setRewards(DEFAULT);
	}
	else if (m_rewarde == REWARDS::INVINCIBLE) {
		
	}
	else{
		
	}

	

	switch (m_rewarde)
	{
	case Tank::DEFAULT:
		m_HP = (m_HP - power <= 0) ? 0 : m_HP - power;
		setAnimation(m_form[m_HP - 1]);
		return m_HP;
	case Tank::ADDLIVE:
		break;
	case Tank::SUPERPOWER:
		break;
	case Tank::INVINCIBLE:
		return m_HP;
	default:
		break;
	}

	//触发奖励。
	SDL_UserEvent user;
	user.type = BONUSCHEST;
	user.code = m_rewarde;
	user.data1 = aggressor;
	director->userEventTrigger(user);

	setRewards(DEFAULT);
	return m_HP;
}

void Tank::setFactory(TankFactory * factory)
{
	sm_factory = factory;
}

void Tank::update(Uint32 time)
{
	
}
