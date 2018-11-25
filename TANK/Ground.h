#pragma once
#include "Scene.h"

#define MAP_SIZE 26
#define GRID_SIZE 40

class Ground :
	public Scene
{
public:
	Ground(Renderer *renderer, const SDL_Color &backdrop);
	~Ground();

private:
	
};

