#include "stdafx.h"
#include "shop.h"


shop::shop()
{
}


shop::~shop()
{
}

HRESULT shop::init()
{
	tagShopItem sItem;
	ZeroMemory(&sItem, sizeof(tagShopItem));

	sItem.itemImage = IMAGEMANAGER->findImage("활");
	sItem.itemKind = BOW;
	sItem.price = 30;
	sItem.num = 0;
	_vSItem.push_back(sItem);

	sItem.itemImage = IMAGEMANAGER->findImage("부메랑");
	sItem.itemKind = BOOMERANG;
	sItem.price = 50;
	sItem.num = 1;
	_vSItem.push_back(sItem);

	sItem.itemImage = IMAGEMANAGER->findImage("폭탄10개");
	sItem.itemKind = BOMB;
	sItem.price = 40;
	sItem.num = 2;
	_vSItem.push_back(sItem);

	sItem.itemImage = IMAGEMANAGER->findImage("체력MAX");
	sItem.itemKind = HPMAX;
	sItem.price = 20;
	sItem.num = 3;
	_vSItem.push_back(sItem);

	sItem.itemImage = IMAGEMANAGER->findImage("화살10개");
	sItem.itemKind = SHOPARROW10;
	sItem.price = 20;
	sItem.num = 4;
	_vSItem.push_back(sItem);

	randomSetting();


	npc = RectMakeCenter(WINSIZEX / 2, 310, 42, 66);
	shopNpc = IMAGEMANAGER->findImage("상점상인");
	_index = 0;
	_count = 0;
	return S_OK;
}

void shop::release()
{
	vector<tagShopItem>::iterator eIter = _vSItem.begin();
	for (eIter; eIter != _vSItem.end();)
	{
		//SAFE_DELETE((eIter));
		eIter = _vSItem.erase(eIter);
	}

}

void shop::update()
{
	buyItem();
	collision();

	_count++;
	shopNpc->setFrameY(0);
	if (_count % 12 == 0)
	{
		_index++;
		if (_index > shopNpc->getMaxFrameX())
		{
			_index = 0;
		}
		shopNpc->setFrameX(_index);
	}
}

void shop::render(HDC hdc)
{
	string numberTemp;
	image *numberImageTemp;
	int frameNumXTemp;
	numberImageTemp = IMAGEMANAGER->findImage("숫자");

	for (_viSItem = _vSItem.begin(); _viSItem != _vSItem.end(); ++_viSItem)
	{
		//선택된 아이템만 그리기
		if (_viSItem->num == setItem || _viSItem->num == setItem1 || _viSItem->num == setItem2)
		{
			//상점아이템
			_viSItem->itemImage->render(hdc, _viSItem->itemRc1.left, _viSItem->itemRc1.top);

			//상점아이템 가격표
			char str[128];
			numberTemp = to_string(_viSItem->price);
			for (int i = numberTemp.size() - 1; i >= 0; i--)
			{
				frameNumXTemp = numberTemp[i] - '0';
				numberImageTemp->frameRender(hdc, _viSItem->itemRc1.left + 21 * i, _viSItem->itemRc1.bottom + 10, frameNumXTemp, 0);
			}
		}
		if (_isDebug)
		{
			//Rectangle(hdc, _viSItem->itemRc2.left, _viSItem->itemRc2.top, _viSItem->itemRc2.right, _viSItem->itemRc2.bottom);
			Rectangle(hdc, _viSItem->itemRc1.left, _viSItem->itemRc1.top, _viSItem->itemRc1.right, _viSItem->itemRc1.bottom);
		}
	}

	IMAGEMANAGER->frameRender("상점상인", hdc, npc.left, npc.top, shopNpc->getFrameX(), shopNpc->getFrameY());
}

void shop::buyItem()
{
	//상점아이템 제거
	for (_viSItem = _vSItem.begin(); _viSItem != _vSItem.end(); )
	{
		RECT temp;   //플레이어 아이템 충돌											가진돈이 아이템가격보다 많다					s키눌러서 구입
		if (IntersectRect(&temp, &_player.getRect(), &_viSItem->itemRc2) && (INVENTORYMANAGER->getMoney() >= _viSItem->price) && KEYMANAGER->isOnceKeyDown('S'))
		{
			INVENTORYMANAGER->setMoney(INVENTORYMANAGER->getMoney() - _viSItem->price);
			//폭탄갯수증가
			if (_viSItem->itemKind == BOMB)
			{
				INVENTORYMANAGER->setBomb(INVENTORYMANAGER->getBomb() + 10);
			}
			//화살갯수증가
			if (_viSItem->itemKind == SHOPARROW10)
			{
				INVENTORYMANAGER->setArrow(INVENTORYMANAGER->getArrow() + 10);
			}
			if (_viSItem->itemKind != SHOPARROW10)
			{
				INVENTORYMANAGER->setIsItem(_viSItem->itemKind, true);
			}
			_viSItem = _vSItem.erase(_viSItem);
			//INVENTORYMANAGER->addItem(Initem1);
		}
		else
		{
			++_viSItem;
		}
	}
}

