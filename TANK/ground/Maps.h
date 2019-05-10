#pragma once
#include <SDL_rect.h>
#include "../TANKS.h"
#include <array>
#include <vector>
#include "../Spirit.h"

typedef unsigned int uint32_t;
class Ground;

class Maps
{
public:
	static constexpr int TAG_BASE = 1,
		enemyBindCount = 3,
		alliesBindCount = 2,
		TERRAIN_CHANGE = 0x2223;
	
	struct TERRAIN
	{
		std::string name;
		Spirit spirit;
		int tankPass, HP;
	};

	Maps() = delete;
	Maps(const Maps&) = delete;
	Maps(const Maps&&) = delete;
	Maps& operator = (const Maps &) = delete;
	Maps& operator = (const Maps &&) = delete;

	int operator()(int x, int y);
	int setTerrain(int x, int y, uint32_t type = 0);
	int createMaps(int level);

	TERRAIN &terrainData(size_t index) {
		return m_terrainPool[index];
	}
	size_t terrainTypeMember() {
		return m_terrainPool.size() + 1;
	}

	const SDL_Point basePosition() {
		return m_reservePoint[0];
	}

	//��ȡ�о�����㡣
	//index��Ҫ��ȡ�ĸ�����ţ���0��ʼ�����ֵ���ܳ���enemyBindCount-1.
	const SDL_Point &enemyBindPosition(int index) {
		return m_reservePoint[index + alliesBindCount + 1];
	}

	//��ȡ�˾�����㡣
	//index��Ҫ��ȡ�ĸ�����ţ���0��ʼ�����ֵ���ܳ���alliesBindCount-1.
	const SDL_Point &alliesBindPosition(int index) {
		return m_reservePoint[index + 1];
	}
	void clearMap();
	~Maps();

private: friend class Ground;
	Maps(Ground *ground);
	void destoryBase();
	std::vector<TERRAIN> m_terrainPool;

private:
	//����λ���ϵĵ��Σ����������setTerrain��ͬ����setMapû�м�鱣�����ֱ��ǿ�����ã���������ground����ײӳ�䡣
	void setMap(int x, int y, int index);
	int loadMaps(int level);
	int createMaps();
	void initReservePoint();
	void smoothMap();
	void smoothPoint(std::array<std::array<int, MAP_SIZE>, MAP_SIZE> & destMap, int x, int y);
	int navigationToBase(const SDL_Point &beginPos);
	int navigationToPosition(const SDL_Point &src, const SDL_Point &dest);

	std::array<std::array<int, MAP_SIZE>, MAP_SIZE> m_map;
	std::array<SDL_Point, 1 + alliesBindCount + enemyBindCount> m_reservePoint;
	
	Ground *m_ground;
};
