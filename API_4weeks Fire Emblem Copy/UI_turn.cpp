#include "stdafx.h"
#include "UI_turn.h"


UI_turn::UI_turn()
{
}


UI_turn::~UI_turn()
{
}

HRESULT UI_turn::init()
{
	playerTurnFront = IMAGEMANAGER->findImage("아군턴");
	enemyTurnFront = IMAGEMANAGER->findImage("적군턴");

	turnImagePos = PointFloatMake(0, 0);
	isTurnChange = false;
	isTurnAnimation = false;
	checkAniTime = true;
	startTime = 0;
	return S_OK;
}

void UI_turn::update()
{
	if (isTurnChange)
	{
		turnImagePos = PointFloatMake(-600, 225);
		rc_TurnFront = RectMake(turnImagePos.x, turnImagePos.y, 600, 225);
		isTurnChange = false;
		isTurnAnimation = true;
		checkAniTime = true;
	}

	if (isTurnAnimation)
	{
		if (GetTickCount() - startTime >= 2000)
		{
			animation();
		}				
	}
}

void UI_turn::animation()
{
	turnImagePos.x += 50;

	if (checkAniTime)
	{
		if (turnImagePos.x >= 90 && turnImagePos.x <= 120)
		{
			startTime = GetTickCount();
			checkAniTime = false;
		}
	}

	if (turnImagePos.x >= WINSIZEX)
	{
		isTurnAnimation = false;
	}
}

void UI_turn::render()
{
	if (isTurnAnimation)
	{
		if (isEnemyTurn)
		{
			playerTurnFront->render(getMemDC(), turnImagePos.x, turnImagePos.y);
		}
		else if (isPlayerTurn)
		{
			enemyTurnFront->render(getMemDC(), turnImagePos.x, turnImagePos.y);
		}
	}
}

void UI_turn::release()
{
}
