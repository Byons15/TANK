#include "Navigator.h"

ROAD Navigator::sm_visiteSign;
Navigator *firstOnly = nullptr;

Navigator::Navigator(const ROAD * road)
	:m_road(road)
{
	if (!firstOnly) {
		for (auto &l : sm_visiteSign) {
			for (auto &b : l) {
				b = false;
			}
		}
		firstOnly = this;
	}

	m_currentPosition = m_path.end();
}

int Navigator::navigation(const SDL_Point & p1, const SDL_Point & p2)
{
	if (!m_road)
		return -2;

	resetNavigator();
	createpreferredDirection(p1, p2);
	auto result = findWay(p1, p2);
	if (!result) {
		m_currentPosition = m_path.begin();
	}

	return result;
}

int Navigator::navigationToCollimationLine(const SDL_Point & p1, const SDL_Point & CollimationLine)
{
	if (!m_road)
		return -2;

	resetNavigator();
	createpreferredDirection(p1, CollimationLine);
	
	auto result = findLine(p1, CollimationLine);
	if (!result) {
		m_currentPosition = m_path.begin();
	}

	return result;
}

void Navigator::resetNavigator()
{
	m_path.clear();
}

int Navigator::findLine(const SDL_Point & p1, const SDL_Point & line)
{
	if ((p1.x == line.x || p1.y == line.y) && collisionCheck(p1)) {
		m_path.push_front(line);
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

	auto result = -1;
	for (auto &d : m_preferredDirection) {
		sm_visiteSign[p1.x][p1.y] = true;
		result = findLine({ p1.x + d.x, p1.y + d.y }, line);
		sm_visiteSign[p1.x][p1.y] = false;
		if (result == 0) {
			m_path.push_front(p1);
			return 0;
		}
	}

	return result;
}

int Navigator::findWay(const SDL_Point & p1, const SDL_Point & p2)
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

	auto result = -1;
	for (auto &d : m_preferredDirection) {
		sm_visiteSign[p1.x][p1.y] = true;
		result = findWay({ p1.x + d.x, p1.y + d.y }, p2);
		sm_visiteSign[p1.x][p1.y] = false;
		if (result == 0) {
			m_path.push_front(p1);
			return 0;
		}
	}

	return result;
}

bool Navigator::collisionCheck(const SDL_Point & p)
{
	for (auto x = p.x; x != p.x + 2; ++x) {
		for (auto y = p.y; y != p.y + 2; ++y) {
			if ((*m_road)[x][y])
				return false;
		}
	}
	return true;
}

bool Navigator::borderCheck(const SDL_Point & p)
{
	//边界检查
	if (p.x < 0 || p.y < 0 || p.x + 2 > MAP_SIZE || p.y + 2 > MAP_SIZE)
		return false;

	return true;
}

void Navigator::createpreferredDirection(const SDL_Point & p1, const SDL_Point & p2)
{
	if (SDL_abs(p2.x - p1.x) > SDL_abs(p2.y - p1.y)) {
		if (p2.x - p1.x > 0) {
			m_preferredDirection[0] = { 1,  0 };
			m_preferredDirection[3] = { -1, 0 };
		}
		else {
			m_preferredDirection[3] = { 1,  0 };
			m_preferredDirection[0] = { -1, 0 };
		}

		if (p2.y - p1.y > 0) {
			m_preferredDirection[1] = { 0,  1 };
			m_preferredDirection[2] = { 0, -1 };
		}
		else {
			m_preferredDirection[2] = { 0,  1 };
			m_preferredDirection[1] = { 0, -1 };
		}
	}
	else {
		if (p2.x - p1.x > 0) {
			m_preferredDirection[1] = { 1,  0 };
			m_preferredDirection[2] = { -1, 0 };
		}
		else {
			m_preferredDirection[2] = { 1,  0 };
			m_preferredDirection[1] = { -1, 0 };
		}

		if (p2.y - p1.y > 0) {
			m_preferredDirection[0] = { 0,  1 };
			m_preferredDirection[3] = { 0, -1 };
		}
		else {
			m_preferredDirection[3] = { 0,  1 };
			m_preferredDirection[0] = { 0, -1 };
		}
	}
}

int Navigator::nextPosition(const SDL_Point & current, Mover::DIRECTION & outDirection)
{
	if (m_path.empty())
		return -1;

	if (current.x == m_currentPosition->x && current.y == m_currentPosition->y) {

		const SDL_Point &p1 = current;
		if (++m_currentPosition == m_path.end()) {
			resetNavigator();
			return -1;
		}
		else {
			const SDL_Point &p2 = *m_currentPosition;
			//走到下一个位置应该往哪个方向走。
			if (p2.x - p1.x != 0) {
				if (p2.x - p1.x < 0)
					outDirection = m_currentDirection = Mover::LEFT;
				else
					outDirection = m_currentDirection = Mover::RIGHT;
			}
			else if (p2.y - p1.y != 0) {
				if (p2.y - p1.y < 0)
					outDirection = m_currentDirection = Mover::UP;
				else
					outDirection = m_currentDirection = Mover::DOWN;
			}
		}
	}
	else {
		outDirection = m_currentDirection;
		//TODO:: 这里可能需要添加一份处理tank脱离路线的代码。
	}

	
	return 0;
}

Navigator::~Navigator()
{
}

void Navigator::createDirection(const SDL_Point & p1, const SDL_Point & p2)
{
	if (SDL_abs(p2.x - p1.x) > SDL_abs(p2.y - p1.y)) {
		if (p2.x - p1.x > 0) {
			m_preferredDirection[0] = { 1,  0 };
			m_preferredDirection[3] = { -1, 0 };
		}
		else {
			m_preferredDirection[3] = { 1,  0 };
			m_preferredDirection[0] = { -1, 0 };
		}

		if (p2.y - p1.y > 0) {
			m_preferredDirection[1] = { 0,  1 };
			m_preferredDirection[2] = { 0, -1 };
		}
		else {
			m_preferredDirection[2] = { 0,  1 };
			m_preferredDirection[1] = { 0, -1 };
		}
	}
	else {
		if (p2.x - p1.x > 0) {
			m_preferredDirection[1] = { 1,  0 };
			m_preferredDirection[2] = { -1, 0 };
		}
		else {
			m_preferredDirection[2] = { 1,  0 };
			m_preferredDirection[1] = { -1, 0 };
		}

		if (p2.y - p1.y > 0) {
			m_preferredDirection[0] = { 0,  1 };
			m_preferredDirection[3] = { 0, -1 };
		}
		else {
			m_preferredDirection[3] = { 0,  1 };
			m_preferredDirection[0] = { 0, -1 };
		}
	}
}
