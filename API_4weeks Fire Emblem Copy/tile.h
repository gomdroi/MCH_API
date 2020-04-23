#pragma once
#include "objects.h"

#define BIGNUM 5000
class unitSet;
class tile :
	public objects
{
private:

	TERRAIN_TYPE terrainType;
	TILE_TYPE tileType;
	unitSet* onTileUnit;
	UNITDEF onTileUnitType;
	image* blueTile;
	image* redTile;
	UNITTYPE settingUnitType;

	bool listOn;			//openlist에 추가 되었는지 여부
	int F, G, H;
	char str[128];
	tile* parent;			//부모 타일 주소

	int tileIndexX;			
	int tileIndexY;			

	int def;
	int avoid;

	char tileNameStr[256];
	char tileDefStr[256];
	char tileAvoStr[256];

	bool showBlueTile;
	bool showRedTile;

	char moveStr[256];

	int tileFrameCount;
	int tileFrameIndex;

public:
	tile();
	~tile();

	virtual HRESULT init();
	virtual void update();
	virtual void animation();
	virtual void render();
	virtual void release();
	virtual void setTile(TERRAIN_TYPE _terraintype);
	virtual void maptoolSavedSettingUnit();

	int getTileIndexX() { return tileIndexX; }
	void setTileIndexX(int _tileindexx) { tileIndexX = _tileindexx; }
	int getTileIndexY() { return tileIndexY; }
	void setTileIndexY(int _tileindexy) { tileIndexY = _tileindexy; }

	TILE_TYPE getTileType() { return tileType; }
	void setTileType(TILE_TYPE _tiletype) { tileType = _tiletype; }
	TERRAIN_TYPE getTerrainType() { return terrainType; }
	void setTerrainType(TERRAIN_TYPE _terraintype) { terrainType = _terraintype; }
	UNITTYPE getSettingUnitType() { return settingUnitType; }
	void setSettingUnitType(UNITTYPE _settingunittype) { settingUnitType = _settingunittype; }

	bool getListOn() { return listOn; }
	void setListOn(bool _liston) { listOn = _liston; }
	tile* getParent() { return parent; }
	void setParent(tile* _parent) { parent = _parent; }

	 unitSet* getOnTileUnit() { return onTileUnit; }
	void setOnTileUnit(unitSet* _ontileunit) { onTileUnit = _ontileunit; }
	UNITDEF getOnTileUnitType() { return onTileUnitType; }
	

	bool getShowBlueTile() { return showBlueTile; }
	void setShowBlueTile(bool _showBlueTile) { showBlueTile = _showBlueTile; }
	bool getShowRedTile() { return showRedTile; }
	void setShowRedTile(bool _showRedTile) { showRedTile = _showRedTile; }

	char* getTileNameStr() { return tileNameStr; }
	char* getTileDefStr() { return tileDefStr; }
	char* getTileAvoStr() { return tileAvoStr; }

	int getTileDef() { return def; }
	int getTileAvoid() { return avoid; }

	int getG() { return G; }
	void setG(int _g) { G = _g; }
	int getF() { return F; }
	void setF(int _f) { F = _f; }
	int getH() { return H; }
	void setH(int _h) { H = _h; }
};

