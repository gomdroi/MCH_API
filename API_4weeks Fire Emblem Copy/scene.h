#pragma once
#include "gameNode.h"
class scene: public gameNode
{
protected:
	image* bgMap;

public:
	scene();
	~scene();

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();
};

