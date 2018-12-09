#pragma once
#include <SDL_events.h>

class Games;

/*
��װSDL�¼��Լ�SDL�û��¼����ù��ӵķ�ʽʵ�֡�
*/
class EventInterface
{
	friend Games;
public:
	virtual ~EventInterface() = default;
protected:
	
	//SDL�¼����ӻص�������
	//����Games::setEventHook���������¼�����ʱ��ص����������
	//������ʵ�֡�
	virtual void eventHookProc(const SDL_Event & event) = 0;

	//SDL�û��¼����ӻص�������
	//����������û��Զ����¼��Ĺ��ӻص�����Games::setUserEventHook���á�
	//��Ӧ������Games::userEventTrigger�㲥���¼���ȷ�е�˵Ӧ������Ϣ��
	virtual void userEventHookProc(const SDL_UserEvent &event) = 0;

};
