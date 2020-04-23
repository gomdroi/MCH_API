#pragma once
#include "scene.h"

enum GAMESCENE1_MOVE
{
	GAMESCENE1_MOVE_NULL=-1,
	GAMESCENE1_MOVE_HOME,
	GAMESCENE1_MOVE_GAMESCENE2,
	GAMESCENE1_MOVE_BOSS2,
	GAMESCENE1_MOVE_END
};

class gameScene1 :
	public scene
{
public:
	gameScene1();
	~gameScene1();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();
};

