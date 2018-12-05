#include "..\Spirit.h"
#include "../Scene.h"
#include "../Renderer.h"

Spirit::Spirit(Scene * scene, const std::string & animationName)
	:Spirit()
{
	m_scene = scene;
	addAnimation(animationName);
}

Spirit::~Spirit()
{
}

int Spirit::setScene(Scene * scene)
{
	if (m_scene)
		return -1;

	if (!scene)
		return -2;

	m_scene = scene;
	return 0;
}

size_t Spirit::addAnimation(const Animation & a)
{
	m_animationPool.push_back(a);
	return m_animationPool.size() - 1;
}

size_t Spirit::addAnimation(const std::string & name)
{
	return addAnimation(m_scene->renderer()->animationFactory().findAnimation(name));
}

int Spirit::showAnimation(const Animation & a)
{
	return showAnimation(addAnimation(a));;
}

int Spirit::showAnimation(const std::string & name)
{
	return showAnimation(addAnimation(name));
}

int Spirit::showAnimation(size_t index)
{
	if (index < m_animationPool.size()) {

		m_renderSize.h = m_animationPool[index].clipRect.h;
		m_renderSize.h = m_animationPool[index].clipRect.w;

		m_currAnimation = index;
		return 0;
	}

	return -1;
}

int Spirit::removeAnimation(int index)
{
	if (index < 0 || index >= m_animationPool.size())
		return -1;

	m_animationPool.erase(m_animationPool.begin() + index);

	if (m_currAnimation == index)
		m_currAnimation = 0;

	return 0;
}

void Spirit::update(Uint32 time)
{
	if (m_animationPool.empty())
		return;

	Uint32 &interval = m_animationPool[m_currAnimation].frameInterval;
	Uint32 &count = m_animationPool[m_currAnimation].frameCount;
	int &x = m_animationPool[m_currAnimation].clipRect.x;
	auto &rect = m_animationPool[m_currAnimation].clipRect;

	if (count > 1) {
		int currItem = (time % (count * interval)) / interval;
		rect.x = (rect.w * currItem) + x;
	}
}

void Spirit::render(const SDL_Point & position)
{
	if (!m_scene)
		return;

	SDL_Rect renderRect{ m_scene->rect().x + position.x, m_scene->rect().y + position.y,
						 m_renderSize.w, m_renderSize.h};

	auto &a = m_animationPool[m_currAnimation];
	m_scene->renderer()->renderTexture(a.texture, renderRect, a.clipRect, m_angle, m_alpha);
}
