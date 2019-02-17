#include "Player.h"
#include "../Director.h"
#include "../FileLoader.h"
#include <sstream>

Player::Player(PLAYER p)
{

	std::vector<std::string> data;
	fileLoad("control", &data);

	std::string playerName;
	switch (p)
	{
	case Player::P1:
		playerName = "P1";
		break;
	case Player::P2:
		playerName = "P2";
		break;
	default:
		break;
	}

	for (auto &l : data) {
		std::istringstream is(l);
		std::string head;
		is >> head;
		if (head == playerName) {

			//映射移动操作。
			//这块代码我觉得很蠢。
			int key[4];
			for (auto & k : key) {
				is >> k;
			}
			m_move[key[0]] = Mover::UP;
			m_move[key[1]] = Mover::DOWN;
			m_move[key[2]] = Mover::LEFT;
			m_move[key[3]] = Mover::RIGHT;
			is >> m_A >> m_B;
			break;
		}
	}

	//监控按键。
	for (auto &k : m_move) {
		director->monitoringKey(k.first);
	}
	director->monitoringKey(m_A);
	director->monitoringKey(m_B);
}

int Player::command(Ground * ground, Tank * tank, Uint32 timestamp, Mover::DIRECTION & direction)
{
	auto result = inputDirection(&direction);

	if (director->keyState(m_A)) {
		tank->fire();
	}

	return result;
}

int Player::inputDirection(Mover::DIRECTION *ret)
{
	int maxTime = 0;
	int result = -1;
	for(auto &k : m_move) {
		if (director->keyState(k.first) > maxTime) {
			maxTime = director->keyState(k.first);
			*ret = m_move[k.first];
			result = 0;
		}
	}

	return result;
}
