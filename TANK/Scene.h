#pragma once
#include <SDL_pixels.h>
#include <SDL_rect.h>

class Renderer;
struct SDL_Texture;

class Scene
{
	friend class Renderer;
public:
	Scene() = delete;
	Scene(Renderer *renderer, const SDL_Rect &sceneRect, const SDL_Color *backdrop = 0);
	virtual ~Scene();

	Renderer *renderer() {
		return m_renderer;
	}
	const SDL_Rect &rect() const {
		return m_rect;
	}
	void setShow(bool state) {
		m_showState = state;
	}
	bool showState() {
		return m_showState;
	}
protected:
	virtual void update(Uint32 time) = 0;
	virtual int render() = 0;

	SDL_Texture *backdrop(SDL_Rect *rect);
private:
	SDL_Rect m_rect;
	SDL_Texture *m_backdrop;
	Renderer *m_renderer;
	bool m_showState;
};

