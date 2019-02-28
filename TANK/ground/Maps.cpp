#include "Maps.h"
#include "../FileLoader.h"
#include <sstream>
#include <random>
#include <SDL_timer.h>

Maps::Maps()
{
	clearMap();
}

int Maps::loadMaps(int level)
{

	clearMap();

	//导入数据。
	std::string groupName = "level " + std::to_string(level);
	std::vector<std::string> data;
	if (fileLoad(groupName, &data) != MAP_SIZE)
		return -1;
	
	m_basePoint = { -1, -1 };
	//读取数据并构造地图。
	size_t enemyRefreshIndex = 0;
	for (int ln = 0; ln != MAP_SIZE; ++ln) {
		std::istringstream is(data[ln]);
		for (int col = 0; col != MAP_SIZE; ++col) {
			is >> m_map[col][ln];

			//导入的数据标识中-1表示P1刷新点， -2表示P2刷新点， -3表示敌军刷新点。
			if (m_map[col][ln] < 0) {
				m_map[col][ln] = 0;

				//将坐标的中的标识赋0，并保存这个敌军刷新点。说一下就是敌军刷新点最多有且必须有3个。
				//赋0表示这个点是空的。
				//之后的两个if语句是保存P1 P2刷新点的。
				if (m_map[col][ln] == -3) 
					m_enemyBind[enemyRefreshIndex++] = { col, ln };

				if (m_map[col][ln] == -1) 
					m_alliesBind[0] = { col, ln };

				if (m_map[col][ln] == -2) 
					m_alliesBind[1] = { col, ln };
			}

			//保存基地位置。
			if (m_basePoint.x == -1 && m_basePoint.y == -1 && m_map[col][ln] == 1) {
				m_basePoint = { col, ln };
			}
		}
	}
	
	

	return 0;
}

int Maps::createMaps(Uint32 terrainMaxIndex)
{
	clearMap();
	
	//简单地随机分布地形。
	std::default_random_engine e(SDL_GetTicks());
	std::uniform_int_distribution<> dis(1, terrainMaxIndex);
	for (size_t x = 0; x != m_map.size(); ++x) {
		for (size_t y = 0; y != m_map[x].size(); ++y) {
			m_map[x][y] = dis(e);
			m_map[x][y] = (m_map[x][y] == TAG_BASE) ? 0 : m_map[x][y];
		}
	}

	//平滑两遍地图。
	natureMap(terrainMaxIndex);
	natureMap(terrainMaxIndex);

	//TODO：：这里调整地图以保证各个刷新点能到达基地的位置。

	//设置基地围墙
	//先把基地的位置上也填充砖块没关系，反正之后会基地会替换它。
	for (auto x = 11; x != 15; ++x) {
		for (auto y = 23; y != 26; ++y) {
			m_map[x][y] = 2;
		}
	}

	//保留刷新点以及基地位置。
	initReservePoint();

	return 0;
}

int Maps::operator()(int x, int y)
{
	if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
		throw std::out_of_range("访问地图越界");

	return m_map[x][y];
}

int Maps::setTerrain(int x, int y, uint32_t terrainIndex)
{
	if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
		throw std::out_of_range("访问地图越界");
	
	m_map[x][y] = terrainIndex;
	return 0;
}

void Maps::initReservePoint()
{
	for (auto x = 0; x != 2; ++x) {
		for (auto y = 0; y != 2; ++y) {

			for (auto &p : m_enemyBind) {
				m_map[p.x + x][p.y + y] = 0;
			}
			for (auto &p : m_alliesBind) {
				m_map[p.x + x][p.y + y] = 0;
			}
			m_map[x + m_basePoint.x][y + m_basePoint.y] = TAG_BASE;
		}
	}
}

void Maps::clearMap()
{
	for (auto &iter : m_map) {
		iter.fill(0);
	}

	//默认坦克刷新点。
	m_enemyBind[0]  = { 0, 0 };
	m_enemyBind[1]  = { 12, 0 };
	m_enemyBind[2]  = { 24, 0 };
	m_alliesBind[0] = { 9, 24 }; 
	m_alliesBind[1] = { 15, 24 };

	//默认基地位置。
	m_map[12][24] = m_map[13][24] = m_map[12][25] = m_map[13][25] = TAG_BASE;
	m_basePoint = { 12, 24 };
}

Maps::~Maps()
{
}

void Maps::natureMap(Uint32 terrainMaxIndex)
{	
	//平滑每个位置。
	std::array<std::array<int, MAP_SIZE>, MAP_SIZE> dest = m_map;
	for (auto x = 0; x != MAP_SIZE; ++x) {
		for (auto y = 0; y != MAP_SIZE; ++y) {
			smoothPoint(dest, x, y, terrainMaxIndex);
		}
	}

	m_map = dest;
}

void Maps::smoothPoint(std::array<std::array<int, MAP_SIZE>, MAP_SIZE> & destMap, int x, int y, int terrainMaxIndex)
{
	std::pair<int, int> much{ 0, 0 };

	//找出九宫格中数量最多的地形。
	for (auto t = 0; t <= terrainMaxIndex; ++t) {
		int weight = 0;

		//遍历以x，y为中心的九宫格。 
		SDL_Rect rect;
		rect.x = (x - 1 < 0) ? x : x - 1;
		rect.y = (y - 1 < 0) ? y : y - 1;
		rect.w = (x + 1 >= MAP_SIZE) ? x : x + 1;
		rect.h = (y + 1 >= MAP_SIZE) ? y : y + 1;
		for (auto i = rect.x; i <= rect.w; ++i) {
			for (auto j = rect.y; j <= rect.h; ++j) {
				if (t == m_map[i][j])
					++weight;
			}
		}

		if (much.second < weight) {
			much = {t, weight };
		}
		else if(much.second == weight){
			std::default_random_engine e(SDL_GetTicks());
			std::uniform_int_distribution<> dis(1, 10);
			if (dis(e) > 5) {
				much = { t, weight };
			}
		}
	}

	much.first = (much.first == 1) ? 0 : much.first;
	destMap[x][y] = much.first;
}

int Maps::navigationToBase(const SDL_Point & beginPos)
{
	
	return 0;
}

int Maps::navigationToPosition(const SDL_Point & src, const SDL_Point & dest)
{
	return 0;
}
