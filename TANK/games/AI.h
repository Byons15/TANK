#pragma once
#include "..\ground\Tank.h"
#include "../ground/Ground.h"
#include "../Event.h"
#include <array>
#include "Navigator.h"
#include <random>

class Driver
{
private:
	class Action
	{
	public:

		//派生类实现，派生类必须满足返回值的要求。
		//返回0表示动作还没有做完，下一帧继续调用。 
		//返回-1表示动作已经做完，Action可以被delete。
		virtual int action(const SDL_Point position, Uint32 timestamp, Mover::DIRECTION &outDirection) = 0;
	};

public:
	Driver(Tank *tank, int level);
	void run(const SDL_Point &position, Uint32 timestamp, Mover::DIRECTION &outDirection);

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

private:
	
};

class AI :
	public Commander,
	public EventInterface
{
public:
	AI(Tank *tank, int level);
	~AI();
	virtual int command(SDL_Point position, Uint32 timestamp, Mover::DIRECTION &direction) override;
	
private:
	Tank *m_tank;
	int m_level;

private: //driver

#pragma region OldCode
	SDL_Point random(const SDL_Rect & range);
	SDL_Rect createViewRange(const SDL_Point &center, int viewRange);
	int newTarget(const SDL_Point &current, const SDL_Rect &viewRange);

	//move
	bool collisionCheck(const SDL_Point & p);
	void newTarget(const SDL_Point &position, Uint32 timestamp);
	Navigator m_gps;

#pragma endregion

private: //gunner
	virtual void userEventHookProc(const SDL_UserEvent &event);
	static Uint32 fireTimerCallback(Uint32 interval, void *param);
	
	virtual bool requestFire() override;
	int m_timerID, m_fireIntervalMin, m_fireIntervalMax;
};
