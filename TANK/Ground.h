#pragma once
#include "Scene.h"
#include "games\Maps.h"
#include "Event.h"
#include "games\Tank.h"
#include <map>
#include "games\TankFactory.h"

class Tank;

class Ground :
	public Scene,
	public EventInterface
{
public:

	enum CAMP
	{
		ALLISE,
		ENEMY,
	};

	Ground(Renderer *renderer);
	~Ground();

	virtual void open(void *data, int code) override;
	virtual void close() override;

	//����ͼ
	Maps &maps() {
		return m_maps;
	}

	void addTank(int tankModel, CAMP camp, int bindIndex);

	//����̹�ˡ�
	//̹���Ѵݻٷ���0�� ̹�������˷���1�� ̹�˴����޵�״̬����-1
	int attackTank(Tank *tank, int power);

protected: 
	virtual void update(Uint32 time) override;
	virtual int render() override;
	virtual void userEventHookProc(const SDL_UserEvent &user) override;

private: friend class Tank;
	//����̹��λ�á�
	//�˹���ֻ����Tank����ã������˵��û����ս������
	//�ɹ�����0�� ������ײ�޷������򷵻�-1
	int tankPositionUpdate(Tank *tank, const SDL_Point &pos);

private:
	void clearRadar();

	Maps m_maps;
	std::array<std::array<Tank *, MAP_SIZE>, MAP_SIZE> m_radar;  //�״ﱣ�����ĸ������ϴ���̹�ˡ�
	std::map<Tank *, SDL_Point> m_tanks;  //valֵ�������̹����ս�������е�λ�õ����Ͻǡ�

	TankFactory m_tankFactory;
};


SDL_Rect pixelToGroundRect(const SDL_Rect & pixelRect);
SDL_Point pixelToGroundPoint(const SDL_Point & pixelPoint);