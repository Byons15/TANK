#include "Mover.h"
#include <cmath>

int Mover::move(const SDL_Point & origin, const SDL_Point & dest, unsigned startTimestamp, float speeds)
{
	//直线距离。
	m_lenght = static_cast<int>(sqrt((dest.x - origin.x) * (dest.x - origin.x) +
		(dest.y - origin.y) * (dest.y - origin.y)));

	//正余铉比
	m_sina = static_cast<float>(dest.y - origin.y) / static_cast<float>(m_lenght);
	m_cosa = static_cast<float>(dest.x - origin.x) / static_cast<float>(m_lenght);

	m_speeds = speeds;
	m_startTimestamp = startTimestamp;
	m_origin = origin;
	m_dest = dest;

	m_moving = true;
	return 0;
}

int Mover::move(const SDL_Point & origin, DIRECTION direction, int lenght, unsigned startTimestamp, float speeds)
{
	m_dest = origin;
	switch (direction)
	{
	case Mover::UP:
		m_dest.y -= lenght;
		break;
	case Mover::RIGHT:
		m_dest.x += lenght;
		break;
	case Mover::DOWN:
		m_dest.y += lenght;
		break;
	case Mover::LEFT:
		m_dest.x -= lenght;
		break;
	default:
		break;
	}

	m_speeds = speeds;
	m_startTimestamp = startTimestamp;
	m_origin = origin;
	m_lenght = lenght;
	m_direction = direction;
	m_moving = true;

	return 0;
}

int Mover::setLenght(int newLenght)
{
	return move(m_origin, m_direction, newLenght, m_startTimestamp, m_speeds);
}

SDL_Point Mover::current(unsigned timestamp)
{
	if (!m_moving)
		return m_dest;

	if (timestamp <= m_startTimestamp)
		return m_origin;

	SDL_Point p = m_origin;

	int pixelCount = static_cast<int>((timestamp - m_startTimestamp) * m_speeds);

	if (pixelCount >= m_lenght) {
		m_moving = false;
		return m_dest;
	}

	switch (m_direction)
	{
	case Mover::UP:
		p.y -= pixelCount;
		break;
	case Mover::RIGHT:
		p.x += pixelCount;
		break;
	case Mover::DOWN:
		p.y += pixelCount;
		break;
	case Mover::LEFT:
		p.x -= pixelCount;
		break;
	default:
		break;
	}

	return p;
}