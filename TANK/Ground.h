#pragma once
#include "Scene.h"
#include "ground\Maps.h"
#include <vector>

class Ground :
	public Scene
{
public:
	static constexpr uint32_t GRID_SIZE = 40;

	//delete
	Ground() = delete;

	Ground(Renderer *renderer, const SDL_Color &backdrop = { 0, 0, 0 });
	~Ground();
protected:
	virtual void update(Uint32 time) override;
	virtual int render() override;
private:
	Maps m_maps;
};

