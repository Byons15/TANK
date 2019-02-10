#include "Tank.h"
#include "../Scene.h"
#include "../TANKS.h"
#include "../Director.h"

TankFactory *Tank::sm_factory = 0;

Tank::Tank(Ground * ground, int &model, const SDL_Point &position, REWARDS rewards = DEFAULT)
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

	m_pixelRect.x = position.x;
	m_pixelRect.y = position.y;

	//̹�˵���ײ��СΪ��������-1.
	m_pixelRect.h = m_pixelRect.w = GRID_SIZE * 2 - 1;
}

Tank::~Tank()
{
}

SDL_Point Tank::position() const
{
	return SDL_Point{ m_pixelRect.x, m_pixelRect.y };
}

int Tank::setPosition(const SDL_Point & pos)
{
	//�ȳ�����ground����λ�ã� ����ɹ�������̹��λ�á�
	auto result = m_ground->tankPositionUpdate(this, pos);
	if (!result) {
		m_pixelRect.x = pos.x;
		m_pixelRect.y = pos.y;
	}

	return result;
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

void Tank::update(Uint32 time)
{
	
}
