#include "Tank.h"
#include "../Scene.h"
#include "../TANKS.h"
#include "../Director.h"

TankFactory *Tank::sm_factory = 0;

Tank::Tank(Ground * ground, int &model, const SDL_Point &position)
	:Spirit(ground), m_stopMoving(false), m_invincible(false), m_model(model), m_ground(ground), m_power(1), m_missileFilling(false)
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

void Tank::fire()
{
	if (m_missileFilling)
		return;
	m_missileFillingTime = SDL_GetTicks();
	m_missileFilling = true;

	SDL_Point missilb = m_position;
	
	switch (m_direction)
	{
	case Mover::UP:
		missilb.x = m_position.x + colSize / 2 - Missile::missileSize / 2;
		missilb.y = m_position.y - Missile::missileSize / 2;
		break;
	case Mover::DOWN:
		missilb.x = m_position.x + colSize / 2 - Missile::missileSize / 2;
		missilb.y = m_position.y + colSize - Missile::missileSize / 2;
		break;
	case Mover::LEFT:
		missilb.x = m_position.x - Missile::missileSize / 2;
		missilb.y = m_position.y + colSize / 2 - Missile::missileSize / 2;
		break;
	case Mover::RIGHT:
		missilb.x = m_position.x + colSize - Missile::missileSize / 2;
		missilb.y = m_position.y + colSize / 2 - Missile::missileSize / 2;
	default:
		break;
	}

	m_ground->addMissile(new Missile(m_ground, this, m_power, missilb, m_direction));
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
		m_stopMoving = false;
		m_mover.endMove();
		return;
	}
	
	//获取坦克覆盖的范围。
	auto gridRect = pixelToGroundRect({ m_position.x, m_position.y, colSize, colSize });
	int lastLenght;

	//根据当前方向获取坦克需要移动到的网格。
	switch (m_direction)
	{
	case Mover::UP:
		lastLenght = m_mover.beginPos().y - (gridRect.y * GRID_SIZE);
		break;
	case Mover::RIGHT:
		lastLenght = ((gridRect.x + gridRect.w - 2) * GRID_SIZE) - m_mover.beginPos().x;
		break;
	case Mover::DOWN:
		lastLenght = ((gridRect.y + gridRect.h - 2) * GRID_SIZE) - m_mover.beginPos().y;
		break;
	case Mover::LEFT:
		lastLenght = m_mover.beginPos().x - (gridRect.x * GRID_SIZE);
		break;
	default:
		break;
	}

	//从当前位置移动到目标网格所在的位置。
	m_mover.setLenght(lastLenght);
	m_stopMoving = true;
}

int Tank::setCommander(Commander * cmder)
{
	m_commander = cmder;
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
	auto result = m_commander->command(m_ground, this, time, direction);

	if (m_missileFilling) {
		if (time - m_missileFillingTime >= 300) {
			m_missileFilling = false;
		}
	}

	//如果有移动命令并且坦克不处于stopMoving
	if (!result && !m_stopMoving) {
		if (direction != m_direction) {
			stopMove();
		}
		else if (!m_mover.state()){
			startMove(direction, time);
		}
	}
	
	if(result == -1 && !m_stopMoving){
		stopMove();
	}

	//处于移动中时，更新坦克位置。
	if (m_mover.state()) {
		auto newPos = m_mover.current(time);
		
		Tank *colDest;
		auto checkResult = m_ground->tankColCheck(this, newPos, &colDest);
		if (!checkResult) {
			m_position = newPos;
		}
		else if (checkResult == -1) {
			stopMove();
		}
		else if(checkResult == -2){
			if (colDest->m_direction != m_direction) {
				stopMove();
			}
			else {
				
			}
		}

		if (!m_mover.state())
			m_stopMoving = false;

	}
	
	//没有移动并且有移动命令时，开始依据命令移动。
	if (!m_mover.state() && !result) {
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
