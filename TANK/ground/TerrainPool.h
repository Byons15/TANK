#pragma once
#include "../SDL/AnimationFactory.h"
#include <vector>

struct Terrain
{
	std::string name;
	Animation animation;
	uint32_t w = 0, h = 0, HP = 0;
	char tankTransit = 0, bulletTransit = 0;
};

class TerrainPool
{
public:
	TerrainPool(const AnimationFactory &animationFactory);
	void update(Uint32 time);
	~TerrainPool();
private:
	std::vector<Terrain> m_pool;
	const AnimationFactory &m_animationFactory;
};

