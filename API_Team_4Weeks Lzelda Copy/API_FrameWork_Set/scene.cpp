#include "stdafx.h"
#include "scene.h"


scene::scene()
{
}


scene::~scene()
{
	release();
}

HRESULT scene::init()
{
	addSampleImageName();

	if (_sceneName == SCENE_NAME_INGAME_SHOP_SCENE)
	{
		_shop.init();
	}
	if (_sceneName == SCENE_NAME_INGAME_SHOP_SCENE)
	{
		_player.aniStop();
	}

	ENEMYMANAGER->release();

	return S_OK;
}

void scene::update()
{
	_sound.update();

	if (_sceneName == SCENE_NAME_INGAME_SCENE || _sceneName == SCENE_NAME_INGAME_SHOP_SCENE)
	{
		ITEMMANAGER->update();
		EFFECTMANAGER->update();

		if (!INVENTORYMANAGER->getIsInventory())
		{
			if (_nowFloor == 1)
			{
				_player.update(_currentMapX*TILESIZE, _currentMapY*TILESIZE, &_firstFloorMap);
			}
			else if (_nowFloor == 2)
			{
				_player.update(_currentMapX*TILESIZE, _currentMapY*TILESIZE, &_secondFloorMap);
			}
			ENEMYMANAGER->update();
		}

		INVENTORYMANAGER->update();

		if (_sceneName == SCENE_NAME_INGAME_SHOP_SCENE)
		{
			_shop.update();
		}
	}
}

void scene::release()
{
	_tileSampleImageNameVector.clear();
	_objSampleImageNameVector.clear();
	_interactionObjectSampleImageNameVector.clear();

	if (_sceneName == SCENE_NAME_INGAME_SCENE || _sceneName == SCENE_NAME_INGAME_SHOP_SCENE)
	{
		clearMoveRc();

		if (_sceneName == SCENE_NAME_INGAME_SHOP_SCENE)
		{
			_shop.release();
		}

		ITEMMANAGER->release();
		EQUIPMENTMANAGER->isActiveOff();
	}
}

void scene::render()
{
	if (_sceneName == SCENE_NAME_INGAME_SCENE || _sceneName == SCENE_NAME_INGAME_SHOP_SCENE)
	{
		//2층일때 랜더링 순서와 1층일때 랜더링 순서를 달리한다.
		if (_nowFloor == 2 || _player.getIsLadder())
		{
			drawTileMap(1, 1, _player.getCameraX(), _player.getCameraY());
			drawTileMap(1, 2, _player.getCameraX(), _player.getCameraY());
			_player.render(_backmap->getMemDC(), false);
		}
		else if (_nowFloor == 1)
		{
			drawTileMap(1, 1, _player.getCameraX(), _player.getCameraY());
			_player.render(_backmap->getMemDC(), false);
			drawTileMap(1, 2, _player.getCameraX(), _player.getCameraY());
		}

		ITEMMANAGER->render(_backmap->getMemDC());
		EFFECTMANAGER->render(_backmap->getMemDC());

		if (_sceneName == SCENE_NAME_INGAME_SHOP_SCENE)
		{
			_shop.render(_backmap->getMemDC());
		}

		if (_isDebug)
		{
			//문
			for (int i = 0; i < _moveRcVector.size(); i++)
			{
				Rectangle(_backmap->getMemDC(), _moveRcVector[i].left, _moveRcVector[i].top, _moveRcVector[i].right, _moveRcVector[i].bottom);
			}
		}

		ENEMYMANAGER->render(_backmap->getMemDC());

		BitBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, _backmap->getMemDC(), _player.getCameraX(), _player.getCameraY(), SRCCOPY);

		UIMANAGER->render(getMemDC(), UI_KIND_PLAYERINFO);
		INVENTORYMANAGER->render(getMemDC());
	}
}

tileMap scene::getTileMap()
{
	return _firstFloorMap;
}

