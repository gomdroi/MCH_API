#pragma once
#include "equipment.h"
class agahnim_bolt :
	public equipment
{
private:
	int _boltFrameCount;
	int _boltFrameIndex;

	float _postPositionX2;
	float _postPositionY2;

	float _postPositionX3;
	float _postPositionY3;

	float _postPositionX4;
	float _postPositionY4;

	float _postPositionX5;
	float _postPositionY5;

	float startTime;
	float boltTailTime;

	bool _isPPX2Fill;
	bool _isPPX3Fill;
	bool _isPPX4Fill;
	bool _isPPX5Fill;

	int PPX2Timer;
	int PPX3Timer;
	int PPX4Timer;
	int PPX5Timer;

	tileMap curTileMap;
public:

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void Move();
	virtual void Collision();
	virtual void animation();
	agahnim_bolt();
	~agahnim_bolt();
};

