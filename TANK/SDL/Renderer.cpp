#include "..\Renderer.h"
#include "../FileLoader.h"
#include <SDL_timer.h>
#include <SDL_render.h>
#include "../Scene.h"
#include "../Director.h"

Renderer::Renderer(SDL_Window * window, bool VSync)
	:m_window(window)
{
	if (m_renderer)
		return;

	//�Ӵ��ڴ�����Ⱦ�������ݲ��������Ƿ�����ֱͬ����
	Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
	flags = VSync ? flags | SDL_RENDERER_PRESENTVSYNC : flags;

	m_renderer = SDL_CreateRenderer(window, -1, flags);
	if (!m_renderer)
		return;

	//���������������Ⱦ������������
	m_animationFactory.createFactory(this);

	//��ȡ���ڲ�hook���ڴ�С�����¼���ʹ���յ���Ⱦ������Ӧ���ڡ�
	SDL_GetWindowSize(window, (int *)&m_windowOriginalSize.w, (int *)&m_windowOriginalSize.h);
	m_windowWHScale = (float)m_windowOriginalSize.h / (float)m_windowOriginalSize.w;
	m_windowWidthScale = m_windowHeightScale = 1;

	setEventHook(SDL_WINDOWEVENT);

	//����Ⱦ���������¼����С�
	setUserEventHook(RENDER);
	SDL_UserEvent user;
	user.type = RENDER;
	userEventTrigger(user);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_renderer);
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
	
	//ͨ������������������Ⱦ��С��
	SDL_Rect rect = destRect;
	rect.x = (int)((float)rect.x * m_windowWidthScale);
	rect.y = (int)((float)rect.y * m_windowHeightScale);
	rect.w = (int)((float)rect.w * m_windowWidthScale);
	rect.h = (int)((float)rect.h * m_windowHeightScale);

	SDL_RenderCopyEx(m_renderer, texture, &srcRect, &rect, angle, NULL, (SDL_RendererFlip)flip);

	//��ԭ͸���ȣ���Ϊ�����п�������Ԫ�ġ�
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
		//�����ڴ�С�����¼��Ը��ı�����
		if (event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.windowID == SDL_GetWindowID(m_window)) {
			m_windowHeightScale = (float)event.window.data2 / (float)m_windowOriginalSize.h;
			m_windowWidthScale = (float)event.window.data1 / (float)m_windowOriginalSize.w;
			SDL_SetWindowSize(m_window, (int)((float)(event.window.data2 / m_windowWHScale)), (int)((float)(event.window.data2)));
		}
		if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED && event.window.windowID == SDL_GetWindowID(m_window)) {
			m_windowHeightScale = (float)event.window.data2 / (float)m_windowOriginalSize.h;
			m_windowWidthScale = (float)event.window.data1 / (float)m_windowOriginalSize.w;
	//		SDL_SetWindowSize(m_window, (int)((float)(event.window.data2 / m_windowWHScale)), (int)((float)(event.window.data2)));
		}
		break;
	default:
		break;
	}
}

void Renderer::render()
{
	auto time = timer.current();
	
	SDL_RenderClear(m_renderer);

	for (auto &s : m_renderQueue) {
		if (s->state()) {

			//��Ⱦ����������
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
	
	//�ڿ�����ֱͬ��ʱ��SDL_RenderPresent�ڲ��������߳���ͬ��������Ƶ�ʡ�
	SDL_RenderPresent(m_renderer);

}
