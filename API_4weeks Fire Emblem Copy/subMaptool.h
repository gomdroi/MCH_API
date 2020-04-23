#pragma once
#include"gameNode.h"
class mainMaptool;
class subMaptool : public gameNode
{
private:
	RECT rc_curSelectedTile;
	char curSelectedTileStr[256];
	RECT rc_stageSelect[3];
	RECT rc_stageSave;
	RECT rc_stageLoad;
	RECT rc_tileProperty[TILETYPEMAX];
	RECT rc_unitProperty[UNITTYPEMAX];
	STAGE_NUMBER curStage;

	TERRAIN_TYPE selectedTerrainType;
	UNITTYPE selectedUnitType;

	
public:
	subMaptool();
	~subMaptool();

	virtual HRESULT init();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void release();

	void changeCurSelectedTileStr();

};

