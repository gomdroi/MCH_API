#include "stdafx.h"
#include "mainGame.h"


mainGame::mainGame()
{
}


mainGame::~mainGame()
{
}

HRESULT mainGame::init()
{
	gameNode::init(true);

	SCENEMANAGER->addScene("스테이지1", new stage1);
	SCENEMANAGER->addScene("스테이지2", new stage2);
	SCENEMANAGER->addScene("스테이지3", new stage3);
	SCENEMANAGER->addScene("스테이지토크", new smallTalk);
	SCENEMANAGER->addScene("인트로", new intro);
	SCENEMANAGER->addScene("메인메뉴", new mainmenu);
	SCENEMANAGER->addScene("맵툴", new mapToolScene);


	SCENEMANAGER->changeScene("인트로");
	return S_OK;
}

void mainGame::release()
{
	gameNode::release();


}

void mainGame::update()
{
	gameNode::update();
	SCENEMANAGER->update();


}

void mainGame::render(/*HDC hdc*/)
{
	//흰색 비트맵
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
//====================================================
	SCENEMANAGER->render();
	TIMEMANAGER->render(getMemDC());
	//=====================================================
	//백버퍼의 내용을 HDC에 그린다.(지우지마!!)
	this->getBackBuffer()->render(getHDC(), 0, 0);
}









