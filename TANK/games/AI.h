#pragma once
#include "..\ground\Tank.h"
#include "../ground/Ground.h"
#include "../Event.h"
#include <array>
#include "Navigator.h"

class Driver
{
public:
	Driver(Tank *tank, int level);
	void run(const SDL_Point &position, Uint32 timestamp, Mover::DIRECTION &outDirection);
private:

	class Action
	{
	public:

		//派生类实现，派生类必须满足返回值的要求。
		//返回0表示动作还没有做完，下一帧继续调用。 
		//返回-1表示动作已经做完，Action可以被delete。
		virtual int action(const SDL_Point position, Uint32 timestamp, Mover::DIRECTION &outDirection) = 0;
	};

	class Stay
		:Action
	{
	public:
		Stay();
		int action(const SDL_Point position, Uint32 timestamp, Mover::DIRECTION &outDirection) override;
	private:
		Uint32 m_startTimestamp;
		Uint32 m_stayTime;
	};

	class Move
		:Action
	{
		Move();
		int action(const SDL_Point position, Uint32 timestamp, Mover::DIRECTION &outDirection) override;
	private:
		Navigator m_gps;
	};
};

class AI :
	public Commander
{
public:
	AI(Tank *tank, int level);
	~AI();
	virtual int command(SDL_Point position, Uint32 timestamp, Mover::DIRECTION &direction) override;
	
private:
	Tank *m_tank;
	int m_level;

private: //driver
	SDL_Point random(const SDL_Rect & range);
	int intoCollimationLine(const SDL_Point &target);
	SDL_Rect createViewRange(const SDL_Point &center, int viewRange);
	int newTarget(const SDL_Rect &viewRange);
	//move
	bool collisionCheck(const SDL_Point & p);
	void newTarget(const SDL_Point &position, Uint32 timestamp);
	Navigator m_gps;
	
	//stay
	Uint32 m_startTimestamp;
	Uint32 m_stayTime;

private: //gunner
	virtual bool requestFire() override;
};
