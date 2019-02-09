#pragma once
#include <SDL_rect.h>

class Mover
{
public:
	Mover() = default;
	int move(const SDL_Point &origin, const SDL_Point &dest, unsigned startTimestamp, unsigned speeds);
	SDL_Point current(unsigned timestamp);
	void endMove() {
		m_moving = false;
	}
	bool state() const {
		m_moving;
	}
	~Mover() = default;
private:
	SDL_Point m_origin, m_dest;
	float m_cosX, m_cosY, m_speeds, m_lenght;
	unsigned m_lastTimestamp;
	bool m_moving;
};

