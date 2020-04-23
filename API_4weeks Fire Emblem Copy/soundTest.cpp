#include "stdafx.h"
#include "soundTest.h"


soundTest::soundTest()
{
}


soundTest::~soundTest()
{
}

HRESULT soundTest::init()
{
	SOUNDMANAGER->addSound("�ڻ��", "sound/������ũ.mp3", true, true);
	//SOUNDMANAGER->play("�ڻ��");
	return S_OK;
}

void soundTest::release()
{
}

void soundTest::update()
{
	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		SOUNDMANAGER->play("�ڻ��", 1.0f);
	}
	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		SOUNDMANAGER->stop("�ڻ��");
	}
	if (KEYMANAGER->isOnceKeyDown('T'))
	{
		SOUNDMANAGER->resume("�ڻ��");
	}
	if (KEYMANAGER->isOnceKeyDown('P'))
	{
		SOUNDMANAGER->pause("�ڻ��");
	}

}
