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
		INIDATA->addData("����", "�ڰ�����", "500");
		INIDATA->addData("���", "����", "70");
		INIDATA->addData("����", "�������� ��������", "100");

		INIDATA->iniSave("KYUNGIL");
	}

}

void iniDataTest::render()
{
	char str[128];

	//������ ���ų� �߸��� ���� ��� 0�� ����
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		sprintf_s(str, "%d",
			INIDATA->loadDataIntrger("KYUNGIL", "����", "�������� ��������"));
		SetTextColor(getMemDC(), RGB(255, 0, 0));
		TextOut(getMemDC(), WINSIZEX / 2, WINSIZEY / 2, str, strlen(str));

	}
	
}
