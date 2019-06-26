#include "AI.h"
#include "../ground/Ground.h"
#include "../Timer.h"
#include "../Games.h"

#define FIRE 0x56111

AI::AI(Tank * tank, int level)
	:m_tank(tank), m_level(level), m_gps(&tank->ground()->colMap())
{
	m_fireIntervalMin = 1000;
	m_fireIntervalMax = 5000;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> uid(m_fireIntervalMin, m_fireIntervalMax);
	m_timerID = Timer::addTimer(uid(gen), fireTimerCallback, this);
	installUserEventHook();
}

AI::~AI()
{
	Timer::removeTimer(m_timerID);
	uninstallUserEventHook();
}

int AI::command(SDL_Point position, Uint32 timestamp, Mover::DIRECTION & direction)
{
	int defaultWeight = 8;
	int offsetWeight = 100 - defaultWeight * 4;
	std::array<double, 4> weights{ defaultWeight, defaultWeight, defaultWeight, defaultWeight };
	switch (m_tank->direction())
	{
	case Mover::LEFT:
		weights[0] += offsetWeight;
		break;
	case Mover::UP:
		weights[1] += offsetWeight;
		break;
	case Mover::RIGHT:
		weights[2] += offsetWeight;
		break;
	case Mover::DOWN:
		weights[3] += offsetWeight;
		break;
	default:
		break;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> dd(weights.begin(), weights.end());
	switch (dd(gen))
	{
	case 0:
		direction = Mover::LEFT;
		break;
	case 1:
		direction = Mover::UP;
		break;
	case 2:
		direction = Mover::RIGHT;
		break;
	case 3:
		direction = Mover::DOWN;
		break;
	default:
		break;
	}

	return 0;

/*
#pragma region OldCode
	if (m_gps.state()) {
		m_gps.nextPosition(position, direction);
		return 0;
	}

	int viewRange = (m_level >= 25 ? 25 : m_level) / 5 + 5;

	auto rect = createViewRange(position, viewRange);
	if (newTarget(position, rect) == -1) {
		while (true) {
			auto p = random(rect);
			if (collisionCheck(p))
				if (!m_gps.navigation(position, p))
					break;
		}
	}

	return -1;
#pragma endregion
*/
}


void AI::userEventHookProc(const SDL_UserEvent & event)
{
	switch (event.type)
	{
	case FIRE:
		m_tank->fire();
		break;
	default: 
		break;
	}
}

Uint32 AI::fireTimerCallback(Uint32 interval, void * param)
{
	AI *ai = reinterpret_cast<AI *>(param);

	SDL_UserEvent user;
	user.type = FIRE;
	ai->userEventTrigger(user);
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> uid(ai->m_fireIntervalMin, ai->m_fireIntervalMax);
	return uid(gen);
}

bool AI::requestFire()
{

	return false;
}


#pragma region OldCode
SDL_Point AI::random(const SDL_Rect & range)
{
	std::srand(Timer::current() + m_tank->pixelPosition().x + range.x / (range.y + 1));
	std::uniform_int_distribution<> disX(range.x, range.x + range.w - 1);
	std::random_device rd;  // 将用于为随机数引擎获得种子
	std::mt19937 gen(rd()); // 以播种标准 mersenne_twister_engine
	std::uniform_int_distribution<> disY(range.y, range.y + range.h - 1);
	return SDL_Point{ std::rand() % (range.w - 1) + range.x, disY(gen) };
}

SDL_Rect AI::createViewRange(const SDL_Point & center, int viewRange)
{
	SDL_Rect rect;
	rect.x = (center.x - viewRange / 2 + 1 < 0) ? 0 : center.x - viewRange / 2 + 1;
	rect.y = (center.y - viewRange / 2 + 1 < 0) ? 0 : center.y - viewRange / 2 + 1;
	rect.w = (rect.x + viewRange > MAP_SIZE - 1) ? MAP_SIZE - 1 - rect.x : viewRange;
	rect.h = (rect.y + viewRange > MAP_SIZE - 1) ? MAP_SIZE - 1 - rect.y : viewRange;
	return rect;
}

int AI::newTarget(const SDL_Point &current, const SDL_Rect & range)
{
	for (auto x = range.x; x != range.x + range.w; ++x) {
		for (auto y = range.y; y != range.y + range.h; ++y) {
			if (m_tank->ground()->maps()(x, y) == Maps::TAG_BASE) {
				if (!m_gps.navigationToCollimationLine(current, m_tank->ground()->maps().basePosition()))
					return 0;
			}
		}
	}

	std::vector<Tank *> result;
	if (m_tank->ground()->findTankOnRect(m_tank, range, result) != 0) {
		for (auto &t : result) {
			Games *g = (Games *)director;
			if (t->camp() != m_tank->camp())
				if (!m_gps.navigationToCollimationLine(current, t->position()))
					return 0;
		}
	}

	return -1;
}

bool AI::collisionCheck(const SDL_Point & p)
{
	for (auto x = p.x; x != p.x + 2; ++x) {
		for (auto y = p.y; y != p.y + 2; ++y) {
			if (tank()->ground()->colMap()[x][y])
				return false;
		}
	}
	return true;
}

void AI::newTarget(const SDL_Point & position, Uint32 timestamp)
{
	std::default_random_engine eX(Timer::SDLTimer() + m_tank->pixelPosition().x + position.y);
	std::uniform_int_distribution<> disX(0, MAP_SIZE - 2);

	SDL_Point target;
	do {
		do {
			target.x = disX(eX);
			std::srand(Timer::SDLTimer() + m_tank->pixelPosition().y + position.x);
			target.y = std::rand() % (MAP_SIZE - 2);
		} while (!collisionCheck(target));
	} while (m_gps.navigation(position, target));


}

#pragma endregion

