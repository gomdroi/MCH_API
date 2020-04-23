#pragma once
#include "singletonBase.h"
enum STAGE_NUMBER;
enum TERRAIN_TYPE;
enum UNITTYPE;
class tile;
class tileManager :
	public singletonBase<tileManager>
{
private:
	vector<tile*> tileVector;

	int mapMaxX;
	int mapMaxY;
	int mapSize;
	TERRAIN_TYPE savedType;
	UNITTYPE savedUnit;
public:
	tileManager();
	~tileManager();

	virtual HRESULT init();
	virtual void init(STAGE_NUMBER _stagenumber);
	virtual void update();
	virtual void render();
	virtual void release();

	void stageLoad(STAGE_NUMBER _stagenumber);

	vector<tile*> getTileVector() { return tileVector; }

	int getMapMaxX() { return mapMaxX; }
	int getMapMaxY() { return mapMaxY; }
};