void scene::imageButtonRender(imageButton button)
{
	if (button._isFrame)
	{
		if (button._ischeck)
		{
			button._img->frameRender(getMemDC(), button._rc.left, button._rc.top, button._currentX, 0);
		}
		else
		{
			button._img->frameRender(getMemDC(), button._rc.left, button._rc.top, 0, 0);
		}
	}
	else
	{
		button._img->render(getMemDC(), 330, 200);
	}

	//배경모드
	SetBkMode(getMemDC(), TRANSPARENT);
	//색상
	SetTextColor(getMemDC(), RGB(0, 0, 0));

	TextOut(getMemDC(), 0, 0, button._text.c_str(), button._text.size());
}

void scene::imageButtonPoint(imageButton* button)
{
	if (button->_isFrame)
	{
		if (PtInRect(&button->_rc, m_ptMouse))
		{
			button->_ischeck = true;
			button->_currentX = 1;
		}
		else
		{
			button->_ischeck = false;
			button->_currentX = 0;
		}
	}
}

void scene::sceneLoading(const char * fileName)
{
	char str[1000];

	// 1층로드
	ifstream inFile(fileName);

	//타일맵
	tileMap *tileMapTemp;
	tileMapTemp = new tileMap();
	vector<vector<tagTile>> tileMapVectorTemp;

	//오브젝트
	vector<tagInteractionObject> interactionObjectVectorTemp;
	tagInteractionObject interactionObject_Temp;
	int vectorNum;

	ZeroMemory(str, sizeof(str));
	inFile.getline(str, 1000);
	sscanf_s(str, "x : %d, y : %d\r\n", &_currentMapX, &_currentMapY);

	IMAGEMANAGER->deleteImage("map_image");
	IMAGEMANAGER->deleteImage("backmap");
	//맵 backbuffer선언
	_backmap = IMAGEMANAGER->addImage("backmap", _currentMapX * TILESIZE, _currentMapY * TILESIZE);
	_mapImg = IMAGEMANAGER->addImage("map_image", _currentMapX* TILESIZE, _currentMapY* TILESIZE);

	//타일맵
	_firstFloorMap.initTileMap(_currentMapX, _currentMapY, TILESIZE, TILESIZE, IMAGEMANAGER->findImage(*_tileSampleImageNameVector.begin()));
	_secondFloorMap.initTileMap(_currentMapX, _currentMapY, TILESIZE, TILESIZE, IMAGEMANAGER->findImage(*_tileSampleImageNameVector.begin()));

	tileMapTemp->initTileMap(_currentMapX, _currentMapY, TILESIZE, TILESIZE, NULL);
	tileMapVectorTemp = tileMapTemp->getTileMap();

	for (int i = 0; i < _currentMapY; i++)
	{
		for (int j = 0; j < _currentMapX; j++)
		{
			ZeroMemory(str, sizeof(str));
			inFile.getline(str, 1000);

			sscanf_s(str, "%d ,%d, %d, %d, %d, %d, %d, %d\r\n",
				&tileMapVectorTemp[i][j].terrainImageNum,
				&tileMapVectorTemp[i][j].terrainFrameX,
				&tileMapVectorTemp[i][j].terrainFrameY,
				&tileMapVectorTemp[i][j].terrain,
				&tileMapVectorTemp[i][j].objImageNum,
				&tileMapVectorTemp[i][j].objFrameX,
				&tileMapVectorTemp[i][j].objFrameY,
				&tileMapVectorTemp[i][j].obj);

			tileMapVectorTemp[i][j].terrainImg = IMAGEMANAGER->findImage(*(_tileSampleImageNameVector.begin() + tileMapVectorTemp[i][j].terrainImageNum));
			tileMapVectorTemp[i][j].objImg = IMAGEMANAGER->findImage(*(_objSampleImageNameVector.begin() + tileMapVectorTemp[i][j].objImageNum));
		}
	}
	_firstFloorMap.setTileMap(tileMapVectorTemp);

	ZeroMemory(str, sizeof(str));
	inFile.getline(str, 1000);
	sscanf_s(str, "interaction_obj_num : %d\r\n", &vectorNum);

	for (int i = 0; i < vectorNum; i++)
	{
		ZeroMemory(str, sizeof(str));
		inFile.getline(str, 1000);

		sscanf_s(str, "%d ,%d, %d, %d, %d, %d\r\n",
			&interactionObject_Temp.xPos,
			&interactionObject_Temp.yPos,
			&interactionObject_Temp.widthPos,
			&interactionObject_Temp.heightPos,
			&interactionObject_Temp.obj,
			&interactionObject_Temp.imageNum);

		interactionObject_Temp._rc = RectMake(interactionObject_Temp.xPos*TILESIZE, interactionObject_Temp.yPos*TILESIZE, interactionObject_Temp.widthPos*TILESIZE, interactionObject_Temp.heightPos*TILESIZE);
		interactionObject_Temp._img = IMAGEMANAGER->findImage(*(_interactionObjectSampleImageNameVector.begin() + interactionObject_Temp.imageNum));

		interactionObjectVectorTemp.push_back(interactionObject_Temp);
	}
	_firstFloorMap.setInteractionObject(interactionObjectVectorTemp);


	tileMapTemp->initTileMap(_currentMapX, _currentMapY, TILESIZE, TILESIZE, NULL);
	tileMapVectorTemp = tileMapTemp->getTileMap();
	interactionObjectVectorTemp.clear();

	//2층 로드
	ZeroMemory(str, sizeof(str));
	inFile.getline(str, 1000);
	sscanf_s(str, "x : %d, y : %d\r\n", &_currentMapX, &_currentMapY);

	for (int i = 0; i < _currentMapY; i++)
	{
		for (int j = 0; j < _currentMapX; j++)
		{
			ZeroMemory(str, sizeof(str));
			inFile.getline(str, 1000);

			sscanf_s(str, "%d ,%d, %d, %d, %d, %d, %d, %d\r\n",
				&tileMapVectorTemp[i][j].terrainImageNum,
				&tileMapVectorTemp[i][j].terrainFrameX,
				&tileMapVectorTemp[i][j].terrainFrameY,
				&tileMapVectorTemp[i][j].terrain,
				&tileMapVectorTemp[i][j].objImageNum,
				&tileMapVectorTemp[i][j].objFrameX,
				&tileMapVectorTemp[i][j].objFrameY,
				&tileMapVectorTemp[i][j].obj);

			tileMapVectorTemp[i][j].terrainImg = IMAGEMANAGER->findImage(*(_tileSampleImageNameVector.begin() + tileMapVectorTemp[i][j].terrainImageNum));
			tileMapVectorTemp[i][j].objImg = IMAGEMANAGER->findImage(*(_objSampleImageNameVector.begin() + tileMapVectorTemp[i][j].objImageNum));
		}
	}
	_secondFloorMap.setTileMap(tileMapVectorTemp);

	ZeroMemory(str, sizeof(str));
	inFile.getline(str, 1000);
	sscanf_s(str, "interaction_obj_num : %d\r\n", &vectorNum);

	for (int i = 0; i < vectorNum; i++)
	{
		ZeroMemory(str, sizeof(str));
		inFile.getline(str, 1000);

		sscanf_s(str, "%d ,%d, %d, %d, %d, %d\r\n",
			&interactionObject_Temp.xPos,
			&interactionObject_Temp.yPos,
			&interactionObject_Temp.widthPos,
			&interactionObject_Temp.heightPos,
			&interactionObject_Temp.obj,
			&interactionObject_Temp.imageNum);

		interactionObject_Temp._rc = RectMake(interactionObject_Temp.xPos*TILESIZE, interactionObject_Temp.yPos*TILESIZE, interactionObject_Temp.widthPos*TILESIZE, interactionObject_Temp.heightPos*TILESIZE);
		interactionObject_Temp._img = IMAGEMANAGER->findImage(*(_interactionObjectSampleImageNameVector.begin() + interactionObject_Temp.imageNum));

		interactionObjectVectorTemp.push_back(interactionObject_Temp);
	}
	_secondFloorMap.setInteractionObject(interactionObjectVectorTemp);

	_nowFloor = 0;
	drawTileMap(0, 1, 0, 0);
	drawTileMap(0, 2, 0, 0);

	delete(tileMapTemp);
}

