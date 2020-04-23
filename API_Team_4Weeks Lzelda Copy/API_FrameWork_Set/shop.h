#pragma once
#include "gameNode.h"
#define SHOPITEMCOUNT 5
struct tagShopItem
{
	image* itemImage;

	SHOPITEM itemKind;

	RECT itemRc1;
	RECT itemRc2;

	int price;
	int num;
};

class shop : public gameNode
{
private:
	int setItem;
	int setItem1;
	int setItem2;
	RECT npc;
	image* shopNpc;

	int _count;
	int _index;
public:
	shop();
	~shop();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void buyItem();
	void collision();
	void randomSetting();

	vector<tagShopItem> _vSItem;
	vector<tagShopItem>::iterator _viSItem;

};

