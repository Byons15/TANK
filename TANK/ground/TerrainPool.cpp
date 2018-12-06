#include "TerrainPool.h"
#include "../FileLoader.h"
#include <sstream>

TerrainPool::TerrainPool(const AnimationFactory & animationFactory)
	:m_animationFactory(animationFactory)
{
	std::vector<std::string> terrainPoolData;
	if (!fileLoad("objects", "terrain", &terrainPoolData))
		throw std::runtime_error("构建地形对象池失败");

	for (int i = 1; i <= terrainPoolData.size(); ++i) {
		std::istringstream is(terrainPoolData[i - 1]);
		Terrain t;
		is >> t.name >> t.w >> t.h >> t.tankTransit >> t.HP >> t.bulletTransit;
		t.animation = animationFactory.findAnimation(t.name);
		m_pool.push_back(t);
	}
}

void TerrainPool::update(Uint32 time)
{
	
}

TerrainPool::~TerrainPool()
{
}
