#include "../ResultView.h"
#include "../TANKS.h"
#include "../Renderer.h"
#include "../FileLoader.h"
#include <sstream>
#include "../Timer.h"

ResultView::ResultView(Renderer * renderer)
	:Scene(renderer, { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT })
{

	setBackdropColor({ 0, 0, 9, 255 });

	//导入各个部件的位置数据。
	std::vector<std::string> data;
	if (fileLoad("resultViewLayout", &data) != 5)
		return;

	std::vector<std::istringstream> stream(data.begin(), data.end());

	//准备标题。
	stream[0] >> m_title1Position.x >> m_title1Position.y >> m_title2Position.x >> m_title2Position.y;
	m_title1.setScene(this);
	m_title2.setScene(this);
	m_title1.setTextRenderFlags(Text::biend);
	m_title2.setTextRenderFlags(Text::biend);
	m_title1.setFontColor(title1Color);
	m_title2.setFontColor(title2Color);
	m_title1.setFontSize(buttonFontSize);
	m_title2.setFontSize(buttonFontSize);
	m_title1.setString(L"Player I");
	m_title2.setString(L"Player II");

	//准备列表。
	SDL_Point layoutOffset, modelIconPos, killCountPos, scorePos;
	stream[1] >> layoutOffset.x >> layoutOffset.y >> modelIconPos.x >> modelIconPos.y;
	std::array<std::string, 4> iconName;
	int ij = 0;
	std::string word;
	while (stream[1] >> word)
		iconName[ij++] = word;

	stream[2] >> killCountPos.x >> killCountPos.y >> scorePos.x >> scorePos.y;
	for (auto i = 0; i != m_lists.size(); ++i) {
		for (auto j = 0; j != m_lists[i].size(); ++j) {
			auto a = renderer->animationFactory().findAnimation(iconName[j]);
			a.frameCount = 1;

			m_lists[i][j].icon.setScene(this);
			m_lists[i][j].killCountBox.setScene(this);
			m_lists[i][j].scoreBox.setScene(this);
			m_lists[i][j].icon.setAnimation(a);
			m_lists[i][j].icon.setRenderSize({ 60, 60 });
			m_lists[i][j].killCountBox.setTextRenderFlags(Text::biend);
			m_lists[i][j].killCountBox.setFontSize(dataFontSize);
			m_lists[i][j].killCountBox.setFontColor(color);
			m_lists[i][j].killCountBox.setString(L"× " + std::to_wstring(0));
			m_lists[i][j].scoreBox.setTextRenderFlags(Text::biend);
			m_lists[i][j].scoreBox.setFontSize(dataFontSize);
			m_lists[i][j].scoreBox.setFontColor(color);
			m_lists[i][j].scoreBox.setString(L"= " + std::to_wstring(0));
			m_lists[i][j].iconPosition = { modelIconPos.x + i * layoutOffset.x, modelIconPos.y + j * layoutOffset.y };
			m_lists[i][j].killCountPosition = { killCountPos.x + i * layoutOffset.x, killCountPos.y + (j * layoutOffset.y) };
			m_lists[i][j].scorePosition = { scorePos.x + (i * layoutOffset.x), scorePos.y + (j * layoutOffset.y) };
		}
	}


	//准备统计框。
	for (auto i = 0; i != 2; ++i) {
		for (auto j = 0; j != 2; ++j) {
			auto &c = m_countBox[i][j];
			c.label.setScene(this);
			c.numberBox.setScene(this);
			c.label.setTextRenderFlags(Text::biend);
			c.numberBox.setTextRenderFlags(Text::biend);
			c.label.setFontColor(color);
			c.numberBox.setFontColor(color);
			c.label.setFontSize(dataFontSize);
			c.numberBox.setFontSize(dataFontSize);
			c.numberBox.setString(std::to_wstring(0));
			stream[3] >> c.labelPosition.x >> c.labelPosition.y >> c.numberBoxPosition.x >> c.numberBoxPosition.y;
		}
		m_countBox[i][0].label.setString(L"KILL: ");
	}
	m_countBox[0][1].label.setString(L"TOTAL: ");
	m_countBox[1][1].label.setString(L"TOTAL: ");

	//准备按钮。
	m_select = 0;
	m_cursor.setScene(this);
	m_home.text.setScene(this);
	m_next.text.setScene(this);
	m_cursor.setAnimation("P1");
	m_cursor.setAngle(90);
	m_cursor.setRenderSize({ 40, 40 });
	stream[4] >> m_home.cursorPosition.x >> m_home.cursorPosition.y >> m_home.position.x >> m_home.position.y
		>> m_next.cursorPosition.x >> m_next.cursorPosition.y >> m_next.position.x >> m_next.position.y;
	m_home.text.setTextRenderFlags(Text::biend);
	m_next.text.setTextRenderFlags(Text::biend);
	m_home.text.setFontColor(color);
	m_next.text.setFontColor(color);
	m_home.text.setFontSize(buttonFontSize);
	m_next.text.setFontSize(buttonFontSize);
	m_home.text.setString(L"HOME");
	m_next.text.setString(L"NEXT");

	loadKey("P1control");
	loadKey("P2control");

	resetListData();
}

