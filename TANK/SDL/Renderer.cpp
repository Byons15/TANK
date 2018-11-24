#include "..\Renderer.h"
#include <SDL_render.h>
#include "../FileLoader.h"
#include <sstream>
#include <SDL_timer.h>
#include "../Games.h"

#define RENDER 0x88181

Renderer::Renderer(SDL_Window * window, bool VSync)
{
	Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
	flags = VSync ? flags | SDL_RENDERER_PRESENTVSYNC : flags;

	m_renderer = SDL_CreateRenderer(window, -1, flags);
	m_animationFactory.createFactory(this);


	SDL_UserEvent user;
	user.type = RENDER;
	games->userEventTrigger(user);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_renderer);
}

void Renderer::userEventHookProc(const SDL_UserEvent & event)
{
	switch (event.type)
	{
	case RENDER:
		break;
	default:
		break;
	}
}