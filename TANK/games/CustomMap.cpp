#include "CustomMap.h"
#include "../TANKS.h"
#include "../FileLoader.h"
constexpr int BOX_A = 0x7FFFFF8F;
constexpr int BOX_B = 0x7FFFFF88;

CustomMap::CustomMap(Renderer * renderer, Ground *ground)
	:Scene(renderer, {0, 0, MAP_SIZE * GRID_SIZE, MAP_SIZE * GRID_SIZE}), m_ground(ground), m_currentTerrainIndex(Maps::TAG_BASE)
{
	setBackdropColor({0, 0, 0, SDL_ALPHA_OPAQUE});
	
	std::vector<std::string> data;
	if (fileLoad("P1control", &data) != 6)
		throw std::runtime_error("‘ÿ»Î∞¥º¸≈‰÷√¥ÌŒÛ");
	
	SDL_Keycode keys[4], boxA, boxB;
	for (size_t i = 0; i != 5; ++i) {
		auto code = SDL_GetKeyFromName(data[i].c_str());
		if (code == SDLK_UNKNOWN)
			throw std::runtime_error("‘ÿ»Î∞¥º¸¥ÌŒÛ");
		if (i < 4)
			keys[i] = code;
		else if (i == 4)
			boxA = code;
		else
			boxB = code;
	}

	m_keyMaps[keys[0]] = Mover::LEFT;
	m_keyMaps[keys[1]] = Mover::RIGHT;
	m_keyMaps[keys[2]] = Mover::UP;
	m_keyMaps[keys[3]] = Mover::DOWN;
	m_keyMaps[boxA] = BOX_A;
	m_keyMaps[boxB] = BOX_B;

	m_curosr.setScene(this);
	auto a = renderer->animationFactory().findAnimation("P1");
	a.frameCount = 1;
	m_curosr.setAnimation(a);
	m_curosr.startFlicker(1000, 200);

	m_curosrPosition = ground->maps().alliesBindPosition(0);
}


CustomMap::~CustomMap()
{
}

void CustomMap::open(void * data, int code)
{
	setState(true);
	setEventHook(SDL_KEYDOWN);
}

void CustomMap::close()
{
	unsetEventHook(SDL_KEYDOWN);
	setState(false);
}

void CustomMap::eventHookProc(const SDL_Event & event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN: {
		auto iter = m_keyMaps.find(event.key.keysym.sym);
		if (iter != m_keyMaps.end()) {
			switch (iter->second)
			{
			case Mover::LEFT:
				m_curosrPosition.x = (m_curosrPosition.x - 1 < 0) ? m_curosrPosition.x : m_curosrPosition.x - 1;
				m_currentTerrainIndex = Maps::TAG_BASE;
			break;
			case Mover::RIGHT:
				m_curosrPosition.x = (m_curosrPosition.x + 1 > MAP_SIZE) ? m_curosrPosition.x : m_curosrPosition.x + 1;
				m_currentTerrainIndex = Maps::TAG_BASE;
			break;
			case Mover::UP:
				m_curosrPosition.y = (m_curosrPosition.y - 1 < 0) ? m_curosrPosition.y : m_curosrPosition.x - 1;
				m_currentTerrainIndex = Maps::TAG_BASE;
			break;
			case Mover::DOWN:
				m_curosrPosition.y = (m_curosrPosition.y + 1 > MAP_SIZE) ? m_curosrPosition.y : m_curosrPosition.x + 1;
				m_currentTerrainIndex = Maps::TAG_BASE;
				break;
			case BOX_A:  //boxA
				for (auto x = m_curosrPosition.x; x != m_curosrPosition.x + 2; ++x) {
					for (auto y = m_curosrPosition.y; y != m_curosrPosition.y + 2; ++y) {
						if(m_currentTerrainIndex == Maps::TAG_BASE)
							m_ground->maps().setTerrain(x, y, 0);
						else
							m_ground->maps().setTerrain(x, y, m_currentTerrainIndex);
					}
				}
				m_currentTerrainIndex = (m_currentTerrainIndex + 1 >= m_ground->maps().terrainTypeMember() - 1) ? 
													Maps::TAG_BASE : m_currentTerrainIndex + 1;
				break;
			case BOX_B:
				close();
				break;
			default:
				break;
			}
		}
	}
	default:
		break;
	}
}

void CustomMap::update(Uint32 time)
{
	for (auto i = 0; i != m_ground->maps().terrainTypeMember(); ++i) {
		m_ground->maps().terrainData(i).spirit.updateFrames(time);
	}
	m_curosr.updateFrames(time);
}

int CustomMap::render()
{
	auto & maps = m_ground->maps();

	bool baseRended = false;
	for (int x = 0; x != MAP_SIZE; ++x) {
		for (int y = 0; y != MAP_SIZE; ++y) {
			if (maps(x, y) == Maps::TAG_BASE && baseRended) {
				continue;
			}
			maps.terrainData(maps(x, y)).spirit.renderFrame({ x * GRID_SIZE, y * GRID_SIZE });
			if(maps(x, y) == Maps::TAG_BASE)
				baseRended = true;
		}
	}

	m_curosr.renderFrame(m_curosrPosition);
}
