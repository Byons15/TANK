#pragma once
#include <SDL_events.h>
#include <functional>
#include "Director.h"

/*
��װSDL�¼��Լ�SDL�û��¼����ù��ӵķ�ʽʵ�֡�
*/
class EventInterface
{
	friend Director;
public:
	virtual ~EventInterface()
	{
		uninstallEventHook();
		uninstallUserEventHook();
	}
protected:
	//see director::userEventTrigger
	void userEventTrigger(const SDL_UserEvent &userEvent)
	{
		director->userEventTrigger(userEvent);
	}

	inline void installEventHook()
	{
		director->setEventHook(this);
	}
	inline void uninstallEventHook()
	{
		director->unsetEventHook(this);
	}
	inline void installUserEventHook()
	{
		director->setUserEventHook(this);
	}
	inline void uninstallUserEventHook()
	{
		director->unsetUserEventHook(this);
	}
	
	//SDL�¼����ӻص�������
	//����Director::setEventHook���������¼�����ʱ��ص����������
	//������ʵ�֡�
	virtual void eventHookProc(const SDL_Event & event)
	{
		//DEFAULT
	}

	//SDL�û��¼����ӻص�������
	//����������û��Զ����¼��Ĺ��ӻص�����Director::setUserEventHook���á�
	//��Ӧ������Director::userEventTrigger�㲥���¼���ȷ�е�˵Ӧ������Ϣ��
	virtual void userEventHookProc(const SDL_UserEvent &event)
	{
		//DEFAULT
	}

	
private:
};
