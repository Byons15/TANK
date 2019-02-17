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
	int move(const SDL_Point &origin, const SDL_Point &dest, unsigned startTimestamp, float speeds);  //¾ÉµÄ
	int move(const SDL_Point &origin, DIRECTION direction, int lenght, unsigned startTimestamp, float speeds);
	int setLenght(int newLenght);
	const SDL_Point &beginPos() const {
		return m_origin;
	}
	SDL_Point current(unsigned timestamp);
	void endMove() {
		m_moving = false;
	}
	bool state() const {
		return m_moving;
	}
	~Mover() = default;
private:
	SDL_Point m_origin, m_dest;
	DIRECTION m_direction;
	float m_cosa, m_sina, m_speeds;
	int m_lenght, m_startTimestamp;
	bool m_moving;
};

