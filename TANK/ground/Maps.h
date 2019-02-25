#pragma once
#include <SDL_rect.h>
#include "../TANKS.h"
#include <array>
#include <vector>

typedef unsigned int uint32_t;

class Maps
{
public:
	static constexpr int TAG_BASE = 1, 
					 	 enemyBindCount = 3, 
					 	 alliesBindCount = 2;
	Maps();
	int loadMaps(int level);
	int createMaps(Uint32 terrainMaxIndex);
	int operator()(int x, int y);
	int setTerrain(int x, int y, uint32_t terrainIndex = 0);
	void initReservePoint();
	const SDL_Point basePosition() {
		return m_basePoint;
	}

	//��ȡ�о�����㡣
	//index��Ҫ��ȡ�ĸ�����ţ���0��ʼ�����ֵ���ܳ���enemyBindCount-1.
	const SDL_Point &enemyBind(int index) {
		return m_enemyBind[index % enemyBindCount];
	}

	//��ȡ�˾�����㡣
	//index��Ҫ��ȡ�ĸ�����ţ���0��ʼ�����ֵ���ܳ���alliesBindCount-1.
	const SDL_Point &alliesBind(int index) {
		return m_alliesBind[index % alliesBindCount];
	}
	void clearMap();
	~Maps();

private:
	void natureMap(Uint32 terrainMaxIndex);
	bool inReservePoint(const SDL_Point &check);
	void smoothPoint(std::array<std::array<int, MAP_SIZE>, MAP_SIZE> & destMap, int x, int y, const std::vector<int> & terrainWeights);

	std::array<std::array<int, MAP_SIZE>, MAP_SIZE> m_map;
	SDL_Point m_enemyBind[enemyBindCount];
	SDL_Point m_alliesBind[alliesBindCount];
	SDL_Point m_basePoint;
};
