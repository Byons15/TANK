#pragma once
#include "Scene.h"
#include "games\Maps.h"
#include "Event.h"

class Ground :
	public Scene,
	public EventInterface
{
public:
	Ground();
	~Ground();

protected:
	virtual void open(void *data, int code) override;
	virtual void close() override;

	virtual void userEventHookProc(const SDL_UserEvent &user) override;

private:
	Maps m_map;
};

