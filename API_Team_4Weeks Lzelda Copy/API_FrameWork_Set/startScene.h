#pragma once
#include "scene.h"
#define BUTTON 3

class startScene : public scene
{
private:
	int _count;
	int next;

	bool startButton;

	int _aniCount;
	int _index;
	int selectNum; // 선택버튼 갯수 카운트
	image* _angel;
	RECT _angelRc;

	RECT select[BUTTON];
public:
	startScene();
	~startScene();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();

	void angelAni();
};

