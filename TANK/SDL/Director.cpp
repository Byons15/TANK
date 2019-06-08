#include "..\Director.h"
#include <SDL_ttf.h>
#include "../Event.h"
#include <stdexcept>
#include "../Timer.h"
#include <SDL_image.h>

#define USER_EVENT 0x4000

Director *director = nullptr;

//Ϊ��ֹͷ�ļ������������ֻ�����εؽ������ʱ����ָ��ķ�ʽ�������
Timer *timer;   

Director::Director()
{
	if (director)
		throw std::runtime_error("Director��ֻ��ͬʱ��Ψһʵ��");
	else
		director = this;

	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER);
	TTF_Init();

	timer = new Timer;
	timer->start();
}

Director::~Director()
{
	delete timer;
	TTF_Quit();
	SDL_Quit();
}

int Director::monitoringKey(SDL_Keycode key)
{
	m_monitoringKey[key] = -1;
	return 0;
}

int Director::monitoringKey(std::initializer_list<SDL_Keycode> keyList)
{
	for (auto key : keyList) {
		Director::monitoringKey(key);
	}
	return 0;
}

int Director::keyState(SDL_Keycode key, int *retkeyDownTime)
{
	auto iter = m_monitoringKey.find(key);
	if (iter == m_monitoringKey.end())
		return 0;
		
	if (iter->second == -1)
		return 0;

	if (retkeyDownTime)
		*retkeyDownTime = Timer::SDLTimer() - iter->second;

	return iter->second;
}


int Director::userEventTrigger(const SDL_UserEvent & user)
{
	SDL_Event event;
	event.user = user;
	event.user.type = event.type = SDL_USEREVENT;
	event.user.code = USER_EVENT;
	event.user.data1 = new SDL_UserEvent(user);
	
	return SDL_PushEvent(&event);
}

int Director::exec()
{
	SDL_Event event;
	bool quit = false;
	while (!quit) {
		if (SDL_WaitEvent(&event) != 1)
			continue;

		switch (event.type)
		{
		case SDL_USEREVENT:
		{
			switch (event.user.code)
			{
			case USER_EVENT:
			{
				//�ַ��û��¼���
				for (auto iter = m_userEventHook.begin(); iter != m_userEventHook.end();) {
					auto temp = iter++;
					(*temp)->userEventHookProc(*((SDL_UserEvent *)event.user.data1));
				}
			}
			}
		}
		break;

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			auto iter = m_monitoringKey.find(event.key.keysym.sym);
			if (iter != m_monitoringKey.end()) {
				
				//key down
				if (event.type == SDL_KEYDOWN) {
					if (iter->second == -1) {   //ֻ��¼��һ��KeyDown�¼�����֤��Աtime�ǵ�һ��KeyDown�¼���ʱ�����
						iter->second = event.key.timestamp;
					}
				}
				//key up
				else {
					iter->second = -1;
				}
			}
		}
		//�����¼��ַ���

		default:
		{
			//�ַ�SDL�¼���
			for (auto iter = m_eventHook.begin(); iter != m_eventHook.end();) {
				auto temp = iter++;
				(*temp)->eventHookProc(event);
			}

			break;
		}
		}
	}

	return 0;
}

void Director::quit()
{
	SDL_Event event;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
}