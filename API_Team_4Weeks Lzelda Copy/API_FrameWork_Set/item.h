#pragma once
#include "gameNode.h"

struct tagItem
{
	image* itemImage;
	ITEM itemKind;
	RECT itemRc1;
	POINT destination;
	POINT itemPosition;
	bool itemUp;
};

class item : public gameNode
{
private:

	vector<tagItem> _vItem;
	vector<tagItem>::iterator _viItem;

	float itemSpeed;

	int count;
	int index;

public:
	item();
	~item();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//æ∆¿Ã≈€ »πµÊ
	void acquisition();

	void addItem(tagItem item);

	float getItemSpeed() { return itemSpeed; };

	void itemMove();
};


