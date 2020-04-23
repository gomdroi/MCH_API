#include "stdafx.h"
#include "dungeon1F.h"


dungeon1F::dungeon1F()
{
}


dungeon1F::~dungeon1F()
{
	release();
}

HRESULT dungeon1F::init()
{
	_sceneName = SCENE_NAME_INGAME_SCENE;

	scene::init();

	sceneLoading("maps/dungeon1f.map");

	_nowFloor = 1;

	_currentMapX = 64;
	_currentMapY = 158;

	addMoveRc(770, 120, 80, 20);				//DUNGEON1F_MOVE_DUNGEON2F(0) 다음 던전으로 가는 렉트입니다
	addMoveRc(770, 3700, 80, 20);				//DUNGEON1F_MOVE_GAMESCENE4(1)으로 가는 렉트입니다

	addMoveRc(770, 3150, 80, 20);				//DUNGEON1F_MOVE_DUNGEON1F_1_2(2) 맵으로 가는 렉트입니다
	addMoveRc(770, 2950, 80, 20);				//DUNGEON1F_MOVE_DUNGEON1F_2_1(3) 맵으로 가는 렉트입니다
	addMoveRc(800, 1560, 80, 20);				//DUNGEON1F_MOVE_DUNGEON1F_2_3(4) 맵으로 가는 렉트입니다
	addMoveRc(770, 1400, 80, 20);				//DUNGEON1F_MOVE_DUNGEON1F_3_2(5) 맵으로 가는 렉트입니다

	_sound.soundOff();
	_sound.soundPlay("던전");

	ENEMYMANAGER->init(6);

	return S_OK;
}

void dungeon1F::update()
{
	scene::update();

	// 렉트충돌을 검출합니다
	switch (collisionRc(_player.getRect()))
	{
	case DUNGEON1F_MOVE_DUNGEON2F:
		SCENEMANAGER->changeScene("dungeon2f");
		_player.setPlayerSceneMove(600, 1930, 32, 96);
		break;
	case DUNGEON1F_MOVE_GAMESCENE4:
		SCENEMANAGER->changeScene("gamescene4");
		_player.setPlayerSceneMove(1150, 830, 64, 64);
		break;
	case DUNGEON1F_MOVE_DUNGEON1F_1_2:
		_player.setPlayerSceneMove(770, 2850, 64, 158);//2850/1550
		_nowFloor = 2;
		break;
	case DUNGEON1F_MOVE_DUNGEON1F_2_1:
		_player.setPlayerSceneMove(770, 3200, 64, 158);
		_nowFloor = 1;
		break;
	case DUNGEON1F_MOVE_DUNGEON1F_2_3:
		_player.setPlayerSceneMove(770, 1300, 64, 158);
		_nowFloor = 1;
		break;
	case DUNGEON1F_MOVE_DUNGEON1F_3_2:
		_player.setPlayerSceneMove(800, 1600, 64, 158);
		_nowFloor = 2;
		break;
	case DUNGEON1F_MOVE_NULL:
	default:
		break;
	}

}

void dungeon1F::release()
{
	scene::release();
}

void dungeon1F::render()
{
	scene::render();
}
