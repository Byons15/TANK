#pragma once
#include "../ground/Tank.h"
#include "../ground/Mover.h"
class Player :
	public Commander
{
public:
	enum PLAYER
	{
		P1,
		P2,
	};

	Player(PLAYER p);
	~Player() = default;
	virtual int command(Ground *ground, Tank *tank, Uint32 timestamp, bool &fire, Mover::DIRECTION &direction) override;
	int inputDirection(Mover::DIRECTION *ret);
private:
	std::map<int, Mover::DIRECTION> m_move;
	int m_A, m_B;
};