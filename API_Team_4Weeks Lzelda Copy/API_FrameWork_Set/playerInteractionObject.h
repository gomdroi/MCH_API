#pragma once
#include "equipment.h"

class playerInteractionObject : public equipment
{
private:
	int startX, startY;
	bool _isfire;
	INTERACTIONOBJECT _interaction_Object_Kind;

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void Move();
	virtual void Collision();

	virtual void setInterectObjKind(INTERACTIONOBJECT interaction_Object_Kind) { _interaction_Object_Kind= interaction_Object_Kind; };

	playerInteractionObject();
	~playerInteractionObject();
};

