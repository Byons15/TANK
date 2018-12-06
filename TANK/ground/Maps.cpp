#include "Maps.h"
#include "../FileLoader.h"
#include <sstream>

Maps::Maps()
	:m_map{0}
{
}

int Maps::loadMaps(int level)
{

	clearMap();

	//导入数据。
	std::string groupName = "level " + std::to_string(level);
	std::vector<std::string> data;
	if (fileLoad("level", groupName, &data) != MAP_SIZE)
		return -1;
	
	
	//读取数据并构造地图。
	size_t enemyRefreshIndex = 0;
	for (int ln = 0; ln != MAP_SIZE; ++ln) {
		std::istringstream is(data[ln]);
		for (int col = 0; col != MAP_SIZE; ++col) {
			is >> m_map[col][ln];

			//导入的数据标识中-1表示P1刷新点， -2表示P2刷新点， -3表示敌军刷新点。
			if (m_map[col][ln] < 0) {
				m_map[col][ln] = 0;

				//将坐标的中的标识赋0，并保存这个敌军刷新点。说一下就是敌军刷新点最多有且必须有3个，多了溢出报错， 
				//赋0表示这个点是空的。
				//之后的两个if语句是保存P1 P2刷新点的。
				if (m_map[col][ln] == -3) 
					m_enemyRefreshPoint[enemyRefreshIndex++] = { col, ln };

				if (m_map[col][ln] == -1) 
					m_alliesRefreshPoint[0] = { col, ln };

				if (m_map[col][ln] == -2) 
					m_alliesRefreshPoint[0] = { col, ln };
			}
		}
	}
	
	

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

void Maps::clearMap()
{

}

Maps::~Maps()
{
}
