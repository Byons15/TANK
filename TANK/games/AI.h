#pragma once
#include "..\ground\Tank.h"
class AI :
	public Commander
{
public:
	AI();
	~AI();
	virtual int command(Ground *ground, Tank *tank, SDL_Point position, Uint32 timestamp, Mover::DIRECTION &direction) override;
	
private:
	
};

