#pragma once
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include "Spirit.h"

class Renderer;
struct SDL_Texture;

/*
Scene(场景)：
Scene是Spirit类依赖的类，一个Scene保存了一组精灵所需要的共同数据，包括背景（这由渲染器处理）、精灵位
置坐标的起点，精灵最终的目标渲染器。Spirit在缺少Scene时将无法正常渲染。Scene同时也是接口，派生类继承
并重新实现update()、render()后，渲染器将会在每帧调用一遍update和render，渲染器首先会调用update，派生
类应该在函数中更新每帧的数据。之后渲染器调用render()，派生类在函数中布置精灵并渲染到场景。事实上我建
议在update中完成所有数据更新，而在render中渲染精灵。

导演可以控制切换场景，可以完成类似 “开始菜单” - “游戏界面”此类的场景转换。
派生类实现open.close.
*/
class Scene
{
	friend class Renderer;
public:
	Scene(Renderer *renderer = 0, const SDL_Rect &sceneRect = {0, 0, 0, 0});
	virtual ~Scene();

	//设置场景背景颜色。
	//成功返回0， 返回-1表示没有设置渲染器。
	int setBackdropColor(const SDL_Color &color);
	Renderer *renderer() {return m_renderer;}
	void setRect(const SDL_Rect &rect);
	const SDL_Rect &rect() const {return m_rect;}
	bool state() const {return m_state;}

	void show(void *data, int code)
	{
		setState(true);
		open(data, code);
	}
	void hide()
	{
		setState(false);
		close();
	}

protected:
	virtual void update(Uint32 time) = 0;

	//派生类实现。
	//返回spirit渲染个数。
	virtual int render() = 0;

	virtual void open(void *data, int code) = 0;
	virtual void close() = 0;

	SDL_Texture *backdrop(SDL_Rect *rect);
	int setState(bool state);
private:
	SDL_Rect m_rect;
	SDL_Texture *m_backdrop;
	Renderer *m_renderer;
	bool m_state;
};

