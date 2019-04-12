#include "Normal.h"

Normal::Normal(Ground * ground)
	:m_ground(ground), m_P1commander(Player::P1)
{
	//TODO::Debug
	m_P1 = m_ground->addTank(2, Ground::ALLISE, 0);
	m_P1->setCommander(&m_P1commander);
	if (m_ground->maps().loadMaps(1) == -1) {
		m_ground->maps().createMaps(m_ground->terrainItemCount());
	}
	m_ground->open(0, 0);
}

Normal::~Normal()
{
}
