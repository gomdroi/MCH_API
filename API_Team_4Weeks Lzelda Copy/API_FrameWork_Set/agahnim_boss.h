#pragma once
#include "enemySet.h"
class agahnim_boss :
	public enemySet
{
private:

	image *_boltImg;
	image *_lightningImg;
	image *_imgBlack;

	RECT lightning[6];

	float startTime;
	float actionTime;


	bool _isAttack;
	bool _isBossOn;
	bool _isMoveSet;
	bool _isFading;

	bool _isCastPrep;
	int _castPrepCount;

	bool _isCastLightning;
	float lightningTime;
	float lightningCount;

	bool _isCastBolt;
	float _boltTime;

	bool _isCastBubble;


	int _monsterFrameIndex2;
	int _monsterFrameCount2;

	int _boltFrameIndex;
	int _boltFrameCount;

	int _alpha = 255;
	int _alpha2 = 0;

public:

	agahnim_boss();
	~agahnim_boss();

	virtual HRESULT init();
	virtual void setEnemy(POINT position);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	//기타 함수 모음
	virtual void animation();
	virtual void collision();
	virtual void heading();
	virtual void move();

	virtual RECT* getLightningRect() { return lightning; }

};

