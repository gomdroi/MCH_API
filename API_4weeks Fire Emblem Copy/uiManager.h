#pragma once
#include "singletonBase.h"
class UI_Set;
class UI_tileInfo;
class UI_command;
class UI_battleinfo;
class UI_turn;
class uiManager :
	public singletonBase<uiManager>
{
private:
	UI_tileInfo* tileInfo;
	UI_command* command;
	UI_battleinfo* battleinfo;
	UI_turn* turnAni;
public:
	uiManager();
	~uiManager();

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	UI_tileInfo* getTileInfo() { return tileInfo; }
	UI_command* getCommand() { return command; }
	UI_battleinfo* getBattleInfo() { return battleinfo; }
	UI_turn* getTrunAni() { return turnAni; }
};

