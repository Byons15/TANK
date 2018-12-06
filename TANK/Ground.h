#pragma once
#include "Scene.h"
#include "ground\Maps.h"
#include <vector>
#include "ground\TerrainPool.h"

class Ground :
	public Scene
{
public:
	static constexpr uint32_t GRID_SIZE = 40;
	Ground() = delete;
	Ground(Renderer *renderer, const SDL_Color &backdrop = { 0, 0, 0 });
	~Ground();
protected:
	virtual void update(Uint32 time);
	virtual int render();
private:
	Maps m_maps;
	TerrainPool m_terrainPool;
};

