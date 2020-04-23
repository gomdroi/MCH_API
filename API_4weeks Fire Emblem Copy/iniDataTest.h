#pragma once
#include "gameNode.h"
class iniDataTest : public gameNode
{
public:
	iniDataTest();
	~iniDataTest();

	HRESULT init();
	void release();
	void update();
	void render();
};

