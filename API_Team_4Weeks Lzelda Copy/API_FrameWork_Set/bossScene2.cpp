#include "stdafx.h"
#include "bossScene2.h"


bossScene2::bossScene2()
{
}


bossScene2::~bossScene2()
{
	release();
}

HRESULT bossScene2::init()
{
	_sceneName = SCENE_NAME_INGAME_SCENE;

	scene::init();

	sceneLoading("maps/boss2.map");

	_nowFloor = 1;

	_currentMapX = 32;
	_currentMapY = 30;

	addMoveRc(390, 680, 80, 20);				//GAMESCENE1_MOVE_GAMESCENE1(0) 다음맵으로 가는 렉트입니다

	ENEMYMANAGER->init(2);

	_sound.soundOff();
	_sound.soundPlay("보스");

	return S_OK;
}

void bossScene2::update()
{
	scene::update();

	switch (collisionRc(_player.getRect()))
	{
	case BOSSSCENE2_MOVE_GAMESCENE1:
		SCENEMANAGER->changeScene("gamescene1");
		_player.setPlayerSceneMove(1470, 1200, 64, 64);
		break;
	case BOSSSCENE2_MOVE_NULL:
	default:
		break;
	}
}

void bossScene2::release()
{
	scene::release();
}

void bossScene2::render()
{
	scene::render();
}
