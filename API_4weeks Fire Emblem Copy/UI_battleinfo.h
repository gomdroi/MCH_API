#pragma once
#include "UI_Set.h"
#include "progressBar.h"
class tile;
class unitSet;
class UI_battleinfo :
	public UI_Set
{
private:
	RECT rc_battleInfo;
	image* battleInfo;
	RECT rc_allyHpStr;
	RECT rc_enemyHpStr;
	RECT rc_allyNameStr;
	RECT rc_enemyNameStr;

	char allyHpStr[256];
	char enemyHpStr[256];

	progressBar* allyHpBar;
	progressBar* enemyHpBar;

	
public:
	UI_battleinfo();
	~UI_battleinfo();

	virtual HRESULT init();
	virtual void update();
	virtual void animation();
	virtual void render();
	virtual void release();

};

