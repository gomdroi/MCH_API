#include "stdafx.h"
#include "tileManager.h"
#include "tile.h"


tileManager::tileManager()
{
}


tileManager::~tileManager()
{
}

HRESULT tileManager::init()
{
	//스테이지1 테스트 초기화
	for (int i = 0; i < 150; i++)
	{
		tile* mapTile = new tile;
		mapTile->init();
		mapTile->setPosition(PointFloatMake(24+(TILEX*(i % 15)), 24+(TILEY*(i / 15))));
		mapTile->setRc(RectMakeCenter(mapTile->getPosition().x, mapTile->getPosition().y, TILEX, TILEY));
		mapTile->setTileIndexX(i % 15);
		mapTile->setTileIndexY(i / 15);
		mapTile->setTile(TERRAIN_GRASS);
		tileVector.push_back(mapTile);
	}
	return S_OK;
}

void tileManager::init(STAGE_NUMBER _stagenumber)
{
	release();
	switch (_stagenumber)
	{
	case STAGE_ONE:
		for (int i = 0; i < 150; i++)
		{
			tile* mapTile = new tile;
			mapTile->init();
			mapTile->setPosition(PointFloatMake(24 + (TILEX*(i % 15)), 24 + (TILEY*(i / 15))));
			mapTile->setRc(RectMakeCenter(mapTile->getPosition().x, mapTile->getPosition().y, TILEX, TILEY));
			mapTile->setTileIndexX(i % 15);
			mapTile->setTileIndexY(i / 15);
			mapTile->setTile(TERRAIN_GRASS);
			tileVector.push_back(mapTile);
		}
		break;
	case STAGE_TWO:
		for (int i = 0; i < 210; i++)
		{
			tile* mapTile = new tile;
			mapTile->init();
			mapTile->setPosition(PointFloatMake(24 + (TILEX*(i % 15)), 24 + (TILEY*(i / 15))));
			mapTile->setRc(RectMakeCenter(mapTile->getPosition().x, mapTile->getPosition().y, TILEX, TILEY));
			mapTile->setTileIndexX(i % 15);
			mapTile->setTileIndexY(i / 15);
			mapTile->setTile(TERRAIN_GRASS);
			tileVector.push_back(mapTile);
		}
		break;
	case STAGE_THREE:
		for (int i = 0; i < 272; i++)
		{
			tile* mapTile = new tile;
			mapTile->init();
			mapTile->setPosition(PointFloatMake(24 + (TILEX*(i % 17)), 24 + (TILEY*(i / 17))));
			mapTile->setRc(RectMakeCenter(mapTile->getPosition().x, mapTile->getPosition().y, TILEX, TILEY));
			mapTile->setTileIndexX(i % 17);
			mapTile->setTileIndexY(i / 17);
			mapTile->setTile(TERRAIN_GRASS);
			tileVector.push_back(mapTile);
		}
		break;
	default:
		break;
	}
}

void tileManager::update()
{
	for (int i = 0; i < tileVector.size(); i++)
	{
		tileVector[i]->update();
	}
}

void tileManager::render()
{
	for (int i = 0; i < tileVector.size(); i++)
	{
		tileVector[i]->render();
	}
}

void tileManager::release()
{
	vector<tile*>::iterator tIter = tileVector.begin();
	for (tIter; tIter != tileVector.end();)
	{
		(*tIter)->release();
		SAFE_DELETE((*tIter));
		tIter = tileVector.erase(tIter);
	}
}

void tileManager::stageLoad(STAGE_NUMBER _stagenumber)
{
	char str[1000];
	char editTemp[256] = { 0 };
	switch (_stagenumber)
	{
	case STAGE_ONE:
		sprintf_s(editTemp, "maps/stage1.map");
		break;
	case STAGE_TWO:
		sprintf_s(editTemp, "maps/stage2.map");
		break;
	case STAGE_THREE:
		sprintf_s(editTemp, "maps/stage3.map");
		break;
	}

	ifstream inFile(editTemp);

	ZeroMemory(str, sizeof(str));
	inFile.getline(str, 1000);
	sscanf_s(str, "x : %d, y : %d\r\n", &mapMaxX, &mapMaxY);
	mapSize = mapMaxX * mapMaxY;

	for (int i = 0; i < mapSize; i++)
	{
		ZeroMemory(str, sizeof(str));
		inFile.getline(str, 1000);

		sscanf_s(str, "%d,%d\r\n", &savedType,&savedUnit);
		tileVector[i]->setTile(savedType);
		tileVector[i]->setSettingUnitType(savedUnit);
	}

	inFile.close();
}
