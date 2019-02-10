#pragma once
#include <SDL_rect.h>
#include "../TANKS.h"
#include <array>

typedef unsigned int uint32_t;

class Maps
{
public:
	static constexpr int TAG_BASE = 1;

	Maps();
	int loadMaps(int level);
	int operator()(int x, int y);
	int setTerrain(int x, int y, uint32_t terrainIndex = 0);
	const SDL_Point &enemyRefreshPoint(int index) {
		return m_enemyRefreshPoint[index];
	}
	const SDL_Point &alliesRefreshPoint(int index) {
		return m_alliesRefreshPoint[index];
	}
	void clearMap();
	~Maps();

private:
	std::array<std::array<int, MAP_SIZE>, MAP_SIZE> m_map;
	SDL_Point m_enemyRefreshPoint[3];
	SDL_Point m_alliesRefreshPoint[2];
};
