#include "stdafx.h"
#include "enemyManager.h"
#include "enemySet.h"
#include "soldier.h"
#include "eyeman.h"
#include "malmijal.h"
#include "snake.h"
#include "bat.h"
#include "worm_boss.h"
#include "agahnim_boss.h"

enemyManager::enemyManager()
{
}


enemyManager::~enemyManager()
{
}

HRESULT enemyManager::init()
{
	return S_OK;
}

HRESULT enemyManager::init(int sceneNum)
{

	malmijal* cMalmijal;
	eyeman* cEyeman;

	bat* cBat;
	worm_boss* cWboss = new worm_boss;
	cWboss->init();
	soldier* cSoldier;
	snake* cSnake;
	agahnim_boss* cAgahnim = new agahnim_boss;
	cAgahnim->init();

	if (sceneNum == 1)
	{
		cSoldier = new soldier;
		cSoldier->init();
		cSoldier->setEnemy(GREEN, PointMake(1100, 1000));
		_enemyVector.push_back(cSoldier);
	}
	else if (sceneNum == 2)
	{
		cAgahnim->setEnemy(PointMake(500, 500));
		_enemyVector.push_back(cAgahnim);
	}
	else if (sceneNum == 3)
	{
		cSoldier = new soldier;
		cSoldier->init();
		cSoldier->setEnemy(BLUE, PointMake(500, 400));
		_enemyVector.push_back(cSoldier);

		cSoldier = new soldier;
		cSoldier->init();
		cSoldier->setEnemy(BLUE, PointMake(800, 500));
		_enemyVector.push_back(cSoldier);

		cSnake = new snake;
		cSnake->init();
		cSnake->setEnemy(PointMake(600, 950));
		_enemyVector.push_back(cSnake);
	}
	else if (sceneNum == 4)
	{
		cSnake = new snake;
		cSnake->init();
		cSnake->setEnemy(PointMake(200, 200));
		_enemyVector.push_back(cSnake);

		cSnake = new snake;
		cSnake->init();
		cSnake->setEnemy(PointMake(300, 300));
		_enemyVector.push_back(cSnake);
	}
	else if (sceneNum == 5)
	{
		cEyeman = new eyeman;
		cEyeman->init();
		cEyeman->setEnemy(GREEN, PointMake(1150, 1000));
		_enemyVector.push_back(cEyeman);

		cSnake = new snake;
		cSnake->init();
		cSnake->setEnemy(PointMake(1200, 300));
		_enemyVector.push_back(cSnake);

		cSoldier = new soldier;
		cSoldier->init();
		cSoldier->setEnemy(GREEN, PointMake(400, 1000));
		_enemyVector.push_back(cSoldier);
	}
	else if (sceneNum == 6)
	{

		//상자 768,696
		//정가운데 768,3504
		cMalmijal = new malmijal;
		cMalmijal->init();
		cMalmijal->setEnemy(PointMake(750, 3400));
		_enemyVector.push_back(cMalmijal);

		cMalmijal = new malmijal;
		cMalmijal->init();
		cMalmijal->setEnemy(PointMake(750, 3600));
		_enemyVector.push_back(cMalmijal);

		cMalmijal = new malmijal;
		cMalmijal->init();
		cMalmijal->setEnemy(PointMake(800, 3500));
		_enemyVector.push_back(cMalmijal);

		cMalmijal = new malmijal;
		cMalmijal->init();
		cMalmijal->setEnemy(PointMake(700, 3500));
		_enemyVector.push_back(cMalmijal);

		cEyeman = new eyeman;
		cEyeman->init();
		cEyeman->setEnemy(GREEN, PointMake(968, 696));
		_enemyVector.push_back(cEyeman);

		cEyeman = new eyeman;
		cEyeman->init();
		cEyeman->setEnemy(RED, PointMake(568, 696));
		_enemyVector.push_back(cEyeman);

		cBat = new bat;
		cBat->init();
		cBat->setEnemy(PointMake(500, 300));
		_enemyVector.push_back(cBat);

		cBat = new bat;
		cBat->init();
		cBat->setEnemy(PointMake(1000, 350));
		_enemyVector.push_back(cBat);

		cBat = new bat;
		cBat->init();
		cBat->setEnemy(PointMake(800, 800));
		_enemyVector.push_back(cBat);

		cBat = new bat;
		cBat->init();
		cBat->setEnemy(PointMake(768, 696));
		_enemyVector.push_back(cBat);
	}
	else if (sceneNum == 7)
	{
		//384,384
		//384,1032
		//384,1944
		cWboss->setEnemy(PointMake(384, 384));
		_enemyVector.push_back(cWboss);

		cEyeman = new eyeman;
		cEyeman->init();
		cEyeman->setEnemy(RED, PointMake(384, 1232));
		_enemyVector.push_back(cEyeman);

		cEyeman = new eyeman;
		cEyeman->init();
		cEyeman->setEnemy(RED, PointMake(384, 932));
		_enemyVector.push_back(cEyeman);

		cSnake = new snake;
		cSnake->init();
		cSnake->setEnemy(PointMake(400, 2044));
		_enemyVector.push_back(cSnake);

		cSnake = new snake;
		cSnake->init();
		cSnake->setEnemy(PointMake(450, 1800));
		_enemyVector.push_back(cSnake);

		cSnake = new snake;
		cSnake->init();
		cSnake->setEnemy(PointMake(250, 2100));
		_enemyVector.push_back(cSnake);

	}

	/*
	_enemyVector.push_back(cMalmijal);
	_enemyVector.push_back(cEyeman);
	_enemyVector.push_back(cSnake);
	_enemyVector.push_back(cBat);
	_enemyVector.push_back(cWboss);
	_enemyVector.push_back(cSoldier);
	_enemyVector.push_back(cAgahnim);*/

	return S_OK;
}

