#include "Normal.h"

Normal::Normal(Ground * ground)
	:m_ground(ground), m_P1commander(Player::P1)
{
	//TODO::Debug
	m_P1 = m_ground->addTank(5, Ground::ALLISE, 0);
	m_P1->setCommander(&m_P1commander);
	m_ground->maps().loadMaps(1);
	m_ground->open(0, 0);
}

Normal::~Normal()
{
}
