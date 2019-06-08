#pragma once
#include "Scene.h"
#include "Event.h"
#include "Text.h"

/*
开始菜单
开始菜单继承场景、事件接口。
开始菜单在关闭时会触发一个"CLOSE"User事件，其中:
data1存放游戏模式 1(闯关模式) or 2(自定义战场)
data2存放玩家个数 1 or 2
*/
class StartMenu :
	public Scene,
	public EventInterface
{
public:

	//开始菜单关闭。
	//data1 :游戏模式。 1(闯关模式) or 2(自定义地图)
	//data2 :玩家个数。
	static constexpr int CLOSE = 0x155230;

	StartMenu(Renderer *renderer);
	~StartMenu();


protected:
	virtual void update(Uint32 time) override;
	virtual int render() override;
	virtual void open(void *data, int code) override;
	virtual void close() override;
	virtual void eventHookProc(const SDL_Event & event) override;

private:
	void switchMenu(int menuLayer);
	

	Spirit m_logo, m_cursor;
	Text m_combo[3];
	SDL_Point m_cursorViewPosition[3], m_menuItemPosition[3], m_logoPosition;
	int m_menuLayer, m_currItem, m_gameModel, m_playerMember;
};

