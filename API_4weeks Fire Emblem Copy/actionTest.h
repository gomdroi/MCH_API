#pragma once
#include"gameNode.h"
#include"action.h"

static image* testImage = nullptr;
static action* testAction = nullptr;
class actionTest :  public gameNode
{
private:

	image * _image;
	action* _action;

public:
	actionTest();
	~actionTest();

	HRESULT init();
	void update();
	void release();
	void render();

	static void callback();

};

