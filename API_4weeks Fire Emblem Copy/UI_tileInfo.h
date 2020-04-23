#pragma once
#include "UI_Set.h"
#include "progressBar.h"
class tile;
class UI_tileInfo :
	public UI_Set
{
private:

	//타일 정보
	RECT rc_tileFront;
	RECT rc_tileNameBack;
	RECT rc_tileNameStr;
	RECT rc_tileDefStr;
	RECT rc_tileAvoStr;
	
	image* tileNameBack;
	image* tileFront;
	
	//타일에 있는 유닛 정보
	RECT rc_unitFront;
	RECT rc_unitSmallMugShot;
	RECT rc_unitHpStr;
	char unitHpStr[256];
	RECT rc_unitNameStr;
	image* unitFront;
	image* unitHp;
	
	progressBar* hpBar;


public:
	UI_tileInfo();
	~UI_tileInfo();

	virtual HRESULT init();
	virtual void update();
	virtual void animation();
	virtual void render();
	virtual void release();
};

