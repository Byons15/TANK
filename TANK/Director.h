#pragma once
#include <SDL.h>
#include <map>
#include <initializer_list>
#include "Scene.h"

class EventInterface;

class Director
{
	friend class EventInterface;
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

	//���ذ��������µ�ʱ���(�ӳ����ʼ����ʼ��ʱ��), û�а����򷵻�0
	int keyState(SDL_Keycode key);

	//����SDL_User�¼���
	//Ҫ����SDL�¼���ֱ��ʹ��SDL API.
	//����ֵ�μ�SDL_PushEvent
	int userEventTrigger(const SDL_UserEvent &userEvent);

	int exec();
	void quit();

protected:

private:
	
	//����SDL�¼����ӣ�������Ӳ�������SDL_user�¼���Ҫ����user�¼�������setUserEventHook��
	//EventInterface����������ʵ��eventHookProc�Խ��ղ������¼���
	//���óɹ�����0�� ����-1��ʾ�¼������Ѿ������ˣ���ֱ����eventHookProc�д���
	int setEventHook(EventInterface *event, int type);
	void unsetEventHook(EventInterface *event, int type);

	//����SDL_user�¼����ӡ�
	//EventInterface����������ʵ��userEventHookProc�Խ��ղ������¼���
	//���óɹ�����0�� ����-1��ʾ�¼������Ѿ������ˣ���ֱ����userEventHookProc�д���
	int setUserEventHook(EventInterface *event, int type);
	void unsetUserEventHook(EventInterface *event, int type);

	std::map<SDL_Keycode, int> m_monitoringKey;
	std::multimap<int, EventInterface *> m_eventHook;
	std::multimap<int, EventInterface *> m_userEventHook;
};

//����
extern Director *director;
