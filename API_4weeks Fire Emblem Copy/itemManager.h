#pragma once
#include "singletonBase.h"

class itemSet;
class itemManager : public singletonBase<itemManager>
{
private:
	vector<itemSet*> shopVector;
public:
	itemManager();
	~itemManager();

	virtual HRESULT init();
	virtual void update();
	virtual void animation();
	virtual void render();
	virtual void release();

	vector<itemSet*> getShopVector() { return shopVector; }
};

