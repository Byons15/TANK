#include "Maps.h"
#include "../FileLoader.h"
#include <sstream>
#include <random>
#include <SDL_timer.h>
#include "Ground.h"
#include "../Timer.h"

Maps::Maps(Ground *ground)
	:m_ground(ground)
{
	//载入地形数据
	std::vector<std::string> data;
	fileLoad("terrain", &data);
	for (auto &s : data) {
		std::istringstream is(s);
		TERRAIN t;
		is >> t.name >> t.tankPass >> t.HP;
		t.spirit.setScene(ground);
		t.spirit.setAnimation(t.name);
		m_terrainPool.push_back(t);
	}

	clearMap();
}

int Maps::createMaps(int level)
{
	if (loadMaps(level) == -1)
		return createMaps();
	return 0;
}

inline void Maps::setMap(int x, int y, int index)
{
	m_map[x][y] = index;
	m_ground->updateColMap(x, y);
}

int Maps::loadMaps(int level)
{

	clearMap();

	//导入数据。
	std::string groupName = "level " + std::to_string(level);
	std::vector<std::string> data;
	if (fileLoad(groupName, &data) != MAP_SIZE)
		return -1;
	
	m_reservePoint[0] = { -1, -1 };
	//读取数据并构造地图。
	size_t enemyRefreshIndex = 0;
	for (int ln = 0; ln != MAP_SIZE; ++ln) {
		std::istringstream is(data[ln]);
		for (int col = 0; col != MAP_SIZE; ++col) {
			int index;
			is >> index;
			//导入的数据标识中-1表示P1刷新点， -2表示P2刷新点， -3表示敌军刷新点。
			if (index < 0) {
				
				//将坐标的中的标识赋0，并保存这个敌军刷新点。说一下就是敌军刷新点最多有且必须有3个。
				//赋0表示这个点是空的。
				//之后的两个if语句是保存P1 P2刷新点的。
				if (index == -3) {
					m_reservePoint[enemyRefreshIndex + alliesBindCount + 1] = { col, ln };
					enemyRefreshIndex++;
				}

				if (index == -1)
					m_reservePoint[1] = { col, ln };

				if (index == -2)
					m_reservePoint[2] = { col, ln };

				setMap(col, ln, 0);
			}
			else {
				setMap(col, ln, index);
			}

			//保存基地位置。
			if (m_reservePoint[0].x == -1 && m_reservePoint[0].y == -1 && m_map[col][ln] == 1) {
				m_reservePoint[0] = { col, ln };
			}
		}
	}
	
	

	return 0;
}

int Maps::createMaps()
{
	clearMap();
	
	//简单地随机分布地形。
	std::default_random_engine e(Timer::SDLTimer());
	std::uniform_int_distribution<> dis(0, m_terrainPool.size());
	for (size_t x = 0; x != m_map.size(); ++x) {
		for (size_t y = 0; y != m_map[x].size(); ++y) {
			m_map[x][y] = dis(e);
			m_map[x][y] = (m_map[x][y] == TAG_BASE) ? 0 : m_map[x][y];
		}
	}

	//平滑两遍地图。
	smoothMap();
	smoothMap();

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

	//更新碰撞模型。
	for (auto x = 0; x != m_map.size(); ++x) {
		for (auto y = 0; y != m_map[x].size(); ++y) {
			setMap(x, y, m_map[x][y]);
		}
	}

	return 0;
}

void Maps::destoryBase()
{
	for (auto x = m_reservePoint[0].x; x != m_reservePoint[0].x + 2; ++x) {
		for (auto y = m_reservePoint[0].y; y != m_reservePoint[0].y + 2; ++y) {
			setMap(x, y, 0);
		}
	}
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
	
	for (auto &p : m_reservePoint) {
		if (x >= p.x  && x < p.x + 2 &&
			y >= p.y && y < p.y + 2)
			return -1;
	}

	setMap(x, y, terrainIndex);

	return 0;
}

void Maps::initReservePoint()
{
	for (auto x = 0; x != 2; ++x) {
		for (auto y = 0; y != 2; ++y) {

			for (auto i = 1; i != m_reservePoint.size(); ++i) {
				m_map[m_reservePoint[i].x + x][m_reservePoint[i].y + y] = 0;
			}

			setMap(x + m_reservePoint[0].x, y + m_reservePoint[0].y, TAG_BASE);
		}
	}
}

void Maps::clearMap()
{
	for (int x = 0; x != m_map.size(); ++x) {
		for (int y = 0; y != m_map[x].size(); ++y) {
			setMap(x, y, 0);
		}
	}

	//默认敌军复活点
	m_reservePoint[3]  = { 0, 0 };
	m_reservePoint[4]  = { 12, 0 };
	m_reservePoint[5]  = { 24, 0 };

	//默认友军复活点
	m_reservePoint[1] = { 9, 24 };
	m_reservePoint[2] = { 15, 24 };

	//默认基地位置。
	m_reservePoint[0] = { 12, 24 };
	setMap(12, 24, TAG_BASE);
	setMap(13, 24, TAG_BASE);
	setMap(12, 25, TAG_BASE);
	setMap(13, 25, TAG_BASE);
}

Maps::~Maps()
{
}

void Maps::smoothMap()
{	
	//平滑每个位置。
	std::array<std::array<int, MAP_SIZE>, MAP_SIZE> dest = m_map;
	for (auto x = 0; x != MAP_SIZE; ++x) {
		for (auto y = 0; y != MAP_SIZE; ++y) {
			smoothPoint(dest, x, y);
		}
	}

	m_map = dest;
}

void Maps::smoothPoint(std::array<std::array<int, MAP_SIZE>, MAP_SIZE> & destMap, int x, int y)
{
	std::pair<int, int> much{ 0, 0 };

	//找出九宫格中数量最多的地形。
	for (size_t t = 0; t <= m_terrainPool.size(); ++t) {
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
			std::default_random_engine e(Timer::SDLTimer());
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
