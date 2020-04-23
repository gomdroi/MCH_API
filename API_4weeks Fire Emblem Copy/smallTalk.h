#pragma once
#include "scene.h"
class smallTalk :
	public scene
{
private:
	image* talking;
	int talkNum;
public:
	smallTalk();
	~smallTalk();

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	virtual void control();

};

