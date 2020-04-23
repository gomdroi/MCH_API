#include "stdafx.h"
#include "shopScene.h"


shopScene::shopScene()
{
}


shopScene::~shopScene()
{
	release();
}

HRESULT shopScene::init()
{
	_sceneName = SCENE_NAME_INGAME_SHOP_SCENE;

	scene::init();

	sceneLoading("maps/shop.map");

	_nowFloor = 1;

	//상점 문
	addMoveRc(380, 690, 80, 20);

	_sound.soundOff();
	_sound.soundPlay("집");
	_isMove = false;

	count = 0;
	index = 0;

	shopEnter = IMAGEMANAGER->findImage("상점입장말풍선");
	shopEnter->setFrameX(index);

	return S_OK;
}

void shopScene::update()
{
	if (_isMove)
	{
		scene::update();

		switch (collisionRc(_player.getRect()))
		{
		case SHOPSCENE_MOVE_GAMESCENE2:
			SCENEMANAGER->changeScene("gamescene2");
			_player.setPlayerSceneMove(1200, 790, 64, 64);
			break;
		case SHOPSCENE_MOVE_NULL:
		default:
			break;
		}
	}
	else
	{
		count++;
		shopEnter->setFrameY(0);
		if (count % 10 == 0)
		{
			count = 0;
			index++;
			if (index > shopEnter->getMaxFrameX())
			{
				index = 3;
			}
			shopEnter->setFrameX(index);
		}

		if (KEYMANAGER->isOnceKeyDown('S'))
		{
			_isMove = true;
		}
	}
}

void shopScene::release()
{
	scene::release();
}

void shopScene::render()
{
	scene::render();

	if (!_isMove)
	{
		IMAGEMANAGER->frameRender("상점입장말풍선", getMemDC(), 90, 200, shopEnter->getFrameX(), shopEnter->getFrameY());
	}
}

