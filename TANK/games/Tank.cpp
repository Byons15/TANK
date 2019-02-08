#include "Tank.h"
#include "../Scene.h"
#include "../TANKS.h"
#include "../Director.h"

TankFactory *Tank::sm_factory = 0;

Tank::Tank(Ground * ground, int &model, REWARDS rewards = DEFAULT)
	:Spirit(ground), m_rewarde(rewards)
{
	//�ӹ������Ҳ����������
	auto &dat = sm_factory->findTankData(model);
	m_form = dat.form;
	m_rewardsForm = dat.rewardForm;
	m_invincibleForm = dat.invincibleForm;
	m_HP = dat.maxHP;
	m_speeds = dat.speeds;

	//����̹�˵�ǰ��̬��
	beHit(0);
	
	//�Ƿ���н�����
	if (rewards) {
		setAnimation(m_rewardsForm);
	}

	//̹�˵���ײ��СΪ��������-1.
	m_pixelRect.h = m_pixelRect.w = GRID_SIZE * 2 - 1;
}

Tank::~Tank()
{
}

int Tank::beHit(int power)
{
	//���̹��Я���н����䣬�򴥷�����, �����Ǽ�HP��
	if (m_rewarde != DEFAULT) {
		
		//����������
		SDL_UserEvent user;
		user.type = BONUSCHEST;
		user.code = m_rewarde;
		director->userEventTrigger(user);

		//�ָ�����̹ͨ�ˡ�
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
