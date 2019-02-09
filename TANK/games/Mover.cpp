#include "Mover.h"
#include <cmath>

int Mover::move(const SDL_Point & origin, const SDL_Point & dest, unsigned startTimestamp, unsigned speeds)
{
	if (m_moving)
		return -1;

	//直线距离。
	m_lenght = static_cast<int>(sqrt((dest.x - origin.x) * (dest.x - origin.x) +
		(dest.y - origin.y) * (dest.y - origin.y)));

	//cos
	m_cosX = m_lenght / (dest.x - origin.x);
	m_cosY = m_lenght / (dest.y - origin.y);

	//转换速度单位。
	//从p / s 转换成 p / ms
	m_speeds = speeds / 1000;
	m_lastTimestamp = startTimestamp;
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

	int pixelCount = static_cast<int>(static_cast<float>(timestamp - m_lastTimestamp) / m_speeds);
	m_lastTimestamp = m_lastTimestamp - ((timestamp - m_lastTimestamp) - pixelCount * m_speeds);

	if (pixelCount >= m_lenght) {
		return m_dest;
	}

	p.x += static_cast<int>(pixelCount * m_cosX);
	p.y += static_cast<int>(pixelCount * m_cosY);

	return p;
}