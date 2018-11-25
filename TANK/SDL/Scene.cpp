#include "..\Scene.h"
#include "../Renderer.h"
#include <SDL_render.h>

Scene::Scene(Renderer *renderer, const SDL_Rect &sceneRect, const SDL_Color *backdrop)
	:m_rect(sceneRect), m_renderer(renderer), m_backdrop(0)
{
	if (backdrop) {
		m_backdrop = SDL_CreateTexture(renderer->renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_rect.w, m_rect.h);
		SDL_SetRenderTarget(renderer->renderer(), m_backdrop);
		SDL_SetRenderDrawColor(renderer->renderer(), backdrop->r, backdrop->g, backdrop->b, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer->renderer(), &m_rect);
		SDL_SetRenderTarget(renderer->renderer(), NULL);
	}
}

Scene::~Scene()
{
	SDL_DestroyTexture(m_backdrop);
}

SDL_Texture * Scene::backdrop(SDL_Rect * rect)
{
	*rect = m_rect;
	return m_backdrop;
}
