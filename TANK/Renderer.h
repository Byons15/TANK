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

	//��ȡSDL��Ⱦ����
	SDL_Renderer *renderer()
	{
		return m_renderer;
	}

	//������Ⱦ���ڡ�
	//�ɹ�����0������-1��ʾ��Ⱦ���Ѿ������˴��ڣ������ٴ����á�
	int setWindow(SDL_Window *window, bool VSync = false);

	//��ȡ��Ⱦ����Ŀ�괰�ڡ�
	SDL_Window *window() {
		return m_window;
	}

	void addScene(Scene *s);

	//���� flip���μ�SDL_RendererFlip
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

