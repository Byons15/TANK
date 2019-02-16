#pragma once
#include "../ground/Ground.h"
#include "../ground/Tank.h"
#include "Player.h"

class Normal
{
public:
	Normal(Ground *ground);
	~Normal();
private:
	Ground *m_ground;
	Tank *m_P1, *m_P2;
	Player m_P1commander;
};

