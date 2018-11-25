#include "..\Ground.h"



Ground::Ground(Renderer *renderer, const SDL_Color &backdrop)
	:Scene(renderer, { 0, 0, MAP_SIZE * GRID_SIZE, MAP_SIZE * GRID_SIZE }, &backdrop)
{
}


Ground::~Ground()
{
}
