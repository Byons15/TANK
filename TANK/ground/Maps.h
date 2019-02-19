#pragma once
#include <SDL_rect.h>
#include "../TANKS.h"
#include <array>

typedef unsigned int uint32_t;

class Maps
{
public:
	static constexpr int TAG_BASE = 1, 
					 	 enemyBindCount = 3, 
					 	 alliesBindCount = 2;
	Maps();
	int loadMaps(int level);
	int operator()(int x, int y);
	int setTerrain(int x, int y, uint32_t terrainIndex = 0);
	const SDL_Point basePosition() {
		return m_basePoint;
	}

	//获取敌军复活点。
	//index：要获取的复活点标号，从0开始，最大值不能超过enemyBindCount-1.
	const SDL_Point &enemyBind(int index) {
		return m_enemyBind[index % enemyBindCount];
	}

	//获取盟军复活点。
	//index：要获取的复活点标号，从0开始，最大值不能超过alliesBindCount-1.
	const SDL_Point &alliesBind(int index) {
		return m_alliesBind[index % alliesBindCount];
	}
	void clearMap();
	~Maps();

private:
	std::array<std::array<int, MAP_SIZE>, MAP_SIZE> m_map;
	SDL_Point m_enemyBind[enemyBindCount];
	SDL_Point m_alliesBind[alliesBindCount];
	SDL_Point m_basePoint;
};
