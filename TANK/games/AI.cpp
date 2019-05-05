#include "AI.h"
#include <random>
#include <SDL_timer.h>

AI *colMapsUpdater = nullptr;
std::array<std::array<bool, MAP_SIZE>, MAP_SIZE> AI::sm_visiteSign;

AI::AI()
	:m_viewRange(8)  //视距
{
	if (!colMapsUpdater) {
		colMapsUpdater = this;
	}
}


AI::~AI()
{
}

int AI::command(Ground * ground, Tank * tank, SDL_Point position, Uint32 timestamp, Mover::DIRECTION & direction)
{
	m_ground = ground;

	if (m_path.empty()) {
		newTarget(tank, position, timestamp);
		m_currentPathPoint = m_path.begin();
		m_nextPoint = position;
	}

	//检查坦克是否已经走到了下一个位置，因为tank总是在自己认为需要的时候就调用command，并不能保证每次position都是不相同的
	//如果坦克已经走到了下一个位置，则计算新的方向返回给它，否则返回上一次调用时的方向让tank继续前进。
	if (m_nextPoint.x == position.x && m_nextPoint.y == position.y) {
		SDL_Point p1 = *m_currentPathPoint;
		SDL_Point p2;

		//这里检查AI给出的路径是否已经走完了，走完之后就清零路径。否则继续走。
		if ((++m_currentPathPoint) == m_path.end()) {
			m_path.clear();
			return -1;
		}
		else {
			p2 = *m_currentPathPoint;
		}

		//走到下一个位置应该往哪个方向走。
		if (p2.x - p1.x != 0) {
			if (p2.x - p1.x < 0)
				direction = m_currentDirection = Mover::LEFT;
			else
				direction = m_currentDirection = Mover::RIGHT;
		}
		else if (p2.y - p1.y != 0) {
			if (p2.y - p1.y < 0)
				direction = m_currentDirection = Mover::UP;
			else
				direction = m_currentDirection = Mover::DOWN;
		}

		m_nextPoint = *m_currentPathPoint;
	}
	else {
		direction = m_currentDirection;
	}

	return 0;
}

int AI::findWay(const SDL_Point & p1, const SDL_Point & p2)
{
	if (p1.x == p2.x && p1.y == p2.y) {
		m_path.push_front(p2);
		return 0;
	}

	if (!borderCheck(p1))
		return -1;

	if (!collisionCheck(p1)) {
		sm_visiteSign[p1.x][p1.y] = true;
		return -1;
	}

	if (sm_visiteSign[p1.x][p1.y]) {
		return -2;
	}

	std::array<SDL_Point, 4> direction;
	createDirection(p1, p2, direction);
	auto result = -1;
	for (auto &d : direction) {
		sm_visiteSign[p1.x][p1.y] = true;
		result = findWay({p1.x + d.x, p1.y + d.y}, p2);
		sm_visiteSign[p1.x][p1.y] = false;
		if (result == 0) {
			m_path.push_front(p1);
			return 0;
		}
	}

	return result;
}

bool AI::collisionCheck(const SDL_Point & p)
{
	for (auto x = p.x; x != p.x + 2; ++x) {
		for (auto y = p.y; y != p.y + 2; ++y) {
			if (m_ground->colMap()[x][y])
				return false;
		}
	}
	return true;
}

bool AI::borderCheck(const SDL_Point & p)
{
	//边界检查
	if (p.x < 0 || p.y < 0 || p.x + 2 > MAP_SIZE || p.y + 2 > MAP_SIZE)
		return false;

	return true;
}

void AI::createDirection(const SDL_Point & p1, const SDL_Point & p2, std::array<SDL_Point, 4> &direction)
{
	if (SDL_abs(p2.x - p1.x) > SDL_abs(p2.y - p1.y)) {
		if (p2.x - p1.x > 0) {
			direction[0] = { 1,  0 };
			direction[3] = { -1, 0 };
		}
		else {
			direction[3] = { 1,  0 };
			direction[0] = { -1, 0 };
		}

		if (p2.y - p1.y > 0) {
			direction[1] = { 0,  1 };
			direction[2] = { 0, -1 };
		}
		else {
			direction[2] = { 0,  1 };
			direction[1] = { 0, -1 };
		}
	}
	else {
		if (p2.x - p1.x > 0) {
			direction[1] = { 1,  0 };
			direction[2] = { -1, 0 };
		}
		else {
			direction[2] = { 1,  0 };
			direction[1] = { -1, 0 };
		}

		if (p2.y - p1.y > 0) {
			direction[0] = { 0,  1 };
			direction[3] = { 0, -1 };
		}
		else {
			direction[3] = { 0,  1 };
			direction[0] = { 0, -1 };
		}
	}
}

Mover::DIRECTION AI::nextDirection()
{
	SDL_Point p1 = *m_currentPathPoint;
	SDL_Point p2 = *(++m_currentPathPoint);


	if (p2.x - p1.x != 0) {
		if (p2.x - p1.x < 0)
			return Mover::LEFT;
		else
			return Mover::RIGHT;
	}

	if (p2.y - p1.y != 0)
		if (p2.y - p1.y < 0)
			return Mover::UP;
		else
			return Mover::DOWN;

	throw std::runtime_error("AI路线异常");
	return Mover::UP;
}

void AI::newTarget(Tank * tank, const SDL_Point & position, Uint32 timestamp)
{
#define OLDCODE
#ifndef OLDCODE

	for (int x = position.x - m_viewRange / 2; x <= position.x + m_viewRange / 2; ++x) {
		for (int y = position.y - m_viewRange / 2; y <= position.y + m_viewRange / 2; ++y) {
			if (!borderCheck({ x, y }))
				continue;

			if (m_ground->maps().operator()(x, y) == Maps::TAG_BASE)
				if (findWay(position, { x, y }))
					if (!m_path.empty())
						return;
		}
	}

	SDL_Point dest;
	
	std::default_random_engine e(timestamp);
	std::uniform_int_distribution<> disX((position.x - m_viewRange / 2 < 0) ? 0 : position.x - m_viewRange / 2 < 0,
		(position.x + m_viewRange / 2 >= MAP_SIZE - 2) ? MAP_SIZE - 2 : position.x + m_viewRange / 2);
	std::uniform_int_distribution<> disY((position.y - m_viewRange / 2 < 0) ? 0 : position.y - m_viewRange / 2 < 0,
		(position.y + m_viewRange / 2 >= MAP_SIZE - 2) ? MAP_SIZE - 2 : position.y + m_viewRange / 2);

	do {
		dest.x = disX(e);
		dest.y = disY(e);
		while (!collisionCheck(dest)) {
			dest.x = disX(e);
			dest.y = disY(e);
		}
	} while (findWay(position, dest));
#endif // OLDCOE

#ifdef OLDCODE
	std::default_random_engine eX(SDL_GetTicks() + tank->pixelPosition().x + position.y);
	std::default_random_engine eY(SDL_GetTicks() + tank->pixelPosition().y + position.x);
	std::uniform_int_distribution<> disX(0, MAP_SIZE - 2);
	std::uniform_int_distribution<> disY(0, MAP_SIZE - 2);

	SDL_Point target;
	do {
		m_path.clear();
		do {
		//	std::srand(SDL_GetTicks() + tank->pixelPosition().x + position.y);
			target.x = disX(eX);
			std::srand(SDL_GetTicks() + tank->pixelPosition().y + position.x);
			target.y = std::rand() % (MAP_SIZE - 2);
		} while (!collisionCheck(target));
	} while (findWay(position, target));
	
#endif // OLDCODE

}
