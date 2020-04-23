#pragma once
#include "objects.h"
enum TILE_TYPE
{
	TILE_EMPTY,
	TILE_PASSAGE,
	TILE_MONSTER,
	TILE_EVENT,

};
class tile : public objects
{
private:
	POINT tileStartPosition;
	TILE_TYPE tileType;
	int tileIndex;
	bool isSelected;
	bool isForbid;
public:

	POINT getStartPosition() { return tileStartPosition; }
	void setStartPosition(POINT _startPosition) { tileStartPosition = _startPosition; }
	TILE_TYPE getTileType() { return tileType; }
	void setTileType(TILE_TYPE _tileType) { tileType = _tileType; }
	bool getIsSelected() { return isSelected; }
	void setIsSelected(bool _selected) { isSelected = _selected; }
	bool getIsForbid() { return isForbid; }
	void setIsForbid(bool _isforbid) { isForbid = _isforbid; }
	int getTileIndex() { return tileIndex; }
	void setTileIndex(int _tileindex) { tileIndex = _tileindex; }
	
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	tile();
	~tile();
};

