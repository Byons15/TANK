#pragma once
#include <string>
#include "SDL\AnimationFactory.h"
#include <vector>

class Scene;


/*
Spirit(精灵)：
Spirit用于处理并显示Animation，精灵将动画的每一帧渲染到的场景中，当没有设置场景时，精灵将无法渲染成功，因为缺少必要数据。
精灵不负责Animation::Texture成员的生命期。精灵还为Animation提供了缩放、角度、透明度设置功能。调用updateFrames()来更新当
前帧画面，但这个函数不会渲染画面，要将精灵当前的帧画面渲染到场景中，请调用render()。
*/
class Spirit
{
public:
	Spirit()
		:m_angle(0), m_alpha(SDL_ALPHA_OPAQUE), m_scene(0) {}
	Spirit(Scene *scene, const std::string &animationName = "");
	virtual ~Spirit();

	//设置场景。 
	//成功返回0， 返回-1表示已经设置过了，不能更换场景。 
	int setScene(Scene *scene);
	Scene *scene() {
		return m_scene;
	}

	void updateFrames(Uint32 time);

	//渲染到position。
	//position: 位置相对于scene。当精灵不属于任何场景时， 渲染失败。
	void renderFrame(const SDL_Point &position) const;

	void setAnimation(const Animation &a);
	void setAnimation(const std::string &name);

	void setRenderSize(SIZE size) {
		m_renderSize = size;
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
	Uint32 m_angle, m_animationFirstTime;
	Scene *m_scene;
	Uint8 m_alpha;
};

