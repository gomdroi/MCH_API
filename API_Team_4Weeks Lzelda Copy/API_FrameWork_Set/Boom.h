#pragma once
#include "equipment.h"
class boom : public equipment
{
public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void Move();
	virtual void Collision();
	boom();
	virtual ~boom();
};

