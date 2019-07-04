#pragma once
#include "../ground/Tank.h"
#include "../ground/Mover.h"
#include "../Event.h"
class Player :
	public Driver
{
public:
	enum PLAYER
	{
		P1,
		P2,
	};
	virtual bool requestFire() override;
	Player(PLAYER p);
	~Player() = default;
	virtual int command(SDL_Point position, Uint32 timestamp, Mover::DIRECTION &direction) override;
private:
	std::map<int, Mover::DIRECTION> m_move;
	int m_A, m_B;
};