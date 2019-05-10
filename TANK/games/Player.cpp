#include "Player.h"
#include "../Director.h"
#include "../FileLoader.h"
#include <sstream>
#include <SDL_keyboard.h>

bool Player::requestFire()
{
	return director->keyState(m_A);;
}

Player::Player(PLAYER p)
{
	std::string playerName;
	switch (p)
	{
	case Player::P1:
		playerName = "P1control";
		break;
	case Player::P2:
		playerName = "P2control";
		break;
	default:
		break;
	}
	std::vector<std::string> data;
	if (fileLoad(playerName, &data) != 6)
		throw std::runtime_error("读取按键设置错误");

	SDL_Keycode keys[4];
	for (size_t i = 0; i != 6; ++i) {
		SDL_Keycode code = SDL_GetKeyFromName(data[i].c_str());
		if (code == SDLK_UNKNOWN)
			throw std::runtime_error("读取按键错误");
		if (i < 4) {
			keys[i] = code;
		}
		else if (i == 4)
			m_A = code;
		else
			m_B = code;
	}

	m_move[keys[0]] = Mover::LEFT;
	m_move[keys[1]] = Mover::RIGHT;
	m_move[keys[2]] = Mover::UP;
	m_move[keys[3]] = Mover::DOWN;

	//监控按键。
	for (auto &k : m_move) {
		director->monitoringKey(k.first);
	}
	director->monitoringKey(m_A);
	director->monitoringKey(m_B);
}

int Player::command(SDL_Point position, Uint32 timestamp, Mover::DIRECTION & direction)
{
	int maxTime = 0;
	int result = -1;
	for (auto &k : m_move) {
		if (director->keyState(k.first) > maxTime) {
			maxTime = director->keyState(k.first);
			direction = m_move[k.first];
			result = 0;
		}
	}

	return result;
}
