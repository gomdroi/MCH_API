#include "stdafx.h"
#include "mainmenu.h"


mainmenu::mainmenu()
{
}


mainmenu::~mainmenu()
{
}

HRESULT mainmenu::init()
{
	
	dragonCursor = IMAGEMANAGER->addFrameImage("메인메뉴커서", "images/dragoncursor.bmp", 720, 360, 1, 4, true, RGB(255, 0, 255));
	cursorPos = PointMake(WINSIZEX / 2, 180);
	g_cursor.setStageNum(1);
	return S_OK;
}

void mainmenu::update()
{
	animation();
	control();
}

void mainmenu::render()
{
	IMAGEMANAGER->render("메인메뉴", getMemDC());
	dragonCursor->frameRender(getMemDC(), 
		cursorPos.x - dragonCursor->getFrameWidth() / 2, cursorPos.y - dragonCursor->getFrameHeight() / 2
		, dragonCursor->getFrameX(), dragonCursor->getFrameY());
}

void mainmenu::release()
{
}

void mainmenu::animation()
{
	m_cursorCount++;
	dragonCursor->setFrameX(0);
	if (m_cursorCount % 4 == 0)
	{
		if (isForwards)
		{

			m_cursorCount = 0;
			dragonCursor->setFrameY(m_cursorIndex++);
			if (m_cursorIndex >= 3) m_cursorIndex = 3;
			if (m_cursorIndex == 2) aniDelayTime = GetTickCount();
			if (m_cursorIndex == 3)
			{
				if (GetTickCount() - aniDelayTime >= 300)
				{
					isForwards = false;
				}
			}
		}
		else if (!isForwards)
		{
			m_cursorCount = 0;
			dragonCursor->setFrameY(m_cursorIndex--);
			if (m_cursorIndex <= 0) m_cursorIndex = 0;
			if (m_cursorIndex == 1) aniDelayTime = GetTickCount();
			if (m_cursorIndex == 0)
			{
				if (GetTickCount() - aniDelayTime >= 300)
				{
					isForwards = true;
				}
			}
		}
	}
}

void mainmenu::control()
{
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		if (cursorPos.y == 180)
		{
			cursorPos.y = 255;
		}
		else
		{
			cursorPos.y -= 75;
		}		
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		if (cursorPos.y == 255)
		{
			cursorPos.y = 180;
		}
		else
		{
			cursorPos.y += 75;
		}		
	}
	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		if (cursorPos.y == 180)
		{
			ITEMMANAGER->init();
			SCENEMANAGER->changeScene("스테이지토크");
			//SCENEMANAGER->changeScene("스테이지3");
		}
		if (cursorPos.y == 255)
		{
			SCENEMANAGER->changeScene("맵툴");
		}
	}
}
