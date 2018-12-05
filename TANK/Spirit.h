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

	size_t addAnimation(const Animation &a);
	size_t addAnimation(const std::string &name);
	int showAnimation(const Animation &a);
	int showAnimation(const std::string &name);
	int showAnimation(size_t index = 0);
	int removeAnimation(int index);

	void update(Uint32 time);
	void render(const SDL_Point &position);

	void setZoom(Uint32 width, Uint32 height) {
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
	std::vector<Animation> m_animationPool;
	SIZE m_renderSize;
	Uint32 m_angle, m_currAnimation;
	Scene *m_scene;
	Uint8 m_alpha;
};

