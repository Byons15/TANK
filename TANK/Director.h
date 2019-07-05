#pragma once
#include <SDL.h>
#include <map>
#include <initializer_list>
#include "Scene.h"
#include <set>

class EventInterface;

class Director
{
	
public:

	struct KeyState
	{
		SDL_Keycode key;
		Uint32 time;
		bool press;
	};

	Director();

	//delete
	Director(const Director &) = delete;
	Director(const Director &&) = delete;
	Director &operator = (const Director &) = delete;
	Director &operator = (const Director &&) = delete;

	virtual ~Director();

	int monitoringKey(SDL_Keycode key);
	int monitoringKey(std::initializer_list<SDL_Keycode> keyList);

	//���ذ��������µ�ʱ���, û�а����򷵻�0
	int keyState(SDL_Keycode key, int *retkeyDownTime = 0);

	//����SDL_User�¼���
	//Ҫ����SDL�¼���ֱ��ʹ��SDL API.
	//����ֵ�μ�SDL_PushEvent
	int userEventTrigger(const SDL_UserEvent &userEvent);

	int exec();
	void quit();

protected:

private: friend class EventInterface;
	
	//����SDL�¼����ӣ�������Ӳ�������SDL_user�¼���Ҫ����user�¼�������setUserEventHook��
	//EventInterface����������ʵ��eventHookProc�Խ��ղ������¼���
	//���óɹ�����0�� ����-1��ʾ�¼������Ѿ������ˣ���ֱ����eventHookProc�д���
	void setEventHook(EventInterface *event){m_eventHook.insert(event);}
	void unsetEventHook(EventInterface *event){m_eventHook.erase(event);}

	//����SDL_user�¼����ӡ�
	//EventInterface����������ʵ��userEventHookProc�Խ��ղ������¼���
	//���óɹ�����0�� ����-1��ʾ�¼������Ѿ������ˣ���ֱ����userEventHookProc�д���
	void setUserEventHook(EventInterface *event){m_userEventHook.insert(event);}
	void unsetUserEventHook(EventInterface *event){m_userEventHook.erase(event);}

private:
	std::map<SDL_Keycode, int> m_monitoringKey;
	std::set<EventInterface *> m_eventHook, m_userEventHook;
};

//����
extern Director *director;