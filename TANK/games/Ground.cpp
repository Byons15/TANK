#include "..\Ground.h"
#include "../Renderer.h"

Ground::Ground(Renderer * renderer)
	:Scene(renderer, {0, 0, GRID_SIZE * MAP_SIZE, GRID_SIZE * MAP_SIZE })
{

}

Ground::~Ground()
{
}

void Ground::open(void * data, int code)
{
}

void Ground::close()
{
}

void Ground::update(Uint32 time)
{
}

int Ground::render()
{
}

void Ground::userEventHookProc(const SDL_UserEvent & user)
{
}
