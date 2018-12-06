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

	std::string groupName = "level " + std::to_string(level);
	std::vector<std::string> data;
	if (fileLoad("level", groupName, &data) != MAP_SIZE)
		return -1;
	
	size_t enemyRefreshIndex = 0;
	for (int ln = 0; ln != MAP_SIZE; ++ln) {
		std::istringstream is(data[ln]);
		for (int col = 0; col != MAP_SIZE; ++col) {
			is >> m_map[col][ln];
			if (m_map[col][ln] < 0) {
				m_map[col][ln] = 0;

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
