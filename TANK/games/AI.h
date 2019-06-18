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

		//������ʵ�֣�������������㷵��ֵ��Ҫ��
		//����0��ʾ������û�����꣬��һ֡�������á� 
		//����-1��ʾ�����Ѿ����꣬Action���Ա�delete��
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
