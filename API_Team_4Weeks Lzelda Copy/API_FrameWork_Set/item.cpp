#include "stdafx.h"
#include "item.h"


item::item()
{
}


item::~item()
{
	release();
}

HRESULT item::init()
{
	itemSpeed = 2.0f;
	count = 0;
	index = 0;
	return S_OK;
}

void item::release()
{
	_vItem.clear();
}

void item::update()
{
	acquisition();
	itemMove();

	//돈 애니메이션
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		if (_viItem->itemKind == MONEY)
		{
			count++;
			_viItem->itemImage->setFrameY(0);
			if (count % 5 == 0)
			{
				count = 0;
				index++;
				if (index > 8)
				{
					index = 0;
				}
				_viItem->itemImage->setFrameX(index);
			}
		}
	}
}

void item::render(HDC hdc)
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		if (!_viItem->itemKind == MONEY)
			_viItem->itemImage->render(hdc, _viItem->itemRc1.left, _viItem->itemRc1.top);
		if (_viItem->itemKind == MONEY)
		{
			_viItem->itemImage->frameRender(hdc, _viItem->itemRc1.left, _viItem->itemRc1.top, _viItem->itemImage->getFrameX(), _viItem->itemImage->getFrameY());
		}
	}
}

//아이템획득
void item::acquisition()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end();)
	{
		RECT temp;
		if (IntersectRect(&temp, &_player.getRect(), &_viItem->itemRc1))
		{
			if (_viItem->itemKind == MONEY)
			{
				INVENTORYMANAGER->setMoney(INVENTORYMANAGER->getMoney() + 5);
			}
			if (_viItem->itemKind == ARROW10)
			{
				INVENTORYMANAGER->setArrow(INVENTORYMANAGER->getArrow() + 10);
			}

			if (_viItem->itemKind == LIFE)
			{
				_player.setCurHP(_player.getCurHP() + 1);
				_sound.effSoundPlay("하트");
			}
			if (_viItem->itemKind == HPMAXUP)
			{
				_player.setMaxHP(_player.getMaxHP() + 2);
				_sound.effSoundPlay("하트증가");
			}
			if (_viItem->itemKind == KEY)
			{
				INVENTORYMANAGER->setKey(INVENTORYMANAGER->getKey() + 1);
			}
			if (_viItem->itemKind == GREENPENDENT)
			{
				INVENTORYMANAGER->setGreenPendent(true);
			}
			if (_viItem->itemKind == REDPENDENT)
			{
				INVENTORYMANAGER->setRedPendent(true);
			}
			if (_viItem->itemKind == BLUEPENDENT)
			{
				INVENTORYMANAGER->setBluePendent(true);
			}
			_viItem = _vItem.erase(_viItem);
		}
		else
		{
			++_viItem;
		}
	}
}

void item::addItem(tagItem item)
{
	_vItem.push_back(item);
}

void item::itemMove()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		if (getDistance(_viItem->itemPosition.x, _viItem->itemPosition.y, _viItem->destination.x, _viItem->destination.y) < 2.0f)
		{
			_viItem->itemUp = false;
		}
		//	else if (getDistance(_viItem->itemPosition.x, _viItem->itemPosition.y, _viItem->destination.x, _viItem->destination.y) >= 2.0f)
		//	{
		//		_viItem->itemUp
		//	}

		if (_viItem->itemUp)
		{
			_viItem->itemRc1.top -= itemSpeed;
			_viItem->itemRc1.bottom -= itemSpeed;
			_viItem->itemPosition.y -= itemSpeed;
		}
		if (!_viItem->itemUp)
		{
			if (getDistance(_viItem->itemPosition.x, _viItem->itemPosition.y, _viItem->destination.x, _viItem->destination.y) < 19.0f)
			{
				_viItem->itemRc1.top += itemSpeed;
				_viItem->itemRc1.bottom += itemSpeed;
				_viItem->itemPosition.y += itemSpeed;
			}
		}
	}
}


