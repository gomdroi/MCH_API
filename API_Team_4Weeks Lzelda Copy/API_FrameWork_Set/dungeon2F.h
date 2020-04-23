#pragma once
#include "scene.h"

enum DUNGEON2F_MOVE
{
	DUNGEON2F_MOVE_NULL = -1,
	DUNGEON2F_MOVE_DUNGEON1F,
	DUNGEON2F_MOVE_GAMESCENE4,
	DUNGEON2F_MOVE_DUNGEON1F_1_2,
	DUNGEON2F_MOVE_DUNGEON1F_2_1,
	DUNGEON2F_MOVE_DUNGEON1F_2_3,
	DUNGEON2F_MOVE_DUNGEON1F_3_2,
	DUNGEON2F_MOVE_END
};

class dungeon2F:public scene
{

public:
	dungeon2F();
	~dungeon2F();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();
};

