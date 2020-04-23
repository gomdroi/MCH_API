#include "stdafx.h"
#include "gameScene2.h"


gameScene2::gameScene2()
{
}


gameScene2::~gameScene2()
{
	release();
}

HRESULT gameScene2::init()
{
	_sceneName = SCENE_NAME_INGAME_SCENE;

	scene::init();

	sceneLoading("maps/scene2.map");

	_nowFloor = 1;

	_currentMapX = 64;
	_currentMapY = 64;

	addMoveRc(1200, 730, 80, 20);								//GAMESCENE2_MOVE_SHOP(0) 샵으로 가는 렉트입니다
	addMoveRc(700, 10, 100, 20);								//GAMESCENE2_MOVE_GAMESCENE1(1) 씬1로 가는 렉트입니다
	addMoveRc(750, 1530, 1220, 20);								//GAMESCENE2_MOVE_GAMESCENE3(2) 씬3로 가는 렉트입니다

	_sound.soundOff();
	_sound.soundPlay("오버월드");

	ENEMYMANAGER->init(3);

	return S_OK;
}

void gameScene2::update()
{
	scene::update();

	switch (collisionRc(_player.getRect()))
	{
	case GAMESCENE2_MOVE_SHOP:
		SCENEMANAGER->changeScene("shopscene");
		_player.setPlayerSceneMove(360, 620, 32, 32);
		break;
	case GAMESCENE2_MOVE_GAMESCENE1:
		SCENEMANAGER->changeScene("gamescene1");
		_player.setPlayerSceneMove(_player.getPositionX(), 1450, 64, 64);
		break;
	case GAMESCENE2_MOVE_GAMESCENE3:
		SCENEMANAGER->changeScene("gamescene3");
		_player.setPlayerSceneMove(_player.getPositionX(), 50, 64, 64);
		break;
	case GAMESCENE2_MOVE_NULL:
	default:
		break;
	}
}

void gameScene2::release()
{
	scene::release();
}

void gameScene2::render()
{
	scene::render();
}
