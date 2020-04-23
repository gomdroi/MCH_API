#pragma once
#include "equipment.h"
class arrow : public equipment
{
private:
public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void Move();
	virtual void Collision();
	arrow();
	virtual ~arrow();
};

