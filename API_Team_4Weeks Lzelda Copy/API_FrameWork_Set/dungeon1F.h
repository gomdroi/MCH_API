#pragma once
#include "scene.h"

enum DUNGEON1F_MOVE
{
	DUNGEON1F_MOVE_NULL = -1,
	DUNGEON1F_MOVE_DUNGEON2F,
	DUNGEON1F_MOVE_GAMESCENE4,
	DUNGEON1F_MOVE_DUNGEON1F_1_2,
	DUNGEON1F_MOVE_DUNGEON1F_2_1,
	DUNGEON1F_MOVE_DUNGEON1F_2_3,
	DUNGEON1F_MOVE_DUNGEON1F_3_2,
	DUNGEON1F_MOVE_END
};

class dungeon1F:public scene
{
public:
	dungeon1F();
	~dungeon1F();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();
};