void scene::addSampleImageName()
{
	//타일맵 이미지 네임 추가
	_tileSampleImageNameVector.push_back("maptool_tile1");
	_tileSampleImageNameVector.push_back("maptool_tile2");
	_tileSampleImageNameVector.push_back("maptool_overworldtile1");
	_tileSampleImageNameVector.push_back("maptool_overworldtile2");

	//오브젝트 이미지 네임 추가
	_objSampleImageNameVector.push_back("maptool_obj");
	_objSampleImageNameVector.push_back("maptool_dungeonobj1");
	_objSampleImageNameVector.push_back("maptool_shop");
	_objSampleImageNameVector.push_back("maptool_house");
	_objSampleImageNameVector.push_back("maptool_overworldblock1");
	_objSampleImageNameVector.push_back("maptool_overworldblock2");
	_objSampleImageNameVector.push_back("maptool_overworldblock3");
	_objSampleImageNameVector.push_back("maptool_houseexterior");
	_objSampleImageNameVector.push_back("maptool_overworldblock4");
	_objSampleImageNameVector.push_back("maptool_dungeonobj2");
	_objSampleImageNameVector.push_back("maptool_dungeonobj3");

	//상호작용 오브젝트 추가
	_interactionObjectSampleImageNameVector.push_back("maptool_grass");
	_interactionObjectSampleImageNameVector.push_back("maptool_smallrock");
	_interactionObjectSampleImageNameVector.push_back("maptool_ladder");
	_interactionObjectSampleImageNameVector.push_back("maptool_treasurebox");
	_interactionObjectSampleImageNameVector.push_back("maptool_bigtreasurebox");
	_interactionObjectSampleImageNameVector.push_back("maptool_jar");
	_interactionObjectSampleImageNameVector.push_back("maptool_opendoor");
	_interactionObjectSampleImageNameVector.push_back("maptool_lockeddoor");
	_interactionObjectSampleImageNameVector.push_back("maptool_ladderreverse");
}

