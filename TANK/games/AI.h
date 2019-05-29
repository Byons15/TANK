#pragma once
#include "..\ground\Tank.h"
#include "../Event.h"
#include <array>

class AI :
	public Commander
{
public:
	AI();
	~AI();
	virtual int command(SDL_Point position, Uint32 timestamp, Mover::DIRECTION &direction) override;
	
private:
	virtual bool requestFire() override;
	int findWay(const SDL_Point &p1, const SDL_Point &p2);
	bool collisionCheck(const SDL_Point &p);
	bool borderCheck(const SDL_Point &p);
	void createDirection(const SDL_Point &p1, const SDL_Point &p2, std::array<SDL_Point, 4> &direction);
	Mover::DIRECTION nextDirection();
	void newTarget(Tank *tank, const SDL_Point &position, Uint32 timestamp);

	static std::array<std::array<bool, MAP_SIZE>, MAP_SIZE> sm_visiteSign;
	std::list<SDL_Point> m_path;
	std::list<SDL_Point>::iterator m_currentPathPoint;
	SDL_Point m_nextPoint;
	Mover::DIRECTION m_currentDirection;
	size_t m_viewRange;
};

