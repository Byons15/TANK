#include "../Event.h"

int Event::eventTrigger(int type, int code, void * data1, void * data2, CALLWAY callWay)
{
	SDL_UserEvent user;
	user.type = type;
	user.code = code;
	user.data1 = data1;
	user.data2 = data2;

	if (callWay == Sync) {
		eventProc(user);	//ͬ������
	}
	else {    //�첽���У��¼���ӵ�SDL�¼�����
		SDL_Event event;

		event.user = user;
		event.user.type = event.type = SDL_USEREVENT;
		event.user.code = EVENT_FILTER;
		event.user.data1 = this;
		event.user.data2 = new SDL_UserEvent(user);
		SDL_PushEvent(&event);
	}

	return 0;
}