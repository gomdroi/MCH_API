#pragma once
#include "scene.h"

enum GAMESCENE2_MOVE
{
	GAMESCENE2_MOVE_NULL = -1,
	GAMESCENE2_MOVE_SHOP,
	GAMESCENE2_MOVE_GAMESCENE1,
	GAMESCENE2_MOVE_GAMESCENE3,
	GAMESCENE2_MOVE_END
};

class gameScene2 :
	public scene
{
public:
	gameScene2();
	~gameScene2();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();
};

