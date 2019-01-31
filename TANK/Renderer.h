#pragma once
#include "Event.h"
#include <set>
#include "SDL\AnimationFactory.h"

struct SDL_Window;
struct SDL_Renderer;
class Scene;

class Renderer :
	public EventInterface
{
public:

	static constexpr int RENDER = 0x88181;

	Renderer(SDL_Window *window, bool VSync = false);
	~Renderer();

	Renderer();

	const AnimationFactory &animationFactory() const {
		return m_animationFactory;
	}

	//获取SDL渲染器。
	SDL_Renderer *renderer()
	{
		return m_renderer;
	}

	//设置渲染窗口。
	//成功返回0，返回-1表示渲染器已经设置了窗口，不可再次设置。
	int setWindow(SDL_Window *window, bool VSync = false);

	//获取渲染器的目标窗口。
	SDL_Window *window() {
		return m_window;
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
	float m_windowWidthScale, m_windowHeightScale;
	bool m_rending;
};

