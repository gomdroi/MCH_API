#include "stdafx.h"
#include "inventory.h"


inventory::inventory()
{
}


inventory::~inventory()
{
}

void inventory::init(RECT inven)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			_invenRc[i][j] = RectMake(inven.left + 73 + j * 71, inven.top + 48 + i * 72, 47, 47);
		}
	}

	tagInvenItem Initem;
	ZeroMemory(&Initem, sizeof(tagInvenItem));

	Initem.itemImage = IMAGEMANAGER->findImage("활");
	Initem.itemKind = BOW;
	Initem.itemRc = _invenRc[0][0];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemImage = IMAGEMANAGER->findImage("부메랑");
	Initem.itemKind = BOOMERANG;
	Initem.itemRc = _invenRc[0][1];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[0][2];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemImage = IMAGEMANAGER->findImage("폭탄");
	Initem.itemKind = BOMB;
	Initem.itemRc = _invenRc[0][3];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[0][4];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[1][0];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[1][1];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[1][2];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[1][3];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[1][4];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemImage = IMAGEMANAGER->findImage("체력MAX");
	Initem.itemKind = HPMAX;
	Initem.itemRc = _invenRc[2][0];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[2][1];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[2][2];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[2][3];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[2][4];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[3][0];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[3][1];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[3][2];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[3][3];
	Initem.isItem = false;
	addItem(Initem);

	Initem.itemRc = _invenRc[3][4];
	Initem.isItem = false;
	addItem(Initem);


}

void inventory::update()
{

}

void inventory::release()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			_vInItem[i * 5 + i].isItem = false;
		}
	}
}

void inventory::render(HDC hdc)
{
	if (_isDebug)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				Rectangle(hdc, getInvenRc(j, i).left, getInvenRc(j, i).top, getInvenRc(j, i).right, getInvenRc(j, i).bottom);
			}
		}
	}
	//인벤칸 그려주기
	for (_viInItem = _vInItem.begin(); _viInItem != _vInItem.end(); ++_viInItem)
	{
		if (_viInItem->isItem)
		{
			_viInItem->itemImage->render(hdc, _viInItem->itemRc.left, _viInItem->itemRc.top);
		}
	}
}

void inventory::addItem(SHOPITEM itemKind)
{
}

void inventory::addItem(tagInvenItem item)
{
	_vInItem.push_back(item);
}

void inventory::getInvenRc(RECT inven)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			//인벤칸 움직이기
			_invenRc[i][j] = RectMake(inven.left + 73 + j * 71, inven.top + 48 + i * 72, 47, 47);
			//이미지도 움직이기
			_vInItem[i * 5 + j].itemRc = _invenRc[i][j];

		}
	}
}

