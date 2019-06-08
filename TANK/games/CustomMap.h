#pragma once
#include "../Event.h"
#include "../Scene.h"
#include "../ground/Ground.h"
#include "../ground/Mover.h"

class CustomMap :
	public Scene,
	public EventInterface
{
public:
	static constexpr int CLOSE = 0x53243;

	CustomMap(Renderer *renderer, Ground *ground);
	virtual ~CustomMap();

protected:
	virtual void open(void *data, int code) override;
	virtual void close() override;

private:
	virtual void eventHookProc(const SDL_Event & event) override;
	virtual void update(Uint32 time) override;
	virtual int render() override;
	Spirit m_curosr;
	int m_currentTerrainIndex;
	SDL_Point m_curosrPosition;
	Ground *m_ground;
	std::map<SDL_Keycode, int> m_keyMaps;
};

