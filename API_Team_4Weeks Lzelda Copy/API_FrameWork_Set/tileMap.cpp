#include "stdafx.h"
#include "tileMap.h"


tileMap::tileMap()
{
}


tileMap::~tileMap()
{
}

HRESULT tileMap::init()
{
	vector<tagTile> tileTemp;

	_map.clear();

	for (int i = 0; i < TILEY; i++)
	{
		tileTemp.clear();
		for (int j = 0; j < TILEX; j++)
		{
			tagTile temp;
			temp.terrainFrameX = 0;
			temp.terrainFrameY = 0;
			temp.objFrameX = 0;
			temp.objFrameY = 0;
			temp.terrainImageNum = 0;
			temp.objImageNum = 0;
			temp.terrain = TR_GRASS;
			temp.obj= OBJ_NONE;
			temp.rc = RectMake(j* TILESIZE,i*TILESIZE, TILESIZE, TILESIZE);
			tileTemp.push_back(temp);
		}
		_map.push_back(tileTemp);
	}

	return S_OK;
}

HRESULT tileMap::init(int sizeX, int sizeY)
{
	vector<tagTile> tileTemp;

	_map.clear();

	for (int i = 0; i < sizeY; i++)
	{
		tileTemp.clear();
		for (int j = 0; j < sizeX; j++)
		{
			tagTile temp;
			temp.terrainFrameX = 0;
			temp.terrainFrameY = 0;
			temp.objFrameX = 0;
			temp.objFrameY = 0;
			temp.terrainImageNum = 0;
			temp.objImageNum = 0;
			temp.terrain = TR_GRASS;
			temp.obj = OBJ_NONE;
			temp.rc = RectMake(j* TILESIZE, i*TILESIZE, TILESIZE, TILESIZE);
			tileTemp.push_back(temp);
		}
		_map.push_back(tileTemp);
	}

	_mapSizeX = sizeX;
	_mapSizeY = sizeY;

	return S_OK;
}

HRESULT tileMap::initTileMap(int sizeX, int sizeY, int tileSizeX, int tileSizeY, image* setImg)
{
	vector<tagTile> tileTemp;

	_map.clear();
	_interactionObject.clear();

	for (int i = 0; i < sizeY; i++)
	{
		tileTemp.clear();
		for (int j = 0; j < sizeX; j++)
		{
			tagTile temp;
			temp.terrainFrameX = 0;
			temp.terrainFrameY = 0;
			temp.objFrameX = 0;
			temp.objFrameY = 0;
			temp.terrainImageNum = 0;
			temp.objImageNum = 0;
			temp.terrainImg = setImg;
			if (temp.terrainImg == NULL)
			{
				temp.terrain = TR_NONE;
			}
			else
			{
				temp.terrain = TR_GRASS;
			}
			temp.obj = OBJ_NONE;
			temp.rc = RectMake(j* tileSizeX, i*tileSizeY, tileSizeX, tileSizeY);
			tileTemp.push_back(temp);
		}
		_map.push_back(tileTemp);
	}

	_mapSizeX = sizeX;
	_mapSizeY = sizeY;

	return S_OK;
}

void tileMap::update()
{
}

void tileMap::render()
{
}

void tileMap::release()
{
	_map.clear();
}

int tileMap::getMapObj(int x, int y)
{
	int tempX, tempY;

	if (x < 0)
	{
		tempX = 0;
	}
	else if (x >= _mapSizeX)
	{
		tempX = _mapSizeX - 1;
	}
	else
	{
		tempX = x;
	}

	if (y < 0)
	{
		tempY = 0;
	}
	else if (y >= _mapSizeY)
	{
		tempY = _mapSizeY - 1;
	}
	else
	{
		tempY = y;
	}

	 return _map[tempY][tempX].obj;
}

void tileMap::addInteractionObject(int xPos, int yPos, int widthPos, int heightPos, RECT rc, INTERACTIONOBJECT obj, image* img, int imageNum)
{
	tagInteractionObject objTemp;

	objTemp.xPos = xPos;
	objTemp.yPos = yPos;
	objTemp.widthPos = widthPos;
	objTemp.heightPos = heightPos;
	objTemp.obj = obj;
	objTemp._img = img;
	objTemp._rc = rc;
	objTemp.imageNum = imageNum;

	_interactionObject.push_back(objTemp);
}

void tileMap::deleteInteractionObject(int num)
{
	_interactionObject.erase(_interactionObject.begin()+num);

	RECT rectTemp = _player.getRect();
}
