#pragma once
#include "scene.h"

enum GAMESCENE3_MOVE
{
	GAMESCENE3_MOVE_NULL = -1,
	GAMESCENE3_MOVE_GAMESCENE2,
	GAMESCENE3_MOVE_GAMESCENE4,
	GAMESCENE3_MOVE_END
};

class gameScene3 :
	public scene
{
public:
	gameScene3();
	~gameScene3();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();
};

