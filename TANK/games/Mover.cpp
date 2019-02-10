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

SDL_Point Mover::current(unsigned timestamp)
{
	if (!m_moving)
		return m_dest;

	SDL_Point p = m_origin;

	int pixelCount = static_cast<int>((timestamp - m_startTimestamp) * m_speeds);

	if (pixelCount >= m_lenght) {
		m_moving = false;
		return m_dest;
	}

	p.x += static_cast<int>(pixelCount * m_cosa);
	p.y += static_cast<int>(pixelCount * m_sina);

	return p;
}