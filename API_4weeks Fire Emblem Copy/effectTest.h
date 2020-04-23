#pragma once
#include"gameNode.h"
#include"effect.h"
class effectTest :  public gameNode
{
private:
	effect * _effect;
	effect* _effect1;
	//·£´ý¿ë º¯¼ö
	int count;
public:
	effectTest();
	~effectTest();


	HRESULT init();
	void release();
	void update();
	void render();


};

