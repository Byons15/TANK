#include "..\Ground.h"
#include "../Renderer.h"


Ground::Ground(Renderer *renderer, const SDL_Color &backdrop)
	:Scene(renderer, { 0, 0, Maps::MAP_SIZE * GRID_SIZE, Maps::MAP_SIZE * GRID_SIZE }, &backdrop),
	m_maps()
{

}


Ground::~Ground()
{
}
