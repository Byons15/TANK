#pragma once
#include "..\Spirit.h"
#include"Mover.h"
#include <list>

class Ground;
class Tank;
class Driver;

class Missile :
	public Spirit
{
public:

	static constexpr unsigned missileSize = 12, boomSize = 42;

	enum EVENT
	{
		//跑弹开始爆炸。
		//code：被炸目标 见：TARGET
		//data1：炮弹。
		//data2：发射炮弹的tank。
		//timestamp：发生的时间戳。
		BEGIN_BOOM = 0x91222,

		//炮弹结束爆炸
		//code：被炸目标 见：TARGET
		//data1：炮弹。
		//data2：发射炮弹的tank。
		//timestamp：发生的时间戳。
		END_BOOM,
	};

	enum TARGET
	{
		TERRAIN = -1,
		TANK = -2,
		BORDER = -3,
	};

	Missile() = delete;
	int power() {return m_power;}
	const SDL_Point &position() const {return m_position;}
	Driver *driver() { return m_driver; }
	Tank *sender() {return m_sender;}

private: friend class Tank;
		 friend class Ground;
	Missile(Ground *ground, Tank *sender, int power);
	void setDestoryIterator(std::list<Missile *>::iterator iter) {m_destoryIterator = iter;}
	std::list<Missile *>::iterator destoryIterator() {return m_destoryIterator;}
	void update(Uint32 time);
	void render();
private:

	Uint32 m_startTime;
	int m_power;
	SDL_Point m_position;
	Tank *m_sender;
	Driver *m_driver;
	Ground *m_ground;
	Mover m_mover;
	TARGET m_boomTarget;
	std::list<Missile *>::iterator m_destoryIterator;
	bool m_boom, m_destory;
};

