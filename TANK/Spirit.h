#pragma once
#include <string>
#include "SDL\AnimationFactory.h"
#include <vector>

class Scene;


/*
Spirit(����)��
Spirit���ڴ�����ʾAnimation�����齫������ÿһ֡��Ⱦ���ĳ����У���û�����ó���ʱ�����齫�޷���Ⱦ�ɹ�����Ϊȱ�ٱ�Ҫ���ݡ�
���鲻����Animation::Texture��Ա�������ڡ����黹ΪAnimation�ṩ�����š��Ƕȡ�͸�������ù��ܡ�����updateFrames()�����µ�
ǰ֡���棬���������������Ⱦ���棬Ҫ�����鵱ǰ��֡������Ⱦ�������У������render()��
*/
class Spirit
{
public:
	Spirit()
		:m_angle(0), m_alpha(SDL_ALPHA_OPAQUE), m_scene(0) {}
	Spirit(Scene *scene, const std::string &animationName = "");
	virtual ~Spirit();

	//���ó����� 
	//�ɹ�����0�� ����-1��ʾ�Ѿ����ù��ˣ����ܸ��������� 
	int setScene(Scene *scene);
	Scene *scene() {
		return m_scene;
	}

	void updateFrames(Uint32 time);

	//��Ⱦ��position��
	//position: λ�������scene�������鲻�����κγ���ʱ�� ��Ⱦʧ�ܡ�
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

