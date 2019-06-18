#include "AI.h"
#include <random>
#include "../ground/Ground.h"
#include "../Timer.h"
#include "../Games.h"



AI::AI(Tank * tank, int level)
	:m_tank(tank), m_level(level), m_gps(&tank->ground()->colMap())
{
}

AI::~AI()
{
}

int AI::command(SDL_Point position, Uint32 timestamp, Mover::DIRECTION & direction)
{
	int viewRange = (m_level >= 25 ? 25 : m_level) / 5 + 5;

	auto rect = createViewRange(position, viewRange);
	

	//TODO::reserve;
	switch (m_tank->model())
	{
	case Tank::AGILITY:
		break;
	case Tank::ARMOURED1:
	case Tank::ARMOURED2:
	case Tank::ARMOURED3:
		break;
	case Tank::ORDINARY1:
	case Tank::ORDINARY2:
		break;
	default:
		break;
	}

	return 0;
}

bool AI::requestFire()
{
	//TODO::
	return false;
}

SDL_Point AI::random(const SDL_Rect & range)
{
	std::default_random_engine eX(Timer::current() + m_tank->pixelPosition().x + range.x / range.y);
	std::uniform_int_distribution<> disX(range.x, range.x + range.w - 1);
	std::random_device rd;  // 将用于为随机数引擎获得种子
	std::mt19937 gen(rd()); // 以播种标准 mersenne_twister_engine
	std::uniform_int_distribution<> disY(range.y, range.y + range.h - 1);
	return SDL_Point{ disX(eX), disY(gen)};
}

SDL_Rect AI::createViewRange(const SDL_Point & center, int viewRange)
{
	SDL_Rect rect;
	rect.x = (center.x - viewRange/2 + 1 < 0) ? 0 : center.x - viewRange/2 + 1;
	rect.y = (center.y - viewRange/2 + 1 < 0) ? 0 : center.y - viewRange/2 + 1;
	rect.w = (rect.x + viewRange > MAP_SIZE - 1) ? MAP_SIZE - 1 - rect.x : viewRange;
	rect.h = (rect.y + viewRange > MAP_SIZE - 1) ? MAP_SIZE - 1 - rect.y : viewRange;
	return rect;
}

int AI::newTarget(const SDL_Rect & range)
{
	for (auto x = range.x; x != range.x + range.w; ++x) {
		for (auto y = range.y; y != range.y + range.h; ++y) {
			if (m_tank->ground()->colMap()[x][y] == Maps::TAG_BASE) { 
				if (!intoCollimationLine({ x, y }))
					return 0;
			}
		}
	}

	std::vector<Tank *> result;
	if (m_tank->ground()->findTankOnRect(m_tank, range, result) != 0) {
		for (auto &t : result) {
			Games *g = (Games *)director;
			if (((Games *)director)->camp(t) != ((Games *) director)->camp(m_tank))
				if (!intoCollimationLine(t->position))
					return 0;
		}
	}

	return -1;
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
	std::uniform_int_distribution<> disX(0, MAP_SIZE - 2);

	SDL_Point target;
	do {
		do {
			target.x = disX(eX);
			std::srand(Timer::SDLTimer() + m_tank->pixelPosition().y + position.x);
			target.y = std::rand() % (MAP_SIZE - 2);
		} while (!collisionCheck(target));
	} while (m_gps.startNavigation(position, target));
	

}