void scene::drawTileMap(bool mapSetAll, int floor, int cameraX, int cameraY)
{
	//이 함수는 맵을 백버퍼가 아닌 다른 곳(백맵)에 그려주는 함수이다
	//이 중 일부분을 때와서 화면에 출력, 미니맵을 그려주게 된다
	//set = 전체맵, 현재맵 유무/ floor = 그려주는 층 구분
	vector<vector<tagTile>> tileMapVectorTemp;
	vector<tagInteractionObject> interactionObjectVectorTemp;

	if (floor == 1)
	{
		tileMapVectorTemp = _firstFloorMap.getTileMap();
		interactionObjectVectorTemp = _firstFloorMap.getInteractionObject();
	}
	else if (floor == 2)
	{
		tileMapVectorTemp = _secondFloorMap.getTileMap();
		interactionObjectVectorTemp = _secondFloorMap.getInteractionObject();
	}
	if (mapSetAll == 0)
	{
		//전체맵을 백이미지에 그려준다
		for (int i = 0; i < _currentMapY; i++)
		{
			for (int j = 0; j < _currentMapX; j++)
			{
				if (tileMapVectorTemp[i][j].terrain == TR_NONE)continue;

				tileMapVectorTemp[i][j].terrainImg->frameRender(_mapImg->getMemDC(), tileMapVectorTemp[i][j].rc.left, tileMapVectorTemp[i][j].rc.top, tileMapVectorTemp[i][j].terrainFrameX, tileMapVectorTemp[i][j].terrainFrameY);

				if (tileMapVectorTemp[i][j].obj == OBJ_NONE)continue;

				tileMapVectorTemp[i][j].objImg->frameRender(_mapImg->getMemDC(), tileMapVectorTemp[i][j].rc.left, tileMapVectorTemp[i][j].rc.top, tileMapVectorTemp[i][j].objFrameX, tileMapVectorTemp[i][j].objFrameY);
			}
		}

		for (int i = 0; i < interactionObjectVectorTemp.size(); i++)
		{
			interactionObjectVectorTemp[i]._img->render(_mapImg->getMemDC(), interactionObjectVectorTemp[i].xPos*TILESIZE, interactionObjectVectorTemp[i].yPos*TILESIZE);
		}
	}
	if (mapSetAll == 1)
	{
		if (floor == 2)
		{		
			//이 부분은 보이는 화면만 백맵에 다시 그려주는 함수이다.
			//맵을 맵 백버퍼에 그려준다
			for (int i = cameraY / TILESIZE; i < _currentMapY; i++)
			{
				//화면에 나오는 타일만 그려주기 위함
				if ((cameraY + WINSIZEY) / TILESIZE + 1 < i)
					break;

				for (int j = cameraX / TILESIZE; j < _currentMapX; j++)
				{
					//화면에 나오는 타일만 그려주기 위함
					if ((cameraX + WINSIZEX) / TILESIZE + 1 < j)
						break;

					if (tileMapVectorTemp[i][j].terrain == TR_NONE)continue;

					tileMapVectorTemp[i][j].terrainImg->frameRender(_backmap->getMemDC(), tileMapVectorTemp[i][j].rc.left, tileMapVectorTemp[i][j].rc.top, tileMapVectorTemp[i][j].terrainFrameX, tileMapVectorTemp[i][j].terrainFrameY);

					if (tileMapVectorTemp[i][j].obj == OBJ_NONE)continue;

					tileMapVectorTemp[i][j].objImg->frameRender(_backmap->getMemDC(), tileMapVectorTemp[i][j].rc.left, tileMapVectorTemp[i][j].rc.top, tileMapVectorTemp[i][j].objFrameX, tileMapVectorTemp[i][j].objFrameY);
				}
			}
			for (int i = 0; i < interactionObjectVectorTemp.size(); i++)
			{
				if (interactionObjectVectorTemp[i].xPos*TILESIZE + interactionObjectVectorTemp[i].widthPos*TILESIZE< cameraX || interactionObjectVectorTemp[i].xPos*TILESIZE>cameraX + WINSIZEX)
				{
					continue;
				}
				else if (interactionObjectVectorTemp[i].yPos*TILESIZE + interactionObjectVectorTemp[i].heightPos*TILESIZE< cameraY || interactionObjectVectorTemp[i].yPos*TILESIZE>cameraY + WINSIZEY)
				{
					continue;
				}

				interactionObjectVectorTemp[i]._img->render(_backmap->getMemDC(), interactionObjectVectorTemp[i].xPos*TILESIZE, interactionObjectVectorTemp[i].yPos*TILESIZE);
			}
			
		}
		else
		{
			BitBlt(_backmap->getMemDC(), cameraX, cameraY, cameraX + WINSIZEX, cameraY + WINSIZEY, _mapImg->getMemDC(), cameraX, cameraY, SRCCOPY);
			//_backmap->render(_mapImg->getMemDC(), cameraX, cameraY,cameraX,cameraY, cameraX+WINSIZEX, cameraY+ WINSIZEY);
		}
		
	}
}

