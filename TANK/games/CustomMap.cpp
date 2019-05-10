#include "CustomMap.h"
#include "../TANKS.h"

CustomMap::CustomMap(Renderer * renderer)
	:Scene(renderer, {0, 0, MAP_SIZE * GRID_SIZE, MAP_SIZE * GRID_SIZE})
{
	setBackdropColor({0, 0, 0, SDL_ALPHA_OPAQUE});
	
}


CustomMap::~CustomMap()
{
}
