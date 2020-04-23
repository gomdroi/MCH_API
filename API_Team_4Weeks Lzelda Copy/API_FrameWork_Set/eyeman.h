#pragma once
#include "enemySet.h"

class eyeman :
	public enemySet
{
private:
	float startTime;
	float actionTime;
	bool _isSleep;
public:
	eyeman();
	~eyeman();

	virtual HRESULT init();
	virtual void setEnemy(ENEMY_COLOR eyemancolor, POINT position);
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

