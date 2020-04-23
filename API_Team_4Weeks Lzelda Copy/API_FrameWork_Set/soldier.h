#pragma once
#include "enemySet.h"

class soldier :
	public enemySet
{
private:
	float startTime;
	float actionTime;

public:
	soldier();
	~soldier();

	virtual HRESULT init();
	virtual void setEnemy(ENEMY_COLOR soldiercolor, POINT position);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	//기타 함수 모음
	virtual void animation();
	virtual void collision();
	virtual void move();
	virtual void heading();
	virtual void detection();
};

