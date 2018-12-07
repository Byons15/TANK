#include "TerrainPool.h"
#include "../FileLoader.h"
#include <sstream>
#include "../Scene.h"
#include "../Renderer.h"

TerrainPool::TerrainPool(Scene * scene)
	:m_scene(scene)
{
	std::vector<std::string> terrainPoolData;
	if (!fileLoad("objects", "terrain", &terrainPoolData))
		throw std::runtime_error("构建地形对象池失败");

	for (auto i = 1; i <= terrainPoolData.size(); ++i) {
		std::istringstream is(terrainPoolData[i - 1]);
		std::string name;
		SIZE collisionSize;
		Uint32 height;
		is >> name >> collisionSize.w >> collisionSize.h >> height;
		m_pool.push_back(Object(scene, name, height, collisionSize));
	}
}

void TerrainPool::update(Uint32 time)
{
	for (auto &obj : m_pool) {
		obj.update(time);
	}
}

TerrainPool::~TerrainPool()
{
}
