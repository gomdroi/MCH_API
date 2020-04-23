#pragma once
#include "equipment.h"
class agahnim_bubble :
	public equipment
{
private:

	int _bubbleFrameCount;
	int _bubbleFrameIndex;
	float startTime;

	int bubbleNum;

	tileMap curTileMap;

public:

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void Move();
	virtual void Collision();
	virtual void animation();
	virtual void pop();
public:
	agahnim_bubble();
	~agahnim_bubble();
};

