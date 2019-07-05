#pragma once
#include "..\Spirit.h"
#include "TankFactory.h"
#include "Mover.h"
#include "../TANKS.h"

class Driver;
class Ground;

class Tank :
	public Spirit
{
public:
	static constexpr int colSize = GRID_SIZE * 2 - 1;
	enum EVENT
	{
		//奖励箱触发。
		//code：奖品
		//data1：攻击者 type : Driver
		//data2：被攻击者，携带奖励箱的tank type : Tank
		BONUSCHEST = 0x300,

		//坦克被击中（不一定摧毁了）。
		//data1：攻击者 type : Driver
		//data2：被攻击者（使用这个指针前请确保坦克还没有被销毁）type : Tank
		ATTACK,

		//坦克被销毁，这个事件一定是跟着Tank::ATTACKTANK事件的。
		//code：被摧毁的坦克的阵营。
		//data1: 攻击者 type : Driver
		//data2：被攻击者。
		//timestamp : 被攻击的坦克型号。
		KILLED,
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

	enum CAMP
	{
		ALLISE,
		ENEMY,
	};
	
	int HP() const {return m_HP;}
	inline const SDL_Point &pixelPosition() const {return m_position;}
	inline SDL_Point position() const {return SDL_Point{m_position.x / GRID_SIZE, m_position.y / GRID_SIZE};}

	//设置坦克位于战场的网格位置。
	int setGroundPosition(const SDL_Point &pos);   
	Mover::DIRECTION direction() const {return m_direction;}
	void fire();
	void reload();

	//开始移动。
	int startMove(Mover::DIRECTION direction, Uint32 time);

	//stopMove会使坦克进入“结束移动”状态，结束移动状态表示坦克自行移动到最近的下一个网格，同时不接受移动操作。
	void stopMove();
	int setDriver(Driver *driver);
	bool moveState() const {return m_mover.state();}
	MODEL model() const {return m_model;}
	Ground *ground() {return m_ground;}
	Driver *driver() { return m_driver; }
	CAMP camp() const { return m_camp; }

	//设置奖励箱
	//rewarde是用户定义的参数，这个参数会在坦克触发奖励箱时传到事件队列，一般用于标识奖励类型，传入0则取消奖励箱。
	//成功返回0， 坦克处在无敌状态则返回-1
	int setRewards(int rewarde);

	//成功返回0， 坦克当前携带有奖励箱则返回-1
	int invincible();

	static int attack(Driver *aggressor, Tank *target, int power);

private: friend class Ground;
	Tank(Ground * ground, CAMP camp, MODEL &model, const SDL_Point &position);
	~Tank();

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
	CAMP m_camp;
	MODEL m_model;
	int m_HP, m_rewarde, m_power;
	float m_speeds, m_defaultSpeeds;
	Mover::DIRECTION m_direction;
	Ground *m_ground;
	Driver *m_driver;
	static TankFactory *sm_factory;
	bool m_invincible, m_reload;
};
 
struct SOCRECARD
{
	int killCount = 0;
	unsigned total = 0;
};
typedef std::map<Tank::MODEL, SOCRECARD> SCORECARDS;

class Driver
{
public:
	Driver();

	//派生类实现此函数以将命令传给坦克，这个函数会在Tank需要命令的时候调用。
	//direction：派生类返回新的移动方向
	//返回值：当前不下达移动命令时返回-1，下达了移动命令时返回0.
	void drive(Tank *tank) { m_tank = tank; }
	virtual bool requestFire() = 0;
	Tank *tank() { return m_tank; }
	void record(Tank::MODEL model);
	unsigned total() { return m_total; }
	void clearScorecards() { m_socrecards.clear(); }
	virtual int command(SDL_Point cuttentPosition, Uint32 timestamp, Mover::DIRECTION &direction) = 0;
	virtual ~Driver() = default;
private:
	Tank *m_tank;
	std::vector<unsigned> sm_scoreTable;
	SCORECARDS m_socrecards;
	unsigned m_total;
};
