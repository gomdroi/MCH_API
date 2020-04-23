#pragma once
#include "scene.h"
class stage1 :
	public scene
{
public:
	stage1();
	virtual ~stage1();

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();
};

