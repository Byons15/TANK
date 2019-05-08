#include "..\Scene.h"
#include "../Renderer.h"
#include <SDL_render.h>
#include "../Director.h"

Scene::Scene(Renderer *renderer, const SDL_Rect &sceneRect)
	:m_rect(sceneRect), m_renderer(renderer), m_backdrop(0), m_state(false)
{
	m_renderer->addScene(this);
}

Scene::~Scene()
{
	SDL_DestroyTexture(m_backdrop);
}

int Scene::setBackdropColor(const SDL_Color & color)
{
	if (m_renderer) {
		m_backdrop = SDL_CreateTexture(m_renderer->renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_rect.w, m_rect.h);
		SDL_SetRenderTarget(m_renderer->renderer(), m_backdrop);
		SDL_SetRenderDrawColor(m_renderer->renderer(), color.r, color.g, color.b, color.a);
		SDL_Rect fillRect = m_rect;
		fillRect.x = fillRect.y = 0;
		SDL_RenderFillRect(m_renderer->renderer(), &fillRect);
		SDL_SetRenderTarget(m_renderer->renderer(), NULL);
		return 0;
	}

	return -1;
}

void Scene::setRect(const SDL_Rect & rect)
{
	m_rect = rect;
}

int Scene::setState(bool state)
{
	if (!m_renderer) {
		m_state = false;
		return -1;
	}
	m_state = state;

	return 0;
}

SDL_Texture * Scene::backdrop(SDL_Rect * rect)
{
	*rect = m_rect;
	return m_backdrop;
}