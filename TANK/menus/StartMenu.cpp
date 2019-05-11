#include "..\StartMenu.h"
#include "../TANKS.h"
#include "../Renderer.h"
#include "../Director.h"

static constexpr int FONT_SIZE = 30;

StartMenu::StartMenu(Renderer *renderer)
	:Scene(renderer, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT})
{
	//设置字体。
	for (auto &c : m_combo) {
		c.setFontSize(FONT_SIZE);
	}

	//设置LOGO、菜单、光标位置以及场景。
	m_logo.setScene(this);
	m_logoPosition = { static_cast<int>(WINDOW_WIDTH * 0.18f), static_cast<int>(WINDOW_HEIGHT * 0.23f) };
	m_logo.setAnimation("LOGO");

	int itemHeight = static_cast<int>(WINDOW_HEIGHT * 0.61f);
	for (int i = 0; i != 3; ++i) {
		auto &p = m_menuItemPosition[i];
		p.y = itemHeight;
		itemHeight += m_combo[0].fontSize() + 20;
		
		m_combo[i].setScene(this);
	}
	switchMenu(1);

	m_cursor.setScene(this);
	m_cursor.setAnimation("P1");
	m_cursor.setAngle(90);
	m_cursor.setRenderSize(SIZE{ FONT_SIZE, FONT_SIZE });

	for (int i = 0; i != 3; ++i) {
		m_cursorViewPosition[i].x = static_cast<int>(WINDOW_WIDTH * 0.42f);
		m_cursorViewPosition[i].y = m_menuItemPosition[i].y;
	}
}

StartMenu::~StartMenu()
{
}

void StartMenu::update(Uint32 time)
{
	m_logo.updateFrames(time);
	m_cursor.updateFrames(time);
	for (auto &text : m_combo) {
		
	
	}
	for (int i = 0; i != 3; ++i) {
		m_combo[i].updateFrames(time);
		m_menuItemPosition[i].x = WINDOW_WIDTH / 2 - m_combo[i].renderSize().w / 2;
	}
}

int StartMenu::render()
{
	//渲染LOGO。
	m_logo.renderFrame(m_logoPosition);

	//渲染菜单。
	for (int i = 0; i != 3; ++i) {
		m_combo[i].renderFrame(m_menuItemPosition[i]);
	}

	//渲染指针。
	m_cursor.renderFrame(m_cursorViewPosition[m_currItem]);
	return 0;
}

void StartMenu::open(void * data, int code)
{
	//接收键盘和鼠标输入以提供选项。
	setEventHook(SDL_KEYDOWN);
	setEventHook(SDL_MOUSEMOTION);
	setEventHook(SDL_MOUSEBUTTONUP);

	setState(true);
}

void StartMenu::eventHookProc(const SDL_Event & event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_DOWN:
			m_currItem = (m_currItem == 2) ? 0 : m_currItem + 1;
			break;

		case SDLK_UP:
			m_currItem = (m_currItem == 0) ? 2 : m_currItem - 1;
			break;

		case SDLK_KP_ENTER:
		case SDLK_RETURN:
		case SDLK_RETURN2:
			switch (m_currItem)
			{
			case 0:
			case 1:
				if (m_menuLayer == 1) {
					m_gameModel = m_currItem + 1;
					switchMenu(2);
				}
				else {
					m_playerMember = m_currItem + 1;
					close();
				}
				break;

			case 2:
				if (m_menuLayer == 2) {  //如果是二级菜单，则返回一级菜单。
					switchMenu(1);
				}
				else {  //如果是一级菜单，则结束程序。
					director->quit();
				}
				break;
			default:
				break;
			}
			break;
		case SDLK_BACKSPACE:
			if (m_menuLayer == 2) {
				switchMenu(1);
			}
			break;
		default:
			break;
		}

		break;
	case SDL_MOUSEMOTION:
		break;

	case SDL_MOUSEBUTTONUP:
		break;

	default:
		break;
	}
}

void StartMenu::close()
{
	unsetEventHook(SDL_KEYDOWN);
	unsetEventHook(SDL_MOUSEMOTION);
	unsetEventHook(SDL_MOUSEBUTTONUP);

	setState(false);

	SDL_UserEvent user;
	user.type = CLOSE;
	user.data1 = reinterpret_cast<void *> (m_gameModel);
	user.data2 = reinterpret_cast<void *> (m_playerMember);
	director->userEventTrigger(user);
}

void StartMenu::switchMenu(int layer)
{
	switch (layer)
	{
	case 1:
		m_combo[0].setString(L"闯关模式");
		m_combo[1].setString(L"自定义");
		m_combo[2].setString(L"退出");
		break;

	case 2:
		m_combo[0].setString(L"1 PLAYER");
		m_combo[1].setString(L"2 PLAYER");
		m_combo[2].setString(L"后退");
		break;

	default:
		break;
	}

	m_currItem = 0;
	m_menuLayer = layer;
}
