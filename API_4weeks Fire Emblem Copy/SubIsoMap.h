#pragma once
#include"gameNode.h"
#include"IsoTile.h"
class SubIsoMap :  public gameNode
{
private:

	bool isDebug;

	int currentTile;
	RECT rc[TILE_SIZE_X][TILE_SIZE_Y];

	int frameX, frameY;



public:
	SubIsoMap();
	~SubIsoMap();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);
};

