#include "..\Renderer.h"
#include "../FileLoader.h"
#include <sstream>
#include <SDL_timer.h>
#include <SDL_render.h>
#include "../Games.h"
#include "../Scene.h"

Renderer::Renderer(SDL_Window * window, bool VSync)
	:m_rending(false)
{
	Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
	flags = VSync ? flags | SDL_RENDERER_PRESENTVSYNC : flags;

	m_renderer = SDL_CreateRenderer(window, -1, flags);
	m_animationFactory.createFactory(this);

	SDL_GetWindowSize(window, (int *)&m_windowOriginalSize.w, (int *)&m_windowOriginalSize.h);
	m_windowWidthRatio = m_windowHeightRatio = 1;
	games->setEventHook(this, SDL_WINDOWEVENT);

	games->setUserEventHook(this, RENDER);
	SDL_UserEvent user;
	user.type = RENDER;
	games->userEventTrigger(user);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_renderer);
}

void Renderer::addScene(Scene * s)
{
	m_renderQueue.insert(s);
}

void Renderer::renderTexture(SDL_Texture * texture, const SDL_Rect & destRect, const SDL_Rect & srcRect, Uint32 angle, Uint8 alpha, int flip)
{
	if (!texture)
		return;

	if (alpha != SDL_ALPHA_OPAQUE) 
		SDL_SetTextureAlphaMod(texture, alpha);
	

	SDL_Rect rect = destRect;
	rect.x = (int)((float)rect.x * m_windowWidthRatio);
	rect.y = (int)((float)rect.y * m_windowHeightRatio);
	rect.w = (int)((float)rect.w * m_windowWidthRatio);
	rect.h = (int)((float)rect.h * m_windowHeightRatio);

	SDL_RenderCopyEx(m_renderer, texture, &srcRect, &rect, angle, NULL, (SDL_RendererFlip)flip);

	if (alpha != SDL_ALPHA_OPAQUE) 
		SDL_SetTextureAlphaMod(texture, SDL_ALPHA_OPAQUE);
}

void Renderer::userEventHookProc(const SDL_UserEvent & event)
{
	switch (event.type)
	{
	case RENDER:
		render();
		break;
	default:
		break;
	}
}

void Renderer::eventHookProc(const SDL_Event & event)
{
	switch (event.type)
	{
	case SDL_WINDOWEVENT:
		if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED && event.window.windowID == SDL_GetWindowID(m_window)) {
			m_windowHeightRatio = (float)event.window.data2 / (float)m_windowOriginalSize.h;
			m_windowWidthRatio = (float)event.window.data1 / (float)m_windowOriginalSize.w;
		}
		break;
	default:
		break;
	}
}

void Renderer::render()
{
	auto time = SDL_GetTicks();

	SDL_RenderClear(m_renderer);
	
	for (auto &s : m_renderQueue) {
		if (s->showState()) {

			SDL_Rect destRect;
			SDL_Texture *backdrop = s->backdrop(&destRect);
			SDL_Rect srcRect = destRect;
			srcRect.x = srcRect.y = 0;
			if (backdrop) {
				renderTexture(backdrop, destRect, srcRect, 0);
			}

			s->update(time);
			s->render();
		}
	}
	SDL_RenderPresent(m_renderer);
}
