#include "stdafx.h"
#include "homeScene.h"


homeScene::homeScene()
{
}


homeScene::~homeScene()
{
	release();
}

HRESULT homeScene::init()
{
	_sceneName = SCENE_NAME_INGAME_SCENE;

	scene::init();

	sceneLoading("maps/home.map");

	_nowFloor = 1;

	_currentMapX = 32;
	_currentMapY = 32;

	addMoveRc(380, 690, 80, 20);

	_sound.soundOff();
	_sound.soundPlay("Áý");

	return S_OK;
}

void homeScene::update()
{
	scene::update();

	switch (collisionRc(_player.getRect()))
	{
	case HOMESCENE_MOVE_GAMESCENE1:
		SCENEMANAGER->changeScene("gamescene1");
		_player.setPlayerSceneMove(600, 830, 64, 64);

		break;
	case HOMESCENE_MOVE_NULL:
	default:
		break;
	}
	
}

void homeScene::release()
{
	scene::release();
}

void homeScene::render()
{
	scene::render();
}
