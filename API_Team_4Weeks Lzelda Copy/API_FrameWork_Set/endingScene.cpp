#include "stdafx.h"
#include "endingScene.h"


endingScene::endingScene()
{
}

endingScene::~endingScene()
{
}

HRESULT endingScene::init()
{
	_ending = IMAGEMANAGER->findImage("엔딩말풍선");
	
	_sceneName = SCENE_NAME_ENDING_SCENE;

	startCount = 0;
	count = 0;
	index = 0;

	start = false;
	_ending->setFrameX(index);

	_sound.soundOff();
	_sound.soundPlay("엔딩");

	ANIMATIONMANAGER->addDefAnimation("player_rotate", "player_rotate", 13, true, true);
	ANIMATIONMANAGER->addDefAnimation("player_dead", "player_dead", 8, false, true);
	_ani = ANIMATIONMANAGER->findAnimation("player_rotate");
	_img = IMAGEMANAGER->findImage("player_rotate");

	_ani->start();
	return S_OK;
}

void endingScene::update()
{
	if (startCount < 45)
	{
		startCount++;
	}
	if (startCount > 40)
	{
		start = true;
	}
	if (INVENTORYMANAGER->getGreenPendent() && INVENTORYMANAGER->getBluePendent() && INVENTORYMANAGER->getRedPendent())
	{
		if (start)
		{
			count++;
			_ending->setFrameY(0);
			if (count % 90 == 0)
			{
				count = 0;
				index++;
				if (index > _ending->getMaxFrameX())
				{
					index = 2;
				}
				_ending->setFrameX(index);
			}
			if (index == 2 && KEYMANAGER->isStayKeyDown(VK_SPACE))
			{
				SCENEMANAGER->changeScene("startScene");
				INVENTORYMANAGER->release();
			}

		}
	}
	else if (_player.getCurHP() <= 0)
	{
		if (start)
		{
			if (_ending->getFrameX() == 0)
			{
				_ani->stop();
				_ani->start();
			}
			_ending->setFrameX(2);

			_ani = ANIMATIONMANAGER->findAnimation("player_dead");
			_img = IMAGEMANAGER->findImage("player_dead");

			if (KEYMANAGER->isStayKeyDown(VK_SPACE))
			{
				SCENEMANAGER->changeScene("startScene");
			}
		}
	}
}

void endingScene::release()
{
}

void endingScene::render()
{
	IMAGEMANAGER->render("검은상자1", getMemDC(), 0, 0);

	if (start)
	{	
		IMAGEMANAGER->frameRender("엔딩말풍선", getMemDC(), 50, 200, _ending->getFrameX(), _ending->getFrameY());
	}

	if (_player.getCurHP() <= 0)
	{
		_img->aniRender(getMemDC(),WINSIZEX/2-100,WINSIZEY-200, _ani);
	}
}
