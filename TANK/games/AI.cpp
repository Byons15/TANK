#include "AI.h"
#include <random>
#include "../ground/Ground.h"
#include "../Timer.h"

AI::AI(Tank * tank, int level)
	:m_tank(tank), m_viewRange(level), m_gps(&tank->ground()->colMap())
{
}

AI::~AI()
{
}

int AI::command(SDL_Point position, Uint32 timestamp, Mover::DIRECTION & direction)
{

	if (!m_gps.state())
		newTarget(position, timestamp);

	m_gps.nextPosition(position, direction);

	return 0;
}

bool AI::requestFire()
{
	//TODO::
	return false;
}

bool AI::collisionCheck(const SDL_Point & p)
{
	for (auto x = p.x; x != p.x + 2; ++x) {
		for (auto y = p.y; y != p.y + 2; ++y) {
			if (tank()->ground()->colMap()[x][y])
				return false;
		}
	}
	return true;
}

void AI::newTarget(const SDL_Point & position, Uint32 timestamp)
{
	std::default_random_engine eX(Timer::SDLTimer() + m_tank->pixelPosition().x + position.y);
	std::default_random_engine eY(Timer::SDLTimer() + m_tank->pixelPosition().y + position.x);
	std::uniform_int_distribution<> disX(0, MAP_SIZE - 2);
	std::uniform_int_distribution<> disY(0, MAP_SIZE - 2);
		
	SDL_Point target;
	do {
		do {
			target.x = disX(eX);
			std::srand(Timer::SDLTimer() + m_tank->pixelPosition().y + position.x);
			target.y = std::rand() % (MAP_SIZE - 2);
		} while (!collisionCheck(target));
	} while (m_gps.startNavigation(position, target));
	

}
