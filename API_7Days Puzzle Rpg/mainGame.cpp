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

	IMAGEMANAGER->addImage("�������ͽ�", "images/MenuTest.bmp", 300, WINSIZEY,true, RGB(255,0,255));
	IMAGEMANAGER->addImage("�ʺ���", "images/mapBoardBack.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Ÿ�Ϻ���", "images/TileBack.bmp", 300, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������1�׸�", "images/stage1green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������2�׸�", "images/stage2green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������3�׸�", "images/stage3green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������4�׸�", "images/stage4green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������5�׸�", "images/stage5green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������6�׸�", "images/stage6green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������7�׸�", "images/stage7green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������8�׸�", "images/stage8green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������9�׸�", "images/stage9green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������10�׸�", "images/stage10green.bmp", 424, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("����", "images/YouDied.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("������", "images/Opening.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�¸�", "images/YouWin.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));

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
	//��� ��Ʈ��
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//====================================================
	stageBackRender(m_player->stageCount);
	IMAGEMANAGER->render("�������ͽ�", getMemDC());
	IMAGEMANAGER->render("Ÿ�Ϻ���", getMemDC(),724,0);

	TILEMANAGER->render();
	EVENTMANAGER->render();
	m_player->render();
	MONSTERMANAGER->render();	
	ITEMMANAGER->render();
	if (isOpening)
	{
		IMAGEMANAGER->render("������", getMemDC());
	}

	if (m_player->getIsWin())
	{
		IMAGEMANAGER->render("�¸�", getMemDC(), 0, 0);
	}

	if (m_player->getCurHp() <= 0)
	{
		m_player->setCurHp(-1000);
		IMAGEMANAGER->render("����", getMemDC(), 0, 0);
	}
	
	//����Ʈ üũ�� ���콺 ��ġ
	wsprintf(mousePointerStr, "X : %d, y : %d", m_ptMouse.x, m_ptMouse.y);
	TextOut(getMemDC(), 310, 750, mousePointerStr, strlen(mousePointerStr));
	//=====================================================
	//������� ������ HDC�� �׸���.(��������!!)
	this->getBackBuffer()->render(hdc, 0, 0);
}

void mainGame::stageBackRender(int _stage)
{
	switch (_stage)
	{
	case 1:
		IMAGEMANAGER->render("��������1�׸�", getMemDC(), 300, 0);
		break;
	case 2:
		IMAGEMANAGER->render("��������2�׸�", getMemDC(), 300, 0);
		break;
	case 3:
		IMAGEMANAGER->render("��������3�׸�", getMemDC(), 300, 0);
		break;
	case 4:
		IMAGEMANAGER->render("��������4�׸�", getMemDC(), 300, 0);
		break;
	case 5:
		IMAGEMANAGER->render("��������5�׸�", getMemDC(), 300, 0);
		break;
	case 6:
		IMAGEMANAGER->render("��������6�׸�", getMemDC(), 300, 0);
		break;
	case 7:
		IMAGEMANAGER->render("��������7�׸�", getMemDC(), 300, 0);
		break;
	case 8:
		IMAGEMANAGER->render("��������8�׸�", getMemDC(), 300, 0);
		break;
	case 9:
		IMAGEMANAGER->render("��������9�׸�", getMemDC(), 300, 0);
		break;
	case 10:
		IMAGEMANAGER->render("��������10�׸�", getMemDC(), 300, 0);
		break;
	default:
		break;
	}
}









