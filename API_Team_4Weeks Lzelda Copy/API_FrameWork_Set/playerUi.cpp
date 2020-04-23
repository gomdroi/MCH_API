#include "stdafx.h"
#include "playerUi.h"


playerUi::playerUi()
{
}


playerUi::~playerUi()
{
}

HRESULT playerUi::init()
{
	_upUiRc = RectMakeCenter(WINSIZEX / 2, 100, WINSIZEX - 100, 150);


	return S_OK;
}

void playerUi::update()
{
}

void playerUi::release()
{
}

void playerUi::render(HDC hdc)
{
	string numberTemp;
	image *numberImageTemp;
	int frameNumXTemp;

	//인벤창 킨게 아닐때만 UI보여주기
	if (!INVENTORYMANAGER->getIsInventory())
	{
		numberImageTemp = IMAGEMANAGER->findImage("숫자");

		numberTemp = to_string(INVENTORYMANAGER->getMoney());

		for (int i = numberTemp.size() - 1; i >= 0; i--)
		{
			frameNumXTemp = numberTemp[i] - '0';
			numberImageTemp->frameRender(hdc, _upUiRc.left + 165 + 21 * i, _upUiRc.top + 45, frameNumXTemp, 0);
		}

		numberTemp = to_string(INVENTORYMANAGER->getBomb());

		for (int i = numberTemp.size() - 1; i >= 0; i--)
		{
			frameNumXTemp = numberTemp[i] - '0';
			numberImageTemp->frameRender(hdc, _upUiRc.left + 260 + 21 * i, _upUiRc.top + 45, frameNumXTemp, 0);
		}

		numberTemp = to_string(INVENTORYMANAGER->getArrow());

		for (int i = numberTemp.size() - 1; i >= 0; i--)
		{
			frameNumXTemp = numberTemp[i] - '0';
			numberImageTemp->frameRender(hdc, _upUiRc.left + 340 + 21 * i, _upUiRc.top + 45, frameNumXTemp, 0);
		}

		numberTemp = to_string(INVENTORYMANAGER->getKey());

		for (int i = numberTemp.size() - 1; i >= 0; i--)
		{
			frameNumXTemp = numberTemp[i] - '0';
			numberImageTemp->frameRender(hdc, _upUiRc.left + 410 + 21 * i, _upUiRc.top + 45, frameNumXTemp, 0);
		}



		//현재 장착중인 아이템
		if (INVENTORYMANAGER->getNowItem() == BOW)
		{
			//if (INVENTORYMANAGER->getIsItem(BOW))
			IMAGEMANAGER->render("활UI", hdc, _upUiRc.left + 66, _upUiRc.top + 26);
		}
		if (INVENTORYMANAGER->getNowItem() == BOOMERANG)
		{
			IMAGEMANAGER->render("부메랑UI", hdc, _upUiRc.left + 66, _upUiRc.top + 26);
		}
		if (INVENTORYMANAGER->getNowItem() == BOMB)
		{
			IMAGEMANAGER->render("폭탄UI", hdc, _upUiRc.left + 66, _upUiRc.top + 26);
		}
		if (INVENTORYMANAGER->getNowItem() == HPMAX)
		{
			IMAGEMANAGER->render("체력UI", hdc, _upUiRc.left + 66, _upUiRc.top + 26);
		}

		IMAGEMANAGER->render("상단UI", hdc, _upUiRc.left, _upUiRc.top);

		//현재 체력 표시
		for (int i = 1; i <= _player.getMaxHP() / 2; i++)
		{
			if (i <= _player.getCurHP() / 2)
			{
				IMAGEMANAGER->render("체력1", hdc, _upUiRc.left + 513 + 21 * i, _upUiRc.top + 50);
			}
			else if ((_player.getCurHP() + 1) / 2 == i)
			{
				IMAGEMANAGER->render("체력2", hdc, _upUiRc.left + 513 + 21 * i, _upUiRc.top + 50);
			}
			else
			{
				IMAGEMANAGER->render("체력3", hdc, _upUiRc.left + 513 + 21 * i, _upUiRc.top + 50);
			}

		}
	}

}
