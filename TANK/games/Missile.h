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
		BEGIN_BOOM = 0x91222,
		END_BOOM,
	};

	enum TARGET
	{
		TANK = -1,
		TERRAIN = -2,
		BORDER = -3,
	};

	Missile() = delete;
private: friend class Tank;
		 friend class Ground;
	Missile(Ground *ground, Tank *sender, int power, const SDL_Point &beginPos, Mover::DIRECTION direction);
	void update(Uint32 time);
	inline void render();
private:
	Uint32 m_startTime;
	int m_power;
	SDL_Point m_position;
	Tank *m_sender;
	Ground *m_ground;
	Mover m_mover;
	TARGET m_boomTarget;
	bool m_boom, m_destory;
};