void enemyManager::release()
{
	vector<enemySet*>::iterator eIter = _enemyVector.begin();
	for (eIter; eIter != _enemyVector.end();)
	{
		SAFE_DELETE((*eIter));
		eIter = _enemyVector.erase(eIter);
	}
}

void enemyManager::update()
{
	for (int i = 0; i < _enemyVector.size(); i++)
	{
		RECT rectTemp = _enemyVector[i]->getRect();
		if (rectTemp.right<_player.getCameraX() - 400 || rectTemp.left > _player.getCameraX() + WINSIZEX + 400 ||
			rectTemp.top < _player.getCameraY() - 400 || rectTemp.bottom > _player.getCameraY() + WINSIZEY + 400) continue;

		_enemyVector[i]->update();
		if (_enemyVector[i]->getHp() <= 0)
		{
			if (strcmp(_enemyVector[i]->getName(), "agahnim") == 0)
			{
				ITEMMANAGER->dropPendent(_enemyVector[i]->getPositionX(), _enemyVector[i]->getPositionY(), REDPENDENT);
			}
			if (strcmp(_enemyVector[i]->getName(), "worm") == 0)
			{
				ITEMMANAGER->dropPendent(_enemyVector[i]->getPositionX(), _enemyVector[i]->getPositionY(), BLUEPENDENT);
			}

			EFFECTMANAGER->play("enemy_death", _enemyVector[i]->getPositionX(), _enemyVector[i]->getPositionY());

			SAFE_DELETE(_enemyVector[i]);
			_enemyVector.erase(_enemyVector.begin() + i);
		}
	}
}

void enemyManager::render(HDC hdc)
{
	for (int i = 0; i < _enemyVector.size(); i++)
	{
		RECT rectTemp = _enemyVector[i]->getRect();
		if (rectTemp.right<_player.getCameraX() - 200 || rectTemp.left > _player.getCameraX() + WINSIZEX + 200 ||
			rectTemp.top < _player.getCameraY() - 200 || rectTemp.bottom > _player.getCameraY() + WINSIZEY + 200) continue;
		_enemyVector[i]->render(hdc);
	}
}
