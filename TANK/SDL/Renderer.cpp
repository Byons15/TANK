#include "..\Renderer.h"
#include "../FileLoader.h"
#include <SDL_timer.h>
#include <SDL_render.h>
#include "../Scene.h"
#include "../Timer.h"

Renderer::Renderer(SDL_Window * window, bool VSync)
	:m_window(window)
{
	if (m_renderer)
		return;

	//从窗口创建渲染器，依据参数决定是否开启垂直同步。
	Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
	flags = VSync ? flags | SDL_RENDERER_PRESENTVSYNC : flags;

	m_renderer = SDL_CreateRenderer(window, -1, flags);
	if (!m_renderer)
		return;

	//创建适用于这个渲染器的纹理工厂。
	m_animationFactory.createFactory(this);

	//获取窗口并hook窗口大小更改事件，使最终的渲染画面适应窗口。
	SDL_GetWindowSize(window, (int *)&m_windowOriginalSize.w, (int *)&m_windowOriginalSize.h);
	m_windowWHScale = (float)m_windowOriginalSize.h / (float)m_windowOriginalSize.w;
	m_windowWidthScale = m_windowHeightScale = 1;
	m_windowSize = m_windowOriginalSize;

	//将渲染工作加入事件队列。
	SDL_UserEvent user;
	user.type = RENDER;
	userEventTrigger(user);

	installEventHook();
	installUserEventHook();
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_renderer);
	uninstallEventHook();
	uninstallUserEventHook();
}

Renderer::Renderer()
	:m_renderer(0), m_window(0)
{
}

void Renderer::addScene(Scene * s)
{
	m_renderQueue.push_back(s);
}
																																				     
void Renderer::renderTexture(SDL_Texture * texture, const SDL_Rect & destRect, const SDL_Rect & srcRect, Uint32 angle, Uint8 alpha, int flip)
{
	if (!texture)
		return;

	if (alpha != SDL_ALPHA_OPAQUE) 
		SDL_SetTextureAlphaMod(texture, alpha);
	
	//通过比例来调整最终渲染大小。
	SDL_Rect rect = destRect;
	rect.x = (int)((float)rect.x * m_windowWidthScale);
	rect.y = (int)((float)rect.y * m_windowHeightScale);
	rect.w = (int)((float)rect.w * m_windowWidthScale);
	rect.h = (int)((float)rect.h * m_windowHeightScale);

	SDL_RenderCopyEx(m_renderer, texture, &srcRect, &rect, angle, NULL, (SDL_RendererFlip)flip);

	//复原透明度，因为纹理有可能是享元的。
	if (alpha != SDL_ALPHA_OPAQUE) 
		SDL_SetTextureAlphaMod(texture, SDL_ALPHA_OPAQUE);
}

void Renderer::userEventHookProc(const SDL_UserEvent & event)
{
	switch (event.type)
	{
	case RENDER: {
		render();

		SDL_UserEvent user;
		user.type = RENDER;
		userEventTrigger(user);
		break;
	}
	default:
		break;
	}
}

void Renderer::eventHookProc(const SDL_Event & event)
{
	switch (event.type)
	{
	case SDL_WINDOWEVENT:
		//处理窗口大小更改事件以更改比例。
		if (event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.windowID == SDL_GetWindowID(m_window)) {
			m_windowHeightScale = (float)event.window.data2 / (float)m_windowOriginalSize.h;
			m_windowWidthScale = (float)event.window.data1 / (float)m_windowOriginalSize.w;
			if (m_windowSize.w != (int)event.window.data1 && m_windowSize.h == (int)event.window.data2) {
				SDL_SetWindowSize(m_window, (int)event.window.data1, (int)((float)(event.window.data1) * m_windowWHScale));
			}
			else {
				SDL_SetWindowSize(m_window, (int)((float)(event.window.data2 / m_windowWHScale)), (int)(event.window.data2));
			}

			m_windowSize.w = (int)event.window.data1;
			m_windowSize.h = (int)event.window.data2;
		}
		if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED && event.window.windowID == SDL_GetWindowID(m_window)) {
			m_windowHeightScale = (float)event.window.data2 / (float)m_windowOriginalSize.h;
			m_windowWidthScale = (float)event.window.data1 / (float)m_windowOriginalSize.w;
		}
		break;
	default:
		break;
	}
}

void Renderer::render()
{
	auto time = Timer::current();
	
	SDL_RenderClear(m_renderer);

	for (auto &s : m_renderQueue) {
		if (s->state()) {

			//渲染场景背景。
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
	
	//在开启垂直同步时，SDL_RenderPresent内部会阻塞线程以同步监视器频率。
	SDL_RenderPresent(m_renderer);

}
