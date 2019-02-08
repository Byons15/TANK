#include "Tank.h"
#include "../Scene.h"
#include "../TANKS.h"
#include "../Director.h"

TankFactory *Tank::sm_factory = 0;

Tank::Tank(Ground * ground, int &model, REWARDS rewards = DEFAULT)
	:Spirit(ground), m_rewarde(rewards)
{
	//从工厂查找并构造参数。
	auto &dat = sm_factory->findTankData(model);
	m_form = dat.form;
	m_rewardsForm = dat.rewardForm;
	m_invincibleForm = dat.invincibleForm;
	m_HP = dat.maxHP;
	m_speeds = dat.speeds;

	//设置坦克当前形态。
	beHit(0);
	
	//是否带有奖励。
	if (rewards) {
		setAnimation(m_rewardsForm);
	}

	//坦克的碰撞大小为两格网格-1.
	m_pixelRect.h = m_pixelRect.w = GRID_SIZE * 2 - 1;
}

Tank::~Tank()
{
}

int Tank::beHit(int power)
{
	//如果坦克携带有奖励箱，则触发奖励, 而不是减HP。
	if (m_rewarde != DEFAULT) {
		
		//触发奖励。
		SDL_UserEvent user;
		user.type = BONUSCHEST;
		user.code = m_rewarde;
		director->userEventTrigger(user);

		//恢复成普通坦克。
		setAnimation(m_form[m_HP - 1]);
		m_rewarde = DEFAULT;
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
