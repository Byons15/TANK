#pragma once
#include "Scene.h"
#include "../Text.h"
#include <array>
#include "Event.h"
#include <set>

class ResultView :
	public Scene,
	public EventInterface
{
public:
	enum EVENT
	{
		CLOSE = 0x2533,
	};
	struct RESULT
	{
		size_t lists[4][2] = {};
		size_t killCount = 0, total = 0;
	};

	ResultView(Renderer *renderer);
	~ResultView();



protected:
	virtual int render() override;
	virtual void update(Uint32 time) override;
	virtual void eventHookProc(const SDL_Event & event) override;

	//data： 一组存放着RESULT指针的数组, 函数只获取成员lists.
	//code：玩家个数，也是data数组的长度。
	//函数不会delete这个数组。
	virtual void open(void *data, int code) override;
	virtual void close() override;

private:
	void loadKey(const std::string & dataGroup);
	static constexpr SDL_Color title1Color{ 56, 217, 123, 255}, 
							   title2Color{252, 194,  13, 255}, 
							   color      {224, 222, 222, 255};
	static constexpr size_t titleFontSize = 60, dataFontSize = 40, buttonFontSize = 40;
	static constexpr Uint32 updateInterval = 400;
	Text m_title1, m_title2;
	SDL_Point m_title1Position, m_title2Position;

	struct BUTTON
	{
		Text text;
		SDL_Point position, cursorPosition;
	} m_home, m_next;
	Spirit m_cursor;
	bool m_select;

	struct ITEM
	{
		Spirit icon;
		Text killCountBox, scoreBox;
		SDL_Point iconPosition, killCountPosition, scorePosition;
		int killCount = 0, score = 0;
	};
	std::array<std::array<ITEM, 4>, 2> m_lists;

	struct COUNTBOX
	{
		Text label, numberBox;
		SDL_Point labelPosition, numberBoxPosition;
		int data = 0;
	};
	std::array<COUNTBOX, 4> m_countBox;
	std::array<RESULT, 2> m_result, m_currentList;
	std::set<SDL_Keycode> m_switchKeys, m_selectKeys;
	Uint32 m_lastUpdateTime;
	bool m_resetView;
};

