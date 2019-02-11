#pragma once
#include "Director.h"
#include "Renderer.h"
#include "StartMenu.h"
#include "Event.h"
#include "games\Ground.h"

class Games :
	public Director,
	public EventInterface
{
public:
	Games();
	~Games();

protected:
	virtual void eventHookProc(const SDL_Event &event) override;
	virtual void userEventHookProc(const SDL_UserEvent &user) override;

private:
	SDL_Window *m_window;
	Renderer m_renderer;
	StartMenu m_startMenu;
	Ground m_ground;
};

