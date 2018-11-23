#pragma once
#include <SDL_events.h>

#define EVENT_FILTER 0x901

class Games;

/*
��װSDL_UserEvent���ࡣ
Event��SDL_UserEvent�е�type��Ա��Ϊ�¼���ʶ��
Event���ܴ���SDL_UserEvent������¼���
*/
class Event
{
	friend Games;
public:

	enum CALLWAY
	{
		Async,
		Sync,
	};

	//�����¼���
	//callWay == Sync : ����ֱ�Ӻ���������ʵ�ֵ�eventProc�����¼����䴦�� 
	//callWay == Async : �������¼�push��SDL���¼������У��첽����eventProc��
	int eventTrigger(int type, int code, void *data1 = 0, void *data2 = 0, CALLWAY callWay = Async);
	virtual ~Event() = default;
protected:
	//�¼�����
	//�����ص��¼��ڴ������������������
	//��������Ҫʵ������
	virtual void eventProc(const SDL_UserEvent &event) = 0;
};
