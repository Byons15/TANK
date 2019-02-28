#include "AI.h"



AI::AI()
{
}


AI::~AI()
{
}

int AI::command(Ground * ground, Tank * tank, Uint32 timestamp, Mover::DIRECTION & direction)
{
	if (tank->moveState())
		return 0;
	return 0;
}
