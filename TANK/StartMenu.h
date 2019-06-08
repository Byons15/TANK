#pragma once
#include "Scene.h"
#include "Event.h"
#include "Text.h"

/*
��ʼ�˵�
��ʼ�˵��̳г������¼��ӿڡ�
��ʼ�˵��ڹر�ʱ�ᴥ��һ��"CLOSE"User�¼�������:
data1�����Ϸģʽ 1(����ģʽ) or 2(�Զ���ս��)
data2�����Ҹ��� 1 or 2
*/
class StartMenu :
	public Scene,
	public EventInterface
{
public:

	//��ʼ�˵��رա�
	//data1 :��Ϸģʽ�� 1(����ģʽ) or 2(�Զ����ͼ)
	//data2 :��Ҹ�����
	static constexpr int CLOSE = 0x155230;

	StartMenu(Renderer *renderer);
	~StartMenu();


protected:
	virtual void update(Uint32 time) override;
	virtual int render() override;
	virtual void open(void *data, int code) override;
	virtual void close() override;
	virtual void eventHookProc(const SDL_Event & event) override;

private:
	void switchMenu(int menuLayer);
	

	Spirit m_logo, m_cursor;
	Text m_combo[3];
	SDL_Point m_cursorViewPosition[3], m_menuItemPosition[3], m_logoPosition;
	int m_menuLayer, m_currItem, m_gameModel, m_playerMember;
};