void scene::addMoveRc(int x, int y, int width, int height)
{
	_moveRcVector.push_back(RectMakeCenter(x, y, width, height));
}

int scene::collisionRc(RECT rc)
{
	//씬 이동
	RECT temp;

	for (int i = 0; i < _moveRcVector.size(); i++)
	{
		if (IntersectRect(&temp, &_moveRcVector[i], &rc))
		{
			return i;
		}
	}

	return -1;
}

void scene::clearMoveRc()
{
	_moveRcVector.clear();
}

void scene::reDrawPosition(bool isPlayer, int floor, int positionX, int positionY, int width, int height, int cameraX, int cameraY)
{
	if (_sceneName == SCENE_NAME_INGAME_SCENE)
	{
		vector<vector<tagTile>> tileMapVectorTemp;
		vector<tagInteractionObject> interactionObjectVectorTemp;

		if (floor == 1)
		{
			tileMapVectorTemp = _firstFloorMap.getTileMap();
			interactionObjectVectorTemp = _firstFloorMap.getInteractionObject();
		}
		else if (floor == 2)
		{
			tileMapVectorTemp = _secondFloorMap.getTileMap();
			interactionObjectVectorTemp = _secondFloorMap.getInteractionObject();
		}
		
		for (int i = positionY / TILESIZE; i < (positionY + height) / TILESIZE; i++) 
		{
			if (_currentMapY <= i)
				break;

			for (int j = positionX / TILESIZE; j < (positionX + width) / TILESIZE; j++)
			{
				if (_currentMapX <= j)
					break;

				if (tileMapVectorTemp[i][j].terrain == TR_NONE)continue;

				if (isPlayer)
				{
					tileMapVectorTemp[i][j].terrainImg->frameRender(_backmap->getMemDC(), tileMapVectorTemp[i][j].rc.left, tileMapVectorTemp[i][j].rc.top, tileMapVectorTemp[i][j].terrainFrameX, tileMapVectorTemp[i][j].terrainFrameY);
				}
				else
				{
					tileMapVectorTemp[i][j].terrainImg->frameRender(_mapImg->getMemDC(), tileMapVectorTemp[i][j].rc.left, tileMapVectorTemp[i][j].rc.top, tileMapVectorTemp[i][j].terrainFrameX, tileMapVectorTemp[i][j].terrainFrameY);
				}

				if (tileMapVectorTemp[i][j].obj == OBJ_NONE)continue;

				if (isPlayer)
				{
					tileMapVectorTemp[i][j].objImg->frameRender(_backmap->getMemDC(), tileMapVectorTemp[i][j].rc.left, tileMapVectorTemp[i][j].rc.top, tileMapVectorTemp[i][j].objFrameX, tileMapVectorTemp[i][j].objFrameY);
				}
				else
				{
					tileMapVectorTemp[i][j].objImg->frameRender(_mapImg->getMemDC(), tileMapVectorTemp[i][j].rc.left, tileMapVectorTemp[i][j].rc.top, tileMapVectorTemp[i][j].objFrameX, tileMapVectorTemp[i][j].objFrameY);
				}
			}
		}
		if (!isPlayer)
		{
			for (int i = 0; i < interactionObjectVectorTemp.size(); i++)
			{
				if (interactionObjectVectorTemp[i].xPos*TILESIZE + interactionObjectVectorTemp[i].widthPos*TILESIZE< cameraX || interactionObjectVectorTemp[i].xPos*TILESIZE>cameraX + WINSIZEX)
				{
					continue;
				}
				else if (interactionObjectVectorTemp[i].yPos*TILESIZE + interactionObjectVectorTemp[i].heightPos*TILESIZE< cameraY || interactionObjectVectorTemp[i].yPos*TILESIZE>cameraY + WINSIZEY)
				{
					continue;
				}

				interactionObjectVectorTemp[i]._img->render(_mapImg->getMemDC(), interactionObjectVectorTemp[i].xPos*TILESIZE, interactionObjectVectorTemp[i].yPos*TILESIZE);
			}
		}
		
		
	}
}


