#pragma once
#include "..\Spirit.h"
#include"Mover.h"

class Ground;
class Tank;

class Missile :
	public Spirit
{
public:

	static constexpr unsigned missileSize = 12, boomSize = 42;

	enum EVENT
	{
		BOOM = 0x91222,
	};

	enum TARGET
	{
		TANK,
		TERRAIN,
		BORDER,
	};

	Missile() = delete;
private: friend class Ground;
	Missile(Ground *ground, Tank *sender, const SDL_Point &beginPos, Mover::DIRECTION direction);
	void update(Uint32 time);
private:
	Uint32 m_startTime;
	SDL_Point m_position;
	Tank *m_sender;
	Mover m_mover;
	TARGET m_boomTarget;
	bool m_boom, m_destory;
};

