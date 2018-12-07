#include "..\Spirit.h"
#include "../Scene.h"
#include "../Renderer.h"

Spirit::Spirit(Scene * scene, const std::string & animationName)
	:Spirit()
{
	m_scene = scene;
	m_animation = scene->renderer()->animationFactory().findAnimation(animationName);
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

void Spirit::update(Uint32 time)
{
	if (m_animation.frameCount > 1) {
		int currItem = (time % (m_animation.frameCount * m_animation.frameInterval)) / m_animation.frameInterval;
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
