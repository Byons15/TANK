#pragma once
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include "Spirit.h"

class Renderer;
struct SDL_Texture;

/*
Scene(����)��
Scene��Spirit���������࣬һ��Scene������һ�龫������Ҫ�Ĺ�ͬ���ݣ�����������������Ⱦ������������λ
���������㣬�������յ�Ŀ����Ⱦ����Spirit��ȱ��Sceneʱ���޷�������Ⱦ��SceneͬʱҲ�ǽӿڣ�������̳�
������ʵ��update()��render()����Ⱦ��������ÿ֡����һ��update��render����Ⱦ�����Ȼ����update������
��Ӧ���ں����и���ÿ֡�����ݡ�֮����Ⱦ������render()���������ں����в��þ��鲢��Ⱦ����������ʵ���ҽ�
����update������������ݸ��£�����render����Ⱦ���顣

���ݿ��Կ����л������������������ ����ʼ�˵��� - ����Ϸ���桱����ĳ���ת����
������ʵ��open.close.
*/
class Scene
{
	friend class Renderer;
public:
	Scene(Renderer *renderer = 0, const SDL_Rect &sceneRect = {0, 0, 0, 0});
	virtual ~Scene();

	//���ó���������ɫ��
	//�ɹ�����0�� ����-1��ʾû��������Ⱦ����
	int setBackdropColor(const SDL_Color &color);
	Renderer *renderer() {return m_renderer;}
	void setRect(const SDL_Rect &rect);
	const SDL_Rect &rect() const {return m_rect;}
	bool state() const {return m_state;}

	void show(void *data, int code)
	{
		setState(true);
		open(data, code);
	}
	void hide()
	{
		setState(false);
		close();
	}

protected:
	virtual void update(Uint32 time) = 0;

	//������ʵ�֡�
	//����spirit��Ⱦ������
	virtual int render() = 0;

	virtual void open(void *data, int code) = 0;
	virtual void close() = 0;

	SDL_Texture *backdrop(SDL_Rect *rect);
	int setState(bool state);
private:
	SDL_Rect m_rect;
	SDL_Texture *m_backdrop;
	Renderer *m_renderer;
	bool m_state;
};