ResultView::~ResultView()
{
	hide();
}

void ResultView::open(void * data, int code)
{
	auto s = (SCORECARDS **)(data);
	if (!*s)
		return;
	
	m_playerCount = code;
	if (m_playerCount <= 0 || m_playerCount > 2)
		throw std::out_of_range("Player count out of range");

	//复制结果，同时将ARMOURE1和ARMOURE2的结果存放到ARMOURED3。
	for (auto i = 0; i != m_playerCount; ++i) {
		auto &scorecards = *(*(s + i));
		m_results[i][4] += m_results[i][0] = scorecards[Tank::ORDINARY1].killCount;
		m_results[i][4] += m_results[i][1] = scorecards[Tank::ORDINARY2].killCount;
		m_results[i][4] += m_results[i][2] = scorecards[Tank::AGILITY].killCount;
		m_results[i][4] += m_results[i][3] = scorecards[Tank::ARMOURED1].killCount + 
											 scorecards[Tank::ARMOURED2].killCount + 
											 scorecards[Tank::ARMOURED3].killCount;
	}
	
	resetListData();
	m_lastUpdateTime = Timer::current();
	installUserEventHook();
	installEventHook();
}

void ResultView::close()
{
	uninstallUserEventHook();
	uninstallEventHook();
}

int ResultView::render()
{
	m_title1.renderFrame(m_title1Position);
	m_title2.renderFrame(m_title2Position);

	for (int i = 0; i != m_lists.size(); ++i) {
		for (int j = 0; j != m_lists[i].size(); ++j) {
			m_lists[i][j].icon.renderFrame(m_lists[i][j].iconPosition);
			m_lists[i][j].killCountBox.renderFrame(m_lists[i][j].killCountPosition);
			m_lists[i][j].scoreBox.renderFrame(m_lists[i][j].scorePosition);
		}
	}

	m_home.text.renderFrame(m_home.position);
	m_next.text.renderFrame(m_next.position);

	m_cursor.renderFrame((!m_select) ? m_home.cursorPosition : m_next.cursorPosition);

	for (auto i = 0; i != m_playerCount; ++i) {
		for (auto j = 0; j != 2; ++j) {
			auto &c = m_countBox[i][j];
			c.label.renderFrame(c.labelPosition);
			c.numberBox.renderFrame(c.numberBoxPosition);
		}
	}

	return 0;
}

