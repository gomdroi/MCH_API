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

	SCENEMANAGER->addScene("��������1", new stage1);
	SCENEMANAGER->addScene("��������2", new stage2);
	SCENEMANAGER->addScene("��������3", new stage3);
	SCENEMANAGER->addScene("����������ũ", new smallTalk);
	SCENEMANAGER->addScene("��Ʈ��", new intro);
	SCENEMANAGER->addScene("���θ޴�", new mainmenu);
	SCENEMANAGER->addScene("����", new mapToolScene);


	SCENEMANAGER->changeScene("��Ʈ��");
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
	//��� ��Ʈ��
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
//====================================================
	SCENEMANAGER->render();
	TIMEMANAGER->render(getMemDC());
	//=====================================================
	//������� ������ HDC�� �׸���.(��������!!)
	this->getBackBuffer()->render(getHDC(), 0, 0);
}









