#include "Tank.h"
#include "../Scene.h"

TankFactory *Tank::sm_factory = 0;

Tank::Tank(Ground * ground, int &model)
	:Spirit(ground)
{
	auto &dat = sm_factory->findTankData(model);
	m_form = dat.form;
	m_rewardsForm = dat.rewardForm;
	m_invincibleForm = dat.invincibleForm;
	m_HP = dat.maxHP;
	m_speeds = dat.speeds;

	beHit(0);
}

Tank::~Tank()
{
}

int Tank::beHit(int power)
{
	m_HP = (m_HP - power <= 0) ? 0 : m_HP - power;
	setAnimation(m_form[m_HP - 1]);
}

void Tank::setFactory(TankFactory * factory)
{
	sm_factory = factory;
}
