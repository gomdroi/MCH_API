#pragma once
#include "scene.h"

enum GAMESCENE4_MOVE
{
	GAMESCENE4_MOVE_NULL = -1,
	GAMESCENE4_MOVE_DUNGEON,
	GAMESCENE4_MOVE_GAMESCENE3,
	GAMESCENE4_MOVE_END
};

class gameScene4 :
	public scene
{
public:
	gameScene4();
	~gameScene4();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();
};

