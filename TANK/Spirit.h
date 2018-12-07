#pragma once
#include <string>
#include "SDL\AnimationFactory.h"
#include <vector>

class Scene;

class Spirit
{
public:
	Spirit()
		:m_angle(0), m_alpha(SDL_ALPHA_OPAQUE), m_scene(0) {}
	Spirit(Scene *scene, const std::string &animationName = "");
	virtual ~Spirit();

	int setScene(Scene *scene);
	Scene *scene() {
		return m_scene;
	}

	virtual void update(Uint32 time);
	void render(const SDL_Point &position);

	void setRenderSize(Uint32 width, Uint32 height) {
		m_renderSize.h = height;
		m_renderSize.w = width;
	}
	SIZE renderSize() {
		return m_renderSize;
	}
	void setAlpha(Uint8 a) {
		m_alpha = a;
	}
	void setAngle(Uint32 a) {
		m_angle = a;
	}

private:
	Animation m_animation;
	SDL_Rect m_clipRect;
	SIZE m_renderSize;
	Uint32 m_angle;
	Scene *m_scene;
	Uint8 m_alpha;
};

