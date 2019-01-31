#pragma once
#include <SDL_events.h>
#include <set>

class Director;

/*
��װSDL�¼��Լ�SDL�û��¼����ù��ӵķ�ʽʵ�֡�
*/
class EventInterface
{
	friend Director;
public:
	virtual ~EventInterface();
protected:

	//see director::setEventHook
	int setEventHook(int type);
	void unsetEventHook(int type);

	//see director::setUserEventHook
	int setUserEventHook(int type);
	void unsetUserEventHook(int type);

	//see director::userEventTrigger
	int userEventTrigger(const SDL_UserEvent &userEvent);
	
	//SDL�¼����ӻص�������
	//����Director::setEventHook���������¼�����ʱ��ص����������
	//������ʵ�֡�
	virtual void eventHookProc(const SDL_Event & event);

	//SDL�û��¼����ӻص�������
	//����������û��Զ����¼��Ĺ��ӻص�����Director::setUserEventHook���á�
	//��Ӧ������Director::userEventTrigger�㲥���¼���ȷ�е�˵Ӧ������Ϣ��
	virtual void userEventHookProc(const SDL_UserEvent &event);
private:
	std::set<int> m_eventHook, m_userEventHook;
};
