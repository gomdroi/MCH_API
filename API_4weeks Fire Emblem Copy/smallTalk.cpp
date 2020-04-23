#include "stdafx.h"
#include "smallTalk.h"


smallTalk::smallTalk()
{
}


smallTalk::~smallTalk()
{
}

HRESULT smallTalk::init()
{	


	talking = new image;
	talking = IMAGEMANAGER->findImage("����������ũ1_1");
	talkNum = 1;
	return S_OK;
}

void smallTalk::update()
{
	control();
	if (g_cursor.getStageNum() == 1)
	{
		switch (talkNum)
		{
		case 1:
			talking = IMAGEMANAGER->findImage("����������ũ1_1");
			break;
		case 2:
			talking = IMAGEMANAGER->findImage("����������ũ1_2");
			break;
		case 3:
			talking = IMAGEMANAGER->findImage("����������ũ1_3");
			break;
		case 4:
			talking = IMAGEMANAGER->findImage("����������ũ1_4");
			break;
		case 5:
			SCENEMANAGER->changeScene("��������1");
			break;
		default:
			break;
		}
	}	
	else if (g_cursor.getStageNum() == 2)
	{
		switch (talkNum)
		{
		case 1:
			talking = IMAGEMANAGER->findImage("����������ũ2_1");
			break;
		case 2:
			talking = IMAGEMANAGER->findImage("����������ũ2_2");
			break;
		case 3:
			talking = IMAGEMANAGER->findImage("����������ũ2_3");
			break;
		case 4:
			SCENEMANAGER->changeScene("��������2");
			break;
		default:
			break;
		}
	}
	else if (g_cursor.getStageNum() == 3)
	{
		switch (talkNum)
		{
		case 1:
			talking = IMAGEMANAGER->findImage("����������ũ3_1");
			break;
		case 2:
			talking = IMAGEMANAGER->findImage("����������ũ3_2");
			break;
		case 3:
			talking = IMAGEMANAGER->findImage("����������ũ3_3");
			break;
		case 4:
			talking = IMAGEMANAGER->findImage("����������ũ3_4");		
			break;
		case 5:
			SCENEMANAGER->changeScene("��������3");
			break;
		default:
			break;
		}
	}
	else if (g_cursor.getStageNum() == 4)
	{
		switch (talkNum)
		{
		case 1:
			talking = IMAGEMANAGER->findImage("����1");
			break;
		case 2:
			talking = IMAGEMANAGER->findImage("����2");
			break;
		case 3:
			SCENEMANAGER->changeScene("���θ޴�");
			break;
		default:
			break;
		}
	}
}

void smallTalk::render()
{
	talking->render(getMemDC(),0,0);
}

void smallTalk::release()
{
	SAFE_DELETE(talking);
}

void smallTalk::control()
{
	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		talkNum += 1;		
	}
}
