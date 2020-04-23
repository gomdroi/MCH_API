#pragma once
#include"gameNode.h"

class player;
class tile;
class mainGame :  public gameNode
{
private:
	player* m_player;
	

	char mousePointerStr[128];
	bool m_isDebug;
	bool isOpening;
public:
	mainGame();
	~mainGame();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	void stageBackRender(int _stage);
	
};

