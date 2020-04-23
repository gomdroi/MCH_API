#include "stdafx.h"
#include "iniDataTest.h"

iniDataTest::iniDataTest()
{
}

iniDataTest::~iniDataTest()
{
}

HRESULT iniDataTest::init()
{
	return S_OK;
}

void iniDataTest::release()
{
}

void iniDataTest::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		INIDATA->addData("대형", "코골지마", "500");
		INIDATA->addData("재상", "엄마", "70");
		INIDATA->addData("명준", "엄마한테 개기지마", "100");

		INIDATA->iniSave("KYUNGIL");
	}

}

void iniDataTest::render()
{
	char str[128];

	//파일이 없거나 잘못된 값일 경우 0이 나옴
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		sprintf_s(str, "%d",
			INIDATA->loadDataIntrger("KYUNGIL", "명준", "엄마한테 개기지마"));
		SetTextColor(getMemDC(), RGB(255, 0, 0));
		TextOut(getMemDC(), WINSIZEX / 2, WINSIZEY / 2, str, strlen(str));

	}
	
}
