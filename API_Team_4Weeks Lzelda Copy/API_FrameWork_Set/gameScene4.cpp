#include "stdafx.h"
#include "gameScene4.h"


gameScene4::gameScene4()
{
}


gameScene4::~gameScene4()
{
	release();
}

HRESULT gameScene4::init()
{
	_sceneName = SCENE_NAME_INGAME_SCENE;

	scene::init();

	sceneLoading("maps/scene4.map");

	_nowFloor = 1;

	_currentMapX = 64;
	_currentMapY = 64;

	addMoveRc(1150, 780, 80, 20);								//GAMESCENE4_MOVE_DUNGEON(0) 씬2로 가는 렉트입니다
	addMoveRc(1530, 730, 20, 1250);								//GAMESCENE4_MOVE_GAMESCENE3(1) 씬4로 가는 렉트입니다

	_sound.soundOff();
	_sound.soundPlay("오버월드");

	ENEMYMANAGER->init(5);

	return S_OK;
}

void gameScene4::update()
{
	scene::update();

	switch (collisionRc(_player.getRect()))
	{
	case GAMESCENE4_MOVE_DUNGEON:
		SCENEMANAGER->changeScene("dungeon1f");
		_player.setPlayerSceneMove(745, 3630, 64, 158);
		break;
	case GAMESCENE4_MOVE_GAMESCENE3:
		SCENEMANAGER->changeScene("gamescene3");
		_player.setPlayerSceneMove(50, _player.getPositionY(), 64, 64);
		break;
	case GAMESCENE4_MOVE_NULL:
	default:
		break;
	}
}

void gameScene4::release()
{
	scene::release();
}

void gameScene4::render()
{
	scene::render();
}
