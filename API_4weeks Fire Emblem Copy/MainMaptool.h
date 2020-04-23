#pragma once
#include "gameNode.h"

class tile;
class MainMaptool :
	public gameNode
{
private:
	vector<tile*> maptoolVector;
	image* stageBack;

	int mapMaxX;
	int mapMaxY;
	int mapSize;

	POINT SetMptmouse;

	HANDLE file;												//세이브, 로드을 위한 변수
	DWORD write;
	DWORD read;
	TERRAIN_TYPE savedType;
	UNITTYPE savedUnit;

public:
	MainMaptool();
	~MainMaptool();

	virtual HRESULT init();
	virtual void init(STAGE_NUMBER _stagenumber);
	virtual void update();
	virtual void render();
	virtual void release();

	virtual void control();
	virtual void save();
	virtual void load();

	void setStageBack(image* _stageback) { stageBack = _stageback; }

};

