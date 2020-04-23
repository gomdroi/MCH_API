#pragma once
#include "scene.h"
class intro :
	public scene
{
public:
	intro();
	~intro();

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	virtual void control();
};

