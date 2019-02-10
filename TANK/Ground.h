#pragma once
#include "Scene.h"
#include "games\Maps.h"
#include "Event.h"

class Ground :
	public Scene,
	public EventInterface
{
public:
	Ground(Renderer *renderer);
	~Ground();

	virtual void open(void *data, int code) override;
	virtual void close() override;

protected:
	
	virtual void update(Uint32 time) override;
	virtual int render() override;
	virtual void userEventHookProc(const SDL_UserEvent &user) override;

private:
	Maps m_map;
	
};


SDL_Rect pixelToGroundRect(const SDL_Rect & pixelRect);
SDL_Point pixelToGroundPoint(const SDL_Point & pixelPoint);