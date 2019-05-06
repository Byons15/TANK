#include "..\StateMenu.h"
#include "../TANKS.h"
#include "../FileLoader.h"
#include <sstream>

StateMenu::StateMenu(Renderer * renderer)
	:Scene(renderer, {GRID_SIZE * MAP_SIZE, 0, WINDOW_WIDTH - GRID_SIZE * MAP_SIZE, WINDOW_HEIGHT})
{
	//导入所有标签或图标的位置。
	std::vector<std::string> data;
	if (fileLoad("", &data) == 0)
		throw std::runtime_error("找不到布局数据");
	
	std::map<std::string, SDL_Point> positions;
	for (auto &line : data) {
		std::istringstream is(line);
		SDL_Point p;
		std::string name;
		is >> name >> p.x >> p.y;
		positions.insert({name, p});
	}

	//将所有标签都绑定到场景中。
	for (auto &l : m_labels)
		l.second.setScene(this);
	m_P1ScoreBox.setScene(this);
	m_P2ScoreBox.setScene(this);
	m_P1LifeCountBox.setScene(this);
	m_P2LifeCountBox.setScene(this);

	//获取所有标签的位置，并设置标签的Sprite。
	m_labels[0].first = positions["P1ScoreLabel"];
	m_labels[1].first = positions["P2ScoreLabel"];
	m_labels[2].first = positions["P1LifeCountLabel"];
	m_labels[3].first = positions["P2LifeCountLabel"];
	m_labels[4].first = positions["P1LifeCountIcon"];
	m_labels[5].first = positions["P2LifeCountIcon"];
	m_labels[6].first = positions["levelIcon"];
	m_labels[0].second.setAnimation("P1ScoreLabel");
	m_labels[1].second.setAnimation("P2ScoreLabel");
	m_labels[2].second.setAnimation("P1LifeCountLabel");
	m_labels[3].second.setAnimation("P2LifeCountLabel");
	m_labels[4].second.setAnimation("P1LifeCountIcon");
	m_labels[5].second.setAnimation("P2LifeCountIcon");
	m_labels[6].second.setAnimation("levelIcon");
	m_P1ScorePosition = positions["P1Score"];
	m_P2ScorePosition = positions["P2Score"];
	m_P1LifeCountPosition = positions["P1LifeCount"];
	m_P2LifeCountPosition = positions["P2LifeCount"];
	m_P1ScoreBox.setString(std::to_wstring(m_P1Score));
	m_P2ScoreBox.setString(std::to_wstring(m_P2Score));
	m_P1LifeCountBox.setString(std::to_wstring(m_P1LifeCount));
	m_P2LifeCountBox.setString(std::to_wstring(m_P2LifeCount));

	//设置各个文本框的大小。
	m_P1ScoreBox.setFontSize(30);
	m_P2ScoreBox.setFontSize(30);
	m_P1LifeCountBox.setFontSize(40);
	m_P2LifeCountBox.setFontSize(40);
	
}

size_t StateMenu::playerLife(Player::PLAYER p)
{
	return (p == Player::P1) ? m_P1LifeCount : m_P2LifeCount;
}

StateMenu::~StateMenu()
{
}

void StateMenu::update(Uint32 time)
{
}

int StateMenu::render()
{
	return 0;
}

void StateMenu::open(void * data, int code)
{
}

void StateMenu::close()
{
}
