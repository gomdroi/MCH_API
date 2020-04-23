#pragma once
#include "item.h"
#include "singletonBase.h"

class itemManager : public singletonBase<itemManager>
{
private:
	item* _item;

public:
	itemManager();
	~itemManager();

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);


	void dropRandItem(int x, int y);
	void dropHpItem(int x, int y, int dropPercentage);
	void dropMoneyItem(int x, int y, int dropPercentage);
	void dropPendent(int x, int y, ITEM pendentColor);

	void dropMaxHp(int x, int y);
	void dropKey(int x, int y);

	float getItemSpeed() { return _item->getItemSpeed(); };
};

