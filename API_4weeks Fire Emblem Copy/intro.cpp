#include "stdafx.h"
#include "intro.h"


intro::intro()
{
}


intro::~intro()
{
}

HRESULT intro::init()
{
	return S_OK;
}

void intro::update()
{
	control();
}

void intro::render()
{
	IMAGEMANAGER->render("��Ʈ��", getMemDC());
}

void intro::release()
{
}

void intro::control()
{
	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		SCENEMANAGER->changeScene("���θ޴�");
	}
}
