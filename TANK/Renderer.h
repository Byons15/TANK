#pragma once
#include "Event.h"
#include <set>
#include "SDL\AnimationFactory.h"

#define RENDER 0x88181

struct SDL_Window;
struct SDL_Renderer;
class Scene;

class Renderer :
	public Event
{
public:
	Renderer(SDL_Window *window, bool VSync = false);
	~Renderer();

	Renderer() = delete;

	const AnimationFactory &animationFactory() const {
		return m_animationFactory;
	}
	//获取SDL渲染器。
	SDL_Renderer *renderer()
	{
		return m_renderer;
	}

	void addScene(Scene *s);

	//参数 flip：参见SDL_RendererFlip
	void renderTexture(SDL_Texture *texture, const SDL_Rect &destRect, const SDL_Rect &srcRect, 
		Uint32 angle, Uint8 alpha = SDL_ALPHA_OPAQUE, int flip = 0);

private:
	virtual void userEventHookProc(const SDL_UserEvent &event) override;
	virtual void eventHookProc(const SDL_Event & event) override;
	void render();

	std::set<Scene *> m_renderQueue;
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	AnimationFactory m_animationFactory;
	SIZE m_windowOriginalSize;
	float m_windowWidthRatio, m_windowHeightRatio;
	bool m_rending;
};

