#include "stdafx.h"
#include "dungeon2F.h"


dungeon2F::dungeon2F()
{
}


dungeon2F::~dungeon2F()
{
	release();
}

HRESULT dungeon2F::init()
{
	_sceneName = SCENE_NAME_INGAME_SCENE;

	scene::init();

	sceneLoading("maps/dungeon2f.map");

	_nowFloor = 1;

	_currentMapX = 32;
	_currentMapY = 96;

	addMoveRc(700, 1950, 20, 80);				//DUNGEON1F_MOVE_DUNGEON2F(0) 다음 던전으로 가는 렉트입니다
	addMoveRc(380, 140, 80, 20);				//DUNGEON1F_MOVE_GAMESCENE4(1)으로 가는 렉트입니다

	addMoveRc(380, 1700, 80, 20);				//DUNGEON1F_MOVE_DUNGEON1F_1_2(2) 맵으로 가는 렉트입니다
	addMoveRc(380, 1460, 80, 20);				//DUNGEON1F_MOVE_DUNGEON1F_2_1(3) 맵으로 가는 렉트입니다
	addMoveRc(380, 910, 80, 20);				//DUNGEON1F_MOVE_DUNGEON1F_2_3(4) 맵으로 가는 렉트입니다
	addMoveRc(380, 650, 80, 20);				//DUNGEON1F_MOVE_DUNGEON1F_3_2(5) 맵으로 가는 렉트입니다

	ENEMYMANAGER->init(7);

	return S_OK;
}
void dungeon2F::update()
{
	vector<enemySet*> enemyList = ENEMYMANAGER->getEnemyList();
	vector<tagInteractionObject> interObjTemp = _firstFloorMap.getInteractionObject();

	scene::update();

	switch (collisionRc(_player.getRect()))
	{
	case DUNGEON2F_MOVE_DUNGEON1F:
		SCENEMANAGER->changeScene("dungeon1f");
		_player.setPlayerSceneMove(770, 200, 64, 158);
		break;
	case DUNGEON2F_MOVE_GAMESCENE4:
		SCENEMANAGER->changeScene("gamescene4");
		_player.setPlayerSceneMove(1150, 830, 64, 64);
		break;
	case DUNGEON2F_MOVE_DUNGEON1F_1_2:
		_player.setPlayerSceneMove(360, 1380, 32, 96);
		break;
	case DUNGEON2F_MOVE_DUNGEON1F_2_1:
		_player.setPlayerSceneMove(360, 1720, 32, 96);
		break;
	case DUNGEON2F_MOVE_DUNGEON1F_2_3:
		_player.setPlayerSceneMove(360, 560, 32, 96);
		_sound.soundOff();
		_sound.soundPlay("보스");
		break;
	case DUNGEON2F_MOVE_DUNGEON1F_3_2:
		_player.setPlayerSceneMove(360, 960, 32, 96);
		_sound.soundOff();
		_sound.soundPlay("던전");
		break;
	case DUNGEON2F_MOVE_NULL:
	default:
		break;
	}

	enemyList = ENEMYMANAGER->getEnemyList();
	for (int i = 0; i < enemyList.size(); i++)
	{
		if (strcmp(enemyList[i]->getName(), "worm") == 0)
		{
			break;
		}

		if (i == enemyList.size() - 1)
		{
			for (int i = 0; i < interObjTemp.size(); i++)
			{
				if (interObjTemp[i].obj == INTERACTIONOBJ_ROCKEDDOOR)
				{
					_firstFloorMap.addInteractionObject(interObjTemp[i].xPos, interObjTemp[i].yPos, interObjTemp[i].widthPos, interObjTemp[i].heightPos,
						interObjTemp[i]._rc, INTERACTIONOBJ_OPENDOOR, IMAGEMANAGER->findImage("maptool_opendoor"), interObjTemp[i].imageNum + 1);

					//오브젝트 삭제
					RECT rectTemp = interObjTemp[i]._rc;

					_firstFloorMap.deleteInteractionObject(i);
					SCENEMANAGER->reDrawPosition(false, SCENEMANAGER->getNowFloor(), rectTemp.left, rectTemp.top, rectTemp.right - rectTemp.left, rectTemp.bottom - rectTemp.top, _player.getCameraX(), _player.getCameraY());
				}
			}
		}
	}
}

void dungeon2F::release()
{
	scene::release();
}

void dungeon2F::render()
{
	scene::render();
}
