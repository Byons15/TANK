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
	struct STATE
	{
		int playerLife[2], plyaerScore[2], level, enemyCount;
	};
	StateMenu(Renderer *renderer);
	size_t playerLife(Player::PLAYER p) const {return (p == Player::P1) ? m_text[2].data : m_text[3].data;}
	void setPlayerLife(Player::PLAYER p, size_t life) {((p == Player::P1) ? m_text[2].data : m_text[3].data) = life;}
	size_t playerScore(Player::PLAYER p) const {return (p == Player::P1) ? m_text[0].data : m_text[1].data;}
	void setPlayerScore(Player::PLAYER p, size_t score) {((p == Player::P1) ? m_text[0].data : m_text[1].data) = score;}
	size_t level() const {return m_text[4].data;}
	void setLevel(size_t level) {m_text[4].data = level;}
	size_t enemyCount() const {return m_enemyCount;}
	void setEnemyCount(size_t count) {m_enemyCount = count;}


	virtual ~StateMenu();

protected:
	virtual void update(Uint32 time) override;
	virtual int render() override;

	//data：接收一个STATE的指针。
	//code：保留，未使用。
	virtual void open(void *data, int code) override;
	virtual void close() override;

private:
	struct TextBox
	{
		Text box;
		SDL_Point position;
		int data = 0;
	};
	std::array<std::pair<SDL_Point, Spirit>, 7> m_labels;
	std::array<TextBox, 5> m_text;
	Spirit m_enemyCountIcon;
	SDL_Point m_enemyCountIconBeginPosition;
	size_t m_enemyCount;

};

