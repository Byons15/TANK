#pragma once
#include "..\ground\Tank.h"
#include "../ground/Ground.h"
#include "../Event.h"
#include <array>
#include "Navigator.h"

class AI :
	public Commander
{
public:
	AI(Tank *tank, int level);
	~AI();
	virtual int command(SDL_Point position, Uint32 timestamp, Mover::DIRECTION &direction) override;
	
private:
	virtual bool requestFire() override;
	bool collisionCheck(const SDL_Point & p);
	void newTarget(const SDL_Point &position, Uint32 timestamp);

	Tank *m_tank;
	Navigator m_gps;
	size_t m_viewRange;
};

