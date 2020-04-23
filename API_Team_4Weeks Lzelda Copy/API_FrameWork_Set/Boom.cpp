#include "stdafx.h"
#include "boom.h"


HRESULT boom::init()
{
	_isActive = false;
	_positionX = 0;
	_positionY = 0;
	_positionZ = 0;
	_equipAngle = 0;

	_speed = 0;
	_damage = 0;
	_range = 0;
	_targetObjectType = EMPTY_TYPE;
	_kind = EQUIPMENT_KIND::EQUIPMENT_KIND_BOOM;
	return S_OK;
}

void boom::release()
{
}

void boom::update()
{
	Collision();
}

void boom::render(HDC hdc)
{
	if (EQUIPMENTMANAGER->getIsBoom())
	{
		if (_isDebug)
		{
			Rectangle(hdc, _positionX, _positionY, _positionX + _img->getWidth() / 8, _positionY + _img->getHeight());
		}
		_img->aniRender(hdc, _positionX, _positionY, _ani);
	}
	else
	{
		if (_isDebug)
		{
			Rectangle(hdc, _rc.left, _rc.top, _positionX + _img->getWidth() / 8, _positionY + _img->getHeight());
		}
		_img->aniRender(hdc, _rc.left, _rc.top, _ani);
	}
}


void boom::Move()
{
}

void boom::Collision()
{
	vector<enemySet*> enemyList = ENEMYMANAGER->getEnemyList();
	for (int i = 0; i < enemyList.size(); i++)
	{
		if (!EQUIPMENTMANAGER->getIsBoom() == false) continue;
		RECT rTemp;
		if (IntersectRect(&rTemp, &_rc, &enemyList[i]->getRect()))
		{
			enemyList[i]->setHp(enemyList[i]->getHp() - 1);
			enemyList[i]->setIsKnockBack(true);
		}
	}
}

boom::boom()
{
}

boom::~boom()
{
}
