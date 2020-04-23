#include "stdafx.h"
#include "boomerang.h"


HRESULT boomerang::init()
{
	_isActive = false;
	_isGuide = false;
	_positionX = 0;
	_positionY = 0;
	_positionZ = 0;
	_fireX = 0;
	_fireY = 0;
	_equipAngle = 0;
	_speed = 10.0f;
	_damage = 0;
	_range = 350.0f;
	_targetObjectType = EMPTY_TYPE;
	_kind = EQUIPMENT_KIND_BOOMERANG;
	return S_OK;
}

void boomerang::release()
{
}

void boomerang::update()
{
	Move();
}

void boomerang::render(HDC hdc)
{
	if (_isDebug)
	{
		Rectangle(hdc, _positionX, _positionY, _positionX + _img->getWidth() / 4, _positionY + _img->getHeight());
	}

	_img->aniRender(hdc, _positionX, _positionY, _ani);
}

void boomerang::Move()
{
	vector<enemySet*> enemyList = ENEMYMANAGER->getEnemyList();
	_positionX += cosf(_equipAngle) * _speed;
	_positionY += -sinf(_equipAngle) * _speed;
	if (_range < getDistance((float)_player.getPositionX(), (float)_player.getPositionY(), _positionX, _positionY))
	{
		_isGuide = true;
	}
	for (int i = 0; i < enemyList.size(); i++)
	{
		//부메랑과 적의 충돌체크
		float distanceEnemy = getDistance(_positionX, _positionY, enemyList[i]->getPositionX(), enemyList[i]->getPositionY()); // 부메랑과 에너미사이의 거리
		if (distanceEnemy <= 50)
		{
			enemyList[i]->setIsStunAtk(true);
			enemyList[i]->setIsKnockBack(true);
			_isGuide = true;
		}
	}
	if (_isGuide == true)
	{
		if (Collision())
		{
			return;
		}
		_equipAngle = getAngle(_positionX, _positionY, (float)_player.getPositionX(), (float)_player.getPositionY());
	}
}

bool boomerang::Collision()
{

	float distance = getDistance(_positionX, _positionY, (float)_player.getPositionX(), (float)_player.getPositionY());

	if (distance < 50)
	{

		_isActive = false;
		_isGuide = false;
		return true;
	}

	return false;
}

boomerang::boomerang()
{
}


boomerang::~boomerang()
{
}
