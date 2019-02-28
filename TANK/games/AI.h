#pragma once
#include "..\ground\Tank.h"
class AI :
	public Commander
{
public:
	AI();
	~AI();
	virtual int command(Ground *ground, Tank *tank, Uint32 timestamp, Mover::DIRECTION &direction) override;
	int navigationToBase(Maps *map, const SDL_Point &beginPos);
private:
	
};

