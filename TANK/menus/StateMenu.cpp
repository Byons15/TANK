#include "..\StateMenu.h"
#include "../TANKS.h"
#include "../FileLoader.h"
#include <sstream>

StateMenu::StateMenu(Renderer * renderer)
	:Scene(renderer, {GRID_SIZE * MAP_SIZE, 0, WINDOW_WIDTH - GRID_SIZE * MAP_SIZE, WINDOW_HEIGHT}),
	m_enemyCount(0)
{
	setBackdropColor(SDL_Color{153, 153, 153, SDL_ALPHA_OPAQUE});
	//导入所有标签或图标的位置。
	std::vector<std::string> data;
	if (fileLoad("stateMenuLayout", &data) != 13)
		throw std::runtime_error("找不到布局数据");

	//获取标签的位置及纹理。
	size_t dataIndex = 0;
	for (auto & l : m_labels) {
		std::istringstream is(data[dataIndex++]);
		std::string name;
		l.second.setScene(this);
		is >> name >> l.first.x >> l.first.y;
		l.second.setAnimation(name);
	}

	//获取文本框的位置
	for (auto &t : m_text) {
		t.box.setScene(this);
		std::istringstream is(data[dataIndex++]);
		std::string name;
		is >> name >> t.position.x >> t.position.y;
		t.box.setFontColor({ 0, 0, 0, SDL_ALPHA_OPAQUE });
	}

	//设置各个文本框的大小和字体颜色。
	m_text[0].box.setFontSize(30);
	m_text[1].box.setFontSize(30);
	m_text[2].box.setFontSize(40);
	m_text[3].box.setFontSize(40);
	m_text[4].box.setFontSize(40);

	//设置敌军计数的图标纹理以及图标排列的起始位置。
	std::istringstream is(data[dataIndex]);
	std::string name;
	is >> name >> m_enemyCountIconBeginPosition.x >> m_enemyCountIconBeginPosition.y;
	m_enemyCountIcon.setScene(this);
	m_enemyCountIcon.setAnimation(name);
	m_enemyCount = 0;
}

StateMenu::~StateMenu()
{
}

void StateMenu::update(Uint32 time)
{
	for (auto &t : m_text) {
		t.box.setString(std::to_wstring(t.data));
	}
	for (auto & l : m_labels)
		l.second.updateFrames(time);
	for (auto & t : m_text)
		t.box.updateFrames(time);
	m_enemyCountIcon.updateFrames(time);
}

int StateMenu::render()
{
	for (auto & l : m_labels) {
		l.second.renderFrame(l.first);
	}

	for (auto & t : m_text) {
		t.box.renderFrame(t.position);
	}

	int offsetX, offsetY;
	for (auto i = 0; i != m_enemyCount; ++i) {
		offsetY = (i / 2) * GRID_SIZE;
		offsetX = (i % 2) * GRID_SIZE;
		m_enemyCountIcon.renderFrame({m_enemyCountIconBeginPosition.x + offsetX, m_enemyCountIconBeginPosition.y + offsetY});
	}

	return 0;
}

void StateMenu::open(void * data, int code)
{
	STATE *s = reinterpret_cast<STATE *> (data);
	if(data)
		setPlayerLife(Player::P1, s->playerLife[0]);
}

void StateMenu::close()
{
}
