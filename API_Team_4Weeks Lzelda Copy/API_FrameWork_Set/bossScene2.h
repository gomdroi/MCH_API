#pragma once
#include "scene.h"

enum BOSSSCENE2_MOVE
{
	BOSSSCENE2_MOVE_NULL = -1,
	BOSSSCENE2_MOVE_GAMESCENE1,
	BOSSSCENE2_MOVE_END
};

class bossScene2 :public scene
{
public:
	bossScene2();
	~bossScene2();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();
};

