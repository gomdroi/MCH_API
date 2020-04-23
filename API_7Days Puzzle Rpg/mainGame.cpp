#include "stdafx.h"
#include "mainGame.h"
#include "player.h"

mainGame::mainGame()
{
}


mainGame::~mainGame()
{
}

HRESULT mainGame::init()
{
	gameNode::init(true);

	IMAGEMANAGER->addImage("스테이터스", "images/MenuTest.bmp", 300, WINSIZEY,true, RGB(255,0,255));
	IMAGEMANAGER->addImage("맵보드", "images/mapBoardBack.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("타일보드", "images/TileBack.bmp", 300, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지1그린", "images/stage1green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지2그린", "images/stage2green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지3그린", "images/stage3green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지4그린", "images/stage4green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지5그린", "images/stage5green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지6그린", "images/stage6green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지7그린", "images/stage7green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지8그린", "images/stage8green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지9그린", "images/stage9green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지10그린", "images/stage10green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("죽음", "images/YouDied.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("오프닝", "images/Opening.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("승리", "images/YouWin.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

	MONSTERMANAGER-> init();
	EVENTMANAGER->init();
	ITEMMANAGER->init();
	TILEMANAGER->init();
	m_player = new player;
	m_player->init();
	MONSTERMANAGER->init(STAGE_ONE,4);
	EVENTMANAGER->init(MONSTERMANAGER->getStage(), 4);
	TILEMANAGER->stageInit(m_player->stageCount);
	
	isOpening = true;
	return S_OK;
}

void mainGame::release()
{
	gameNode::release();
	MONSTERMANAGER->release();
	EVENTMANAGER->release();
	ITEMMANAGER->release();
	SAFE_DELETE(m_player);
	
}

void mainGame::update()
{
	gameNode::update();
	MONSTERMANAGER->update();
	EVENTMANAGER->update();
	ITEMMANAGER->update();
	TILEMANAGER->update();
	
	m_player->update();
	
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		isOpening = false;
	}
}

void mainGame::render(HDC hdc)
{
	//흰색 비트맵
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//====================================================
	stageBackRender(m_player->stageCount);
	IMAGEMANAGER->render("스테이터스", getMemDC());
	IMAGEMANAGER->render("타일보드", getMemDC(),724,0);

	TILEMANAGER->render();
	EVENTMANAGER->render();
	m_player->render();
	MONSTERMANAGER->render();	
	ITEMMANAGER->render();
	if (isOpening)
	{
		IMAGEMANAGER->render("오프닝", getMemDC());
	}

	if (m_player->getIsWin())
	{
		IMAGEMANAGER->render("승리", getMemDC(), 0, 0);
	}

	if (m_player->getCurHp() <= 0)
	{
		m_player->setCurHp(-1000);
		IMAGEMANAGER->render("죽음", getMemDC(), 0, 0);
	}
	
	//포인트 체크용 마우스 위치
	wsprintf(mousePointerStr, "X : %d, y : %d", m_ptMouse.x, m_ptMouse.y);
	TextOut(getMemDC(), 310, 750, mousePointerStr, strlen(mousePointerStr));
	//=====================================================
	//백버퍼의 내용을 HDC에 그린다.(지우지마!!)
	this->getBackBuffer()->render(hdc, 0, 0);
}

void mainGame::stageBackRender(int _stage)
{
	switch (_stage)
	{
	case 1:
		IMAGEMANAGER->render("스테이지1그린", getMemDC(), 300, 0);
		break;
	case 2:
		IMAGEMANAGER->render("스테이지2그린", getMemDC(), 300, 0);
		break;
	case 3:
		IMAGEMANAGER->render("스테이지3그린", getMemDC(), 300, 0);
		break;
	case 4:
		IMAGEMANAGER->render("스테이지4그린", getMemDC(), 300, 0);
		break;
	case 5:
		IMAGEMANAGER->render("스테이지5그린", getMemDC(), 300, 0);
		break;
	case 6:
		IMAGEMANAGER->render("스테이지6그린", getMemDC(), 300, 0);
		break;
	case 7:
		IMAGEMANAGER->render("스테이지7그린", getMemDC(), 300, 0);
		break;
	case 8:
		IMAGEMANAGER->render("스테이지8그린", getMemDC(), 300, 0);
		break;
	case 9:
		IMAGEMANAGER->render("스테이지9그린", getMemDC(), 300, 0);
		break;
	case 10:
		IMAGEMANAGER->render("스테이지10그린", getMemDC(), 300, 0);
		break;
	default:
		break;
	}
}









