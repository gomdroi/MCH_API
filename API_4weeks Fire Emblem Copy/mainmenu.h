#pragma once
#include "scene.h"
class mainmenu :
	public scene
{
private:
	image* dragonCursor;
	POINT cursorPos;
	int m_cursorCount;
	int m_cursorIndex;
	int aniDelayTime;

	bool isForwards;
public:
	mainmenu();
	~mainmenu();

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	virtual void animation();
	virtual void control();
};

