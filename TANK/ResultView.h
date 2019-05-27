#pragma once
#include "Scene.h"
#include "../Text.h"
#include <array>
#include "Event.h"

class ResultView :
	public Scene,
	public EventInterface
{
public:
	ResultView(Renderer *renderer);
	~ResultView();
	virtual void open(void *data, int code) override;
	virtual void close() override;

protected:
	virtual int render() override;
	virtual void update(Uint32 time) override;
	virtual void eventHookProc(const SDL_Event & event) override;
private:
	void loadKey(const std::string & dataGroup);
	static constexpr SDL_Color title1Color{ 56, 217, 123, 255}, 
							   title2Color{252, 194,  13, 255}, 
							   color      {224, 222, 222, 255};
	static constexpr size_t titleFontSize = 40, dataFontSize = 30, buttonFontSize = 40;
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

	std::set<SDL_Keycode> m_switchKeys, m_selectKeys;
};