void ResultView::update(Uint32 time)
{
	if (time - m_lastUpdateTime < updateInterval) 
		return;

	m_lastUpdateTime = time;
	Tank::MODEL models[4] = {Tank::ORDINARY1, Tank::ORDINARY2, Tank::AGILITY, Tank::ARMOURED3};

	for (size_t c = 0; c != m_playerCount; ++c) {
		SCORECARD stat;
		for (auto i = 0; i != 4; ++i) {
			stat.killCount += m_currentList[c][i] = m_currentList[c][i] + 1 >= m_results[c][i] ? m_results[c][i] : m_currentList[c][i]+1;
			m_lists[c][i].killCountBox.setString(L"× " + std::to_wstring(m_currentList[c][i]));
			m_lists[c][i].scoreBox.setString(L"= " + std::to_wstring(m_currentList[c][i]*Driver::queryScoreForModel(models[i])));
			stat.total += m_currentList[c][i] * Driver::queryScoreForModel(models[i]);
		}
		m_countBox[c][0].numberBox.setString(std::to_wstring(stat.killCount));
		m_countBox[c][1].numberBox.setString(std::to_wstring(stat.total));
	}
}

void ResultView::eventHookProc(const SDL_Event & event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		if (m_switchKeys.find(event.key.keysym.sym) != m_switchKeys.end()) {
			m_select = !m_select;
		}
		else if (m_selectKeys.find(event.key.keysym.sym) != m_selectKeys.end()) {
			SDL_UserEvent user;
			user.type = CLOSE;
			user.data1 = reinterpret_cast<void *>(this);
			user.code = static_cast<Sint32>(m_select);
			userEventTrigger(user);
			hide();
		}
		break;
	case SDL_MOUSEMOTION: 
	{
		SDL_Rect r1 = { m_home.position.x, m_home.position.y, m_home.text.renderSize().w, m_home.text.renderSize().h };
		SDL_Rect r2 = { m_next.position.x, m_next.position.y, m_next.text.renderSize().w, m_next.text.renderSize().h };
		SDL_Point p = {event.motion.x, event.motion.y};
		if (SDL_PointInRect(&p, &r1))
			m_select = false;
		else if (SDL_PointInRect(&p, &r2))
			m_select = true;
	}
		break;
	case SDL_MOUSEBUTTONUP:
	{
		if (event.button.button != SDL_BUTTON_LEFT)
			break;

		SDL_Rect r1 = { m_home.position.x, m_home.position.y, m_home.text.renderSize().w, m_home.text.renderSize().h };
		SDL_Rect r2 = { m_next.position.x, m_next.position.y, m_next.text.renderSize().w, m_next.text.renderSize().h };
		SDL_Point p = { event.button.x, event.button.y };
		if (SDL_PointInRect(&p, &r1)) {
			m_select = false;
			SDL_UserEvent user;
			user.type = CLOSE;
			user.data1 = reinterpret_cast<void *>(this);
			user.code = static_cast<Sint32>(m_select);
			userEventTrigger(user);
			hide();
		}
		else if (SDL_PointInRect(&p, &r2)) {
			m_select = true;
			SDL_UserEvent user;
			user.type = CLOSE;
			user.data1 = reinterpret_cast<void *>(this);
			user.code = static_cast<Sint32>(m_select);
			userEventTrigger(user);
			hide();
		}
	}
		break;
	default:
		break;
	}
}

void ResultView::resetListData()
{
	for (int i = 0; i != 2; ++i) {
		m_currentList[i].fill(0);
	}
}

void ResultView::loadKey(const std::string & dataGroup)
{
	std::vector<std::string> control;
	if (fileLoad(dataGroup, &control) != 6)
		return;

	for (auto i = 0; i != 4; ++i) {
		SDL_Keycode code = SDL_GetKeyFromName(control[i].c_str());
		if (code != SDLK_UNKNOWN)
			m_switchKeys.insert(code);
	}

	for (auto i = 4; i != control.size(); ++i) {
		SDL_Keycode code = SDL_GetKeyFromName(control[i].c_str());
		if (code != SDLK_UNKNOWN)
			m_selectKeys.insert(code);
	}
}
