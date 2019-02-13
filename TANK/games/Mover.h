#pragma once
#include <SDL_rect.h>

class Mover
{
public:

	enum DIRECTION
	{
		UP,
		RIGHT = 90,
		DOWN = 180,
		LEFT = 270,
	};

	Mover()
		:m_moving(false) {}
	int move(const SDL_Point &origin, const SDL_Point &dest, unsigned startTimestamp, float speeds);
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
	float m_cosa, m_sina, m_speeds;
	int m_lenght, m_startTimestamp;
	bool m_moving;
};