//상점 아이템 플레이어 충돌
void shop::collision()
{
	POINT _playerPoint = { _player.getPositionX() + 25, _player.getPositionY() + 30 };

	for (_viSItem = _vSItem.begin(); _viSItem != _vSItem.end(); ++_viSItem)
	{
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			if (PtInRect(&_viSItem->itemRc1, POINT{ _playerPoint.x - (_player.getRect().right - _player.getRect().left) / 2 + 1, _playerPoint.y + (_player.getRect().bottom - _player.getRect().top) / 2 })
				|| (PtInRect(&_viSItem->itemRc1, POINT{ _playerPoint.x + (_player.getRect().right - _player.getRect().left) / 2 - 12, _playerPoint.y + (_player.getRect().bottom - _player.getRect().top) / 2 })))
			{
				_player.setPositionY(_viSItem->itemRc1.top - (_player.getRect().bottom - _player.getRect().top) / 2 - 30);
				_player.setRect(RectMakeCenter(_player.getPositionX() + 25, _player.getPositionY() + 30, (_player.getRect().right - _player.getRect().left), (_player.getRect().bottom - _player.getRect().top)));
			}
		}

		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			if (PtInRect(&_viSItem->itemRc1, POINT{ _playerPoint.x - (_player.getRect().right - _player.getRect().left) / 2,_playerPoint.y - (_player.getRect().bottom - _player.getRect().top) / 2 + 1 })
				|| (PtInRect(&_viSItem->itemRc1, POINT{ _playerPoint.x - (_player.getRect().right - _player.getRect().left) / 2, _playerPoint.y + (_player.getRect().bottom - _player.getRect().top) / 2 - 15 }))
				|| (PtInRect(&_viSItem->itemRc1, POINT{ _playerPoint.x - (_player.getRect().right - _player.getRect().left) / 2, _playerPoint.y })))
			{
				_player.setPositionX(_viSItem->itemRc1.right + (_player.getRect().right - _player.getRect().left) / 2 - 25);
				_player.setRect(RectMakeCenter(_player.getPositionX() + 25, _player.getPositionY() + 30, (_player.getRect().right - _player.getRect().left), (_player.getRect().bottom - _player.getRect().top)));
			}
		}
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			if (PtInRect(&_viSItem->itemRc1, POINT{ _playerPoint.x + (_player.getRect().right - _player.getRect().left) / 2,_playerPoint.y - (_player.getRect().bottom - _player.getRect().top) / 2 + 1 })
				|| (PtInRect(&_viSItem->itemRc1, POINT{ _playerPoint.x + (_player.getRect().right - _player.getRect().left) / 2, _playerPoint.y + (_player.getRect().bottom - _player.getRect().top) / 2 - 15 }))
				|| (PtInRect(&_viSItem->itemRc1, POINT{ _playerPoint.x + (_player.getRect().right - _player.getRect().left) / 2, _playerPoint.y })))
			{
				_player.setPositionX(_viSItem->itemRc1.left - (_player.getRect().right - _player.getRect().left) / 2 - 25);
				_player.setRect(RectMakeCenter(_player.getPositionX() + 25, _player.getPositionY() + 30, (_player.getRect().right - _player.getRect().left), (_player.getRect().bottom - _player.getRect().top)));
			}
		}
		if (KEYMANAGER->isStayKeyDown(VK_UP))
		{
			if (PtInRect(&_viSItem->itemRc1, POINT{ _playerPoint.x - (_player.getRect().right - _player.getRect().left) / 2 + 1, _playerPoint.y - (_player.getRect().bottom - _player.getRect().top) / 2 })
				|| (PtInRect(&_viSItem->itemRc1, POINT{ _playerPoint.x + (_player.getRect().right - _player.getRect().left) / 2 - 12, _playerPoint.y - (_player.getRect().bottom - _player.getRect().top) / 2 })))
			{
				_player.setPositionY(_viSItem->itemRc1.bottom + (_player.getRect().bottom - _player.getRect().top) / 2 - 30);
				_player.setRect(RectMakeCenter(_player.getPositionX() + 25, _player.getPositionY() + 30, (_player.getRect().right - _player.getRect().left), (_player.getRect().bottom - _player.getRect().top)));
			}
		}
	}

}

void shop::randomSetting()
{
	//첫번째 상점아이템
	setItem = RND->getInt(SHOPITEMCOUNT - 1);
	for (_viSItem = _vSItem.begin(); _viSItem != _vSItem.end(); ++_viSItem)
	{
		if (_viSItem->num == setItem)
		{
			_viSItem->itemRc1 = RectMakeCenter(230, 440, 47, 47);
			_viSItem->itemRc2 = RectMakeCenter(230, 440, 87, 87);
		}
	}

	//두번째 상점아이템
	setItem1 = RND->getInt(SHOPITEMCOUNT - 1);
	for (_viSItem = _vSItem.begin(); _viSItem != _vSItem.end(); ++_viSItem)
	{
		while (1)
		{
			//똑같은 번호가 나올시 다시돌려라
			if (setItem1 == setItem)
			{
				setItem1 = RND->getInt(3);
			}
			else
			{
				break;
			}
		}
		if (_viSItem->num == setItem1)
		{
			_viSItem->itemRc1 = RectMakeCenter(380, 440, 47, 47);
			_viSItem->itemRc2 = RectMakeCenter(380, 440, 87, 87);
		}
	}
	//세번째 상점아이템
	setItem2 = RND->getInt(SHOPITEMCOUNT - 1);
	for (_viSItem = _vSItem.begin(); _viSItem != _vSItem.end(); ++_viSItem)
	{
		while (1)
		{
			//똑같은 번호가 나올시 다시돌려라
			if (setItem2 == setItem || setItem2 == setItem1)
			{
				setItem2 = RND->getInt(3);
			}
			else
			{
				break;
			}
		}
		if (_viSItem->num == setItem2)
		{
			_viSItem->itemRc1 = RectMakeCenter(530, 440, 47, 47);
			_viSItem->itemRc2 = RectMakeCenter(530, 440, 87, 87);
		}
	}

}
