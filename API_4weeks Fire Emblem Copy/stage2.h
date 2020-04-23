#pragma once
#include "scene.h"
class stage2 :
	public scene
{
public:
	stage2();
	~stage2();

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();
};

