#include "Tank.h"
#include "../Scene.h"
#include "../TANKS.h"
#include "../Director.h"

TankFactory *Tank::sm_factory = 0;

Tank::Tank(Ground * ground, int &model, const SDL_Point &position)
	:Spirit(ground), m_stopMoving(false), m_invincible(false), m_model(model)
{
	//�ӹ������Ҳ����������
	auto &dat = sm_factory->findTankData(model);
	m_form = dat.form;
	m_rewardsForm = dat.rewardForm;
	m_invincibleForm = dat.invincibleForm;
	m_HP = dat.maxHP;
	m_defaultSpeeds = m_speeds = dat.speeds;

	//����̹�˵�ǰ��̬��
	setAnimation(m_form[m_HP - 1]);

	m_position = position;
}

Tank::~Tank()
{
}

int Tank::setPosition(const SDL_Point & pos)
{
	//�ȳ�����ground����λ�ã� ����ɹ�������̹��λ�á�
	Tank *colDset;
	auto result = m_ground->tankColCheck(this, pos, &colDset);
	if (!result) {
		m_position = pos;
	}

	return result;
}

int Tank::startMove(Mover::DIRECTION direction, Uint32 time)
{
	SDL_Point dest = m_position;
	switch (direction)
	{
	case Mover::UP:
		dest.y = 0;
		break;
	case Mover::RIGHT:
		dest.x = GRID_SIZE * MAP_SIZE - GRID_SIZE * 2;
		break;
	case Mover::DOWN:
		dest.y = GRID_SIZE * MAP_SIZE - GRID_SIZE * 2;
		break;
	case Mover::LEFT:
		dest.x = 0;
		break;
	default:
		break;
	}

	m_direction = direction;
	m_mover.move(m_position, dest, time, m_speeds);
	return 0;
}

void Tank::stopMove(Uint32 time)
{
	m_mover.endMove();

	//������������ģ��򲻽��롰�����ƶ�״̬�������򵥵ؽ����ƶ���
	if (onGrid()) {
		m_stopMoving = false;
		return;
	}
	
	//��ȡ̹�˸��ǵķ�Χ��
	auto gridRect = pixelToGroundRect({ m_position.x, m_position.y, colSize, colSize });
	SDL_Point stopPostion;

	//���ݵ�ǰ�����ȡ̹����Ҫ�ƶ���������
	switch (m_direction)
	{
	case Mover::UP:
		stopPostion.x = gridRect.x;
		stopPostion.y = gridRect.y;
		break;
	case Mover::RIGHT:
		stopPostion.x = gridRect.x + gridRect.w - 1;
		stopPostion.y = gridRect.y;
		break;
	case Mover::DOWN:
		stopPostion.x = gridRect.x;
		stopPostion.y = gridRect.y + gridRect.h - 1;
		break;
	case Mover::LEFT:
		stopPostion.x = gridRect.x;
		stopPostion.y = gridRect.y;
		break;
	default:
		break;
	}

	//�ӵ�ǰλ���ƶ���Ŀ���������ڵ�λ�á�
	m_mover.move(m_position, { stopPostion.x * GRID_SIZE, stopPostion. y * GRID_SIZE }, time, m_speeds);
	m_stopMoving = true;  //���롰�����ƶ���״̬��
}

int Tank::setCommander(Commander * cmder)
{
	m_commander = cmder;
	m_stopMoving = true;
	return 0;
}

bool Tank::moveState() const
{
	if (m_stopMoving)
		return false;

	return m_mover.state();
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

	//���̹��Я���н����䣬�򴥷�����, �����Ǽ�HP��
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
		//��ǰ���ǡ������ƶ���״̬������Ҫ�����ƶ�����.

		if (result == -1 || direction != m_direction) {
			stopMove(time);
		}
	}

	if (fire) {
		//TODO::����!!
	}

	//�����ڡ������ƶ���״̬ʱ������ͬ���ǽ����ƶ���״̬һ�������ƶ����и���λ�ã�ֱ���ƶ��������ƶ���

	if (m_mover.state()) {
		SDL_Point p = m_mover.current(time);

		if (m_stopMoving) { //�����ڡ������ƶ���״̬ʱ��������ײ��飬ֱ��������λ�á�
			m_position = p;
			return;
		}

		//�����ײ����Ӧ��
		Tank *colDest;
		auto result = m_ground->tankColCheck(this, p, &colDest);
		if (result == -1) { //�����߽����Ρ�
			stopMove(time);
		}
		else if (result == 0) {
			m_position = p;  //û����������
		}
		else if(result == -2){ //����̹��
			stopMove(time);
			if (colDest->m_direction == m_direction) { //׷β����
				m_speeds = colDest->m_speeds;
				startMove(m_direction, time);
			}
		}
	}
	else { //�ƶ�������ʱ֤��̹���Ѿ��������������ǿ��Կ�ʼִ����һ�������ˡ�
		startMove(direction, time);
	}
}

bool Tank::onGrid()
{
	auto rect = pixelToGroundRect({ m_position.x, m_position.y, colSize, colSize });
	if(rect.w > 2 || rect.h > 2)
		return false;

	return true;
}
