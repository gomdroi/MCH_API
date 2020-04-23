#include "stdafx.h"
#include "equipment.h"

HRESULT equipment::init()
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
	_kind = EQUIPMENT_KIND_NULL;

	return S_OK;
}
void equipment::release()
{

}

void equipment::update()
{

}

void equipment::render(HDC hdc)
{

}

equipment::equipment()
{
}


equipment::~equipment()
{
}
