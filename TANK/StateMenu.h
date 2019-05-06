#pragma once
#include "Scene.h"
#include "Spirit.h"
#include <array>
#include "Text.h"
#include "games/Player.h"

class StateMenu :
	public Scene
{
public:
	StateMenu(Renderer *renderer);
	size_t playerLife(Player::PLAYER p);
	virtual ~StateMenu();
protected:
	virtual void update(Uint32 time) override;
	virtual int render() override;
	virtual void open(void *data, int code) override;
	virtual void close() override;
private:
	std::array<std::pair<SDL_Point, Spirit>, 7> m_labels;
	Text m_P1ScoreBox, m_P2ScoreBox, m_P1LifeCountBox, m_P2LifeCountBox;
	Spirit m_enemyCountIcon;
	SDL_Point m_enemyCountIconBeginPoint, m_P1ScorePosition, m_P2ScorePosition, m_P1LifeCountPosition, m_P2LifeCountPosition;
	size_t m_enemyCount, m_P1Score, m_P2Score, m_P1LifeCount, m_P2LifeCount;
};

