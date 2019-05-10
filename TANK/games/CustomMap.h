#pragma once
#include "../Event.h"
#include "../Scene.h"
class CustomMap :
	public Scene,
	public EventInterface
{
public:
	CustomMap(Renderer *renderer);
	virtual ~CustomMap();
	

	virtual void open(void *data, int code) override;
	virtual void close() override;
private:
	virtual void update(Uint32 time) override;
	virtual int render() override;
};

