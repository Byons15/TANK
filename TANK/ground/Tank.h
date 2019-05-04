#pragma once
#include "..\Spirit.h"
#include "TankFactory.h"
#include "Ground.h"
#include "Mover.h"
#include "../Event.h"

class Commander;

class Tank :
	public Spirit,
	public EventInterface
{
public:
	static constexpr int colSize = GRID_SIZE * 2 - 1;
	enum EVENT
	{
		BONUSCHEST = 0x300,
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

	//开始移动。
	int startMove(Mover::DIRECTION direction, Uint32 time);

	//stopMove会使坦克进入“结束移动”状态，结束移动状态表示坦克自行移动到最近的下一个网格，同时不接受移动操作。
	void stopMove();
	int setCommander(Commander *cmder);
	bool moveState() const {
		return m_mover.state();
	}
	int model() const {
		return m_model;
	}

	//设置奖励箱
	//rewarde是用户定义的参数，这个参数会在坦克触发奖励箱时传到事件队列，一般用于标识奖励类型，传入0则取消奖励箱。
	//成功返回0， 坦克处在无敌状态则返回-1
	int setRewards(int rewarde);

	//成功返回0， 坦克当前携带有奖励箱则返回-1
	int invincible();

private: friend class Ground;
	Tank(Ground * ground, int &model, const SDL_Point &position);
	~Tank();
	int beHit(Tank *aggressor, int power);
	static void setFactory(TankFactory *factory);
	void update(Uint32 time);
	void render();

private:
	bool onGrid();
	void unInvincible();
	virtual void userEventHookProc(const SDL_UserEvent &event) override;

	std::vector<Animation> m_form;
	Animation m_rewardsForm, m_invincibleForm;
	Mover m_mover;
	SDL_Point m_position;
	SDL_Rect m_lastRect;
	int m_HP, m_rewarde, m_model, m_power;
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
	Commander() = default;

	//派生类实现此函数以将命令传给坦克，这个函数会在Tank需要命令的时候调用。
	//direction：派生类返回新的移动方向
	//返回值：当前不下达移动命令时返回-1，下达了移动命令时返回0.
	virtual int command(Ground *ground, Tank *tank, SDL_Point position, Uint32 timestamp, Mover::DIRECTION &direction) = 0;
	virtual ~Commander() = default;
private:
};
