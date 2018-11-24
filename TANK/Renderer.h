#pragma once
#include "Event.h"
#include <set>
#include "SDL\AnimationFactory.h"

struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;
class View;

class Renderer :
	public Event
{
public:
	Renderer(SDL_Window *window, bool VSync = false);
	~Renderer();
	const AnimationFactory &animationFactory() const {
		return m_animationFactory;
	}
	//ªÒ»°SDL‰÷»æ∆˜°£
	SDL_Renderer *renderer()
	{
		return m_renderer;
	}

private:
	virtual void userEventHookProc(const SDL_UserEvent &event);
	void render();
	int renderView(View *view, Uint32 time);

	std::set<View *> m_renderQueue;
	SDL_Renderer *m_renderer;
	AnimationFactory m_animationFactory;
};

