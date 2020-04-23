#pragma once
#include "scene.h"
class stage3 :
	public scene
{
public:
	stage3();
	~stage3();

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();
};

