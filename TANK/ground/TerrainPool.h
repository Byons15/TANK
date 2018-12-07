#pragma once
#include <vector>
#include "Object.h"
class AnimationFactory;
class Scene;

class TerrainPool
{
public:
	TerrainPool(Scene * scene);
	void update(Uint32 time);
	const Object &terrain(int terrainIndex) {
		return m_pool[terrainIndex];
	}
	const Object &operator[](int index) {
		return m_pool[index];
	}
	size_t size() {
		return m_pool.size();
	}
	~TerrainPool();
private:
	std::vector<Object> m_pool;
	Scene *m_scene;
};

