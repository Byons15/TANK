#include "Maps.h"
#include "../FileLoader.h"

Maps::Maps()
	:m_map{0}
{
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
