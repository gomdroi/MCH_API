#pragma once
#include "scene.h"

enum SHOPSCENE_MOVE
{
	SHOPSCENE_MOVE_NULL = -1,
	SHOPSCENE_MOVE_GAMESCENE2,
	SHOPSCENE_MOVE_END
};

class shopScene :public scene
{
private:
	bool _isMove;
	int count;
	int index;

	image* shopEnter;
public:
	shopScene();
	~shopScene();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();


};

