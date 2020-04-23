#include "stdafx.h"
#include "gameScene3.h"


gameScene3::gameScene3()
{
}


gameScene3::~gameScene3()
{
	release();
}

HRESULT gameScene3::init()
{
	_sceneName = SCENE_NAME_INGAME_SCENE;

	scene::init();

	sceneLoading("maps/scene3.map");

	_nowFloor = 1;

	_currentMapX = 64;
	_currentMapY = 64;

	addMoveRc(750, 10, 1220, 20);								//GAMESCENE3_MOVE_GAMESCENE2(0) 씬2로 가는 렉트입니다
	addMoveRc(10, 730, 20, 1250);								//GAMESCENE3_MOVE_GAMESCENE4(1) 씬4로 가는 렉트입니다

	_sound.soundOff();
	_sound.soundPlay("오버월드");

	ENEMYMANAGER->init(4);

	return S_OK;
}

void gameScene3::update()
{
	scene::update();

	switch (collisionRc(_player.getRect()))
	{
	case GAMESCENE3_MOVE_GAMESCENE2:
		SCENEMANAGER->changeScene("gamescene2");
		_player.setPlayerSceneMove(_player.getPositionX(), 1450, 64, 64);
		break;
	case GAMESCENE3_MOVE_GAMESCENE4:
		SCENEMANAGER->changeScene("gamescene4");
		_player.setPlayerSceneMove(1450,_player.getPositionY(), 64, 64);
		break;
	case GAMESCENE3_MOVE_NULL:
	default:
		break;
	}
}

void gameScene3::release()
{
	scene::release();
}

void gameScene3::render()
{
	scene::render();
}
