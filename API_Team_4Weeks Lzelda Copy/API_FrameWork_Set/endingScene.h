#pragma once
#include "scene.h"

class endingScene :public scene
{
private:
	image* _ending;
	animation *_ani;
	image* _img;

	int startCount;
	int count;
	int index;

	bool start;
public:
	endingScene();
	~endingScene();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();

};

