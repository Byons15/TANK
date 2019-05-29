#include "..\Spirit.h"
#include "../Scene.h"
#include "../Renderer.h"
#include <SDL_timer.h>
#include "../Director.h"

Spirit::Spirit(Scene * scene, const std::string & animationName)
	:Spirit()
{
	m_scene = scene;
	if(!animationName.empty() && scene->renderer())
		setAnimation(scene->renderer()->animationFactory().findAnimation(animationName));
}

Spirit::~Spirit()
{
}

int Spirit::setScene(Scene * scene)
{
	if (m_scene)
		return -1;

	m_scene = scene;
	return 0;
}

void Spirit::updateFrames(Uint32 time)
{
	m_clipRect = m_animation.clipRect;
	if (m_animation.frameCount > 1) {
		int currItem = ((time - m_animationFirstTime) % (m_animation.frameCount * m_animation.frameTimeInterval)) / m_animation.frameTimeInterval;
		m_clipRect.x = (m_animation.frameInterval * currItem) + m_animation.clipRect.x;
	}

	if (m_flicker) {
		auto t = SDL_abs(time - m_startFlickerTime) % (m_flickerHideTime + m_flickerShowTime);
		Uint8 alpha= (t > m_flickerShowTime) ? 0 : 255;
		setAlpha(alpha);
	}
}

void Spirit::renderFrame(const SDL_Point & position) const
{
	if (!m_scene)
		return;

	SDL_Rect renderRect{ m_scene->rect().x + position.x, m_scene->rect().y + position.y,
						 m_renderSize.w, m_renderSize.h };

	m_scene->renderer()->renderTexture(m_animation.texture, renderRect, m_clipRect, m_angle, m_alpha);
}

void Spirit::setAnimation(const Animation & a)
{
	m_animation = a;
	m_renderSize.w = a.clipRect.w;
	m_renderSize.h = a.clipRect.h;

	if(a.frameCount > 1)
		m_animationFirstTime = timer.current();

	updateFrames(Timer::current());
}

void Spirit::setAnimation(const std::string & name)
{
	setAnimation(m_scene->renderer()->animationFactory().findAnimation(name));
}

void Spirit::startFlicker(Uint32 showTime, Uint32 hideTime)
{
	m_startFlickerTime = timer.current();
	m_flickerShowTime = showTime;
	m_flickerHideTime = hideTime;
	m_flicker = true;
}
