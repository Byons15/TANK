#pragma once
#include "..\Spirit.h"
#include "TankFactory.h"
#include "Mover.h"
#include "../TANKS.h"

class Commander;
class Ground;

class Tank :
	public Spirit
{
public:
	static constexpr int colSize = GRID_SIZE * 2 - 1;
	enum EVENT
	{
		BONUSCHEST = 0x300,
	};

	enum MODEL
	{
		P1,
		P2,
		AGILITY,
		ARMOURED1,
		ARMOURED2,
		ARMOURED3,
		ORDINARY1,
		ORDINARY2,
	};
	
	int HP() const {
		return m_HP;
	}
	inline const SDL_Point &pixelPosition() const {
		return m_position;
	}
	inline SDL_Point position() const {
		return SDL_Point{m_position.x / GRID_SIZE, m_position.y / GRID_SIZE};
	}

	//设置坦克位于战场的网格位置。
	int setGroundPosition(const SDL_Point &pos);   
	Mover::DIRECTION direction() const {
		return m_direction;
	}
	void fire();
	void reload();

	//开始移动。
	int startMove(Mover::DIRECTION direction, Uint32 time);

	//stopMove会使坦克进入“结束移动”状态，结束移动状态表示坦克自行移动到最近的下一个网格，同时不接受移动操作。
	void stopMove();
	int setCommander(Commander *cmder);
	bool moveState() const {
		return m_mover.state();
	}
	MODEL model() const {
		return m_model;
	}
	Ground *ground() {
		return m_ground;
	}

	//设置奖励箱
	//rewarde是用户定义的参数，这个参数会在坦克触发奖励箱时传到事件队列，一般用于标识奖励类型，传入0则取消奖励箱。
	//成功返回0， 坦克处在无敌状态则返回-1
	int setRewards(int rewarde);

	//成功返回0， 坦克当前携带有奖励箱则返回-1
	int invincible();

private: friend class Ground;
	Tank(Ground * ground, MODEL &model, const SDL_Point &position);
	~Tank();
	int beHit(Tank *aggressor, int power);
	static void setFactory(TankFactory *factory);
	void update(Uint32 time);
	void render();

private:
	bool onGrid();
	void unInvincible();

	std::vector<Animation> m_form;
	Animation m_rewardsForm, m_invincibleForm;
	Mover m_mover;
	SDL_Point m_position;
	SDL_Rect m_lastRect;
	MODEL m_model;
	int m_HP, m_rewarde, m_power, m_killScore, m_score;
	float m_speeds, m_defaultSpeeds;
	Mover::DIRECTION m_direction;
	Ground *m_ground;
	Commander *m_commander;
	static TankFactory *sm_factory;
	bool m_invincible, m_reload;
};
 
class Commander
{
public:
	Commander();

	//派生类实现此函数以将命令传给坦克，这个函数会在Tank需要命令的时候调用。
	//direction：派生类返回新的移动方向
	//返回值：当前不下达移动命令时返回-1，下达了移动命令时返回0.
	int drive(Tank *tank);
	virtual bool requestFire() = 0;
	Tank *tank() {
		return m_tank;
	}
	virtual int command(SDL_Point cuttentPosition, Uint32 timestamp, Mover::DIRECTION &direction) = 0;
	virtual ~Commander() = default;
private:
	Tank *m_tank;
};
