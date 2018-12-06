#pragma once
#include <SDL_rect.h>

typedef unsigned int uint32_t;

class Maps
{
public:
	static constexpr int MAP_SIZE = 26;

	Maps();
	int loadMaps(int level);
	int operator()(int x, int y);
	int setTerrain(int x, int y, uint32_t terrainIndex = 0);
	void clearMap();
	~Maps();
private:
	int m_map[MAP_SIZE][MAP_SIZE];
	SDL_Point m_enemyRefreshPoint[3];
	SDL_Point m_alliesRefreshPoint[2];
};

