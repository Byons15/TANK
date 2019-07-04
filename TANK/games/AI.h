#pragma once
#include "..\ground\Tank.h"
#include "../ground/Ground.h"
#include "../Event.h"
#include <array>
#include "Navigator.h"
#include <random>

class AI :
	public Driver,
	public EventInterface
{
public:
	AI();
	~AI();
	void setAILevel(int level) { m_level = level; }
	virtual int command(SDL_Point position, Uint32 timestamp, Mover::DIRECTION &direction) override;
	
private:
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
