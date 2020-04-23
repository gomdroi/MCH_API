#include "stdafx.h"
#include "gameScene1.h"


gameScene1::gameScene1()
{
}


gameScene1::~gameScene1()
{
	release();
}

HRESULT gameScene1::init()
{
	_sceneName = SCENE_NAME_INGAME_SCENE;

	scene::init();

	sceneLoading("maps/scene1.map");

	_nowFloor = 1;

	_currentMapX = 64;
	_currentMapY = 64;

	addMoveRc(620, 780, 80, 20);				//GAMESCENE1_MOVE_HOME(0) 집으로 가는 렉트입니다
	addMoveRc(700, 1535, 80, 20);				//GAMESCENE1_MOVE_GAMESCENE2(1) 다음맵으로 가는 렉트입니다
	addMoveRc(1530, 1200, 20, 80);				//GAMESCENE1_MOVE_BOSS2(2) 다음맵으로 가는 렉트입니다

	ENEMYMANAGER->init(1);

	_sound.soundOff();
	_sound.soundPlay("오버월드");
	
	return S_OK;
}

void gameScene1::update()
{
	scene::update();

	switch (collisionRc(_player.getRect()))
	{
	case GAMESCENE1_MOVE_HOME:
		SCENEMANAGER->changeScene("homescene");
		_player.setPlayerSceneMove(360, 620, 32, 32);
		break;
	case GAMESCENE1_MOVE_GAMESCENE2:
		SCENEMANAGER->changeScene("gamescene2");
		_player.setPlayerSceneMove(_player.getPositionX(), 50, 64, 64);
		break;
	case GAMESCENE1_MOVE_BOSS2:
		SCENEMANAGER->changeScene("bossscene2");
		_player.setPlayerSceneMove(390, 550, 32, 30);
		break;
	case GAMESCENE1_MOVE_NULL:
	default:
		break;
	}
}

void gameScene1::release()
{
	scene::release();
}

void gameScene1::render()
{
	scene::render();
}
