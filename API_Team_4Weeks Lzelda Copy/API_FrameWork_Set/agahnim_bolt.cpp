#include "stdafx.h"
#include "agahnim_bolt.h"


HRESULT agahnim_bolt::init()
{
	_isActive = false;
	_positionX = 0;
	_positionY = 0;
	_positionZ = 0;
	_equipAngle = 0;
	_speed = 2.0f;
	_damage = 0;
	_range = 1000.0f;
	_kind = EQUIPMENT_KIND_AGAHNIMBOLT;
	_img = new image;
	_img->init("images/enemy/agahnim_bolt.bmp", 300, 450, 2, 3, true, RGB(255, 0, 255));
	_isFlick = false;
	startTime = 0;
	boltTailTime = 0;

	_isPPX2Fill = false;
	_isPPX3Fill = false;
	_isPPX4Fill = false;
	_isPPX5Fill = false;

	PPX2Timer = 300;
	PPX3Timer = 300;
	PPX4Timer = 300;
	PPX5Timer = 300;
	return S_OK;
}

void agahnim_bolt::release()
{
}

void agahnim_bolt::update()
{
	_rc = RectMakeCenter(_positionX, _positionY, 20, 20);
	animation();
	Move();
	Collision();

	if (_isActive)
	{
		if (GetTickCount() - boltTailTime >= 100)
		{
			boltTailTime = GetTickCount();

			if (!_isPPX2Fill)
			{
				_postPositionX2 = _positionX;
				_postPositionY2 = _positionY;
				_isPPX2Fill = true;
			}
			else if (!_isPPX3Fill)
			{
				_postPositionX3 = _positionX;
				_postPositionY3 = _positionY;
				_isPPX3Fill = true;
			}
			else if (!_isPPX4Fill)
			{
				_postPositionX4 = _positionX;
				_postPositionY4 = _positionY;
				_isPPX4Fill = true;
			}
			else if (!_isPPX5Fill)
			{
				_postPositionX5 = _positionX;
				_postPositionY5 = _positionY;
				_isPPX5Fill = true;
			}

		}
	}


	if (_isFlick)
	{
		_speed = 8.0f;
	}
}

void agahnim_bolt::render(HDC hdc)
{
	if (_isPPX5Fill)
	{
		PPX5Timer -= 12;
		_img->frameRender(hdc, _postPositionX5 - _img->getFrameWidth() / 2, _postPositionY5 - _img->getFrameHeight() / 2, _img->getFrameX(), PPX5Timer / 100);
		if (PPX5Timer <= 0)
		{
			_isPPX5Fill = false;
			PPX5Timer = 300;
		}
	}
	if (_isPPX4Fill)
	{
		PPX4Timer -= 12;
		_img->frameRender(hdc, _postPositionX4 - _img->getFrameWidth() / 2, _postPositionY4 - _img->getFrameHeight() / 2, _img->getFrameX(), PPX4Timer / 100);
		if (PPX4Timer <= 0)
		{
			_isPPX4Fill = false;
			PPX4Timer = 300;
		}
	}
	if (_isPPX3Fill)
	{
		PPX3Timer -= 12;
		_img->frameRender(hdc, _postPositionX3 - _img->getFrameWidth() / 2, _postPositionY3 - _img->getFrameHeight() / 2, _img->getFrameX(), PPX3Timer / 100);
		if (PPX3Timer <= 0)
		{
			_isPPX3Fill = false;
			PPX3Timer = 300;
		}
	}
	if (_isPPX2Fill)
	{
		PPX2Timer -= 12;
		_img->frameRender(hdc, _postPositionX2 - _img->getFrameWidth() / 2, _postPositionY2 - _img->getFrameHeight() / 2, _img->getFrameX(), PPX2Timer / 100);
		if (PPX2Timer <= 0)
		{
			_isPPX2Fill = false;
			PPX2Timer = 300;
		}
	}


	_img->frameRender(hdc, _positionX - _img->getFrameWidth() / 2, _positionY - _img->getFrameHeight() / 2, _img->getFrameX(), 2);

}

void agahnim_bolt::Move()
{
	_positionX += cosf(_equipAngle) * _speed;
	_positionY += -sinf(_equipAngle) * _speed;
}

void agahnim_bolt::Collision()
{
	RECT abTemp;


	vector<enemySet*> enemyList = ENEMYMANAGER->getEnemyList();
	for (int i = 0; i < enemyList.size(); i++)
	{
		RECT rTemp;
		if (IntersectRect(&abTemp, &_rc, &enemyList[i]->getRect()) && _isFlick)
		{
			_isFlick = false;

			_isPPX2Fill = false;
			_isPPX3Fill = false;
			_isPPX4Fill = false;
			_isPPX5Fill = false;

			_isActive = false;


			enemyList[i]->setHp(enemyList[i]->getHp() - 1);
			enemyList[i]->setIsKnockBack(true);

			if (enemyList[i]->getHp() == 0)
			{
				ITEMMANAGER->dropRandItem(enemyList[i]->getPositionX(), enemyList[i]->getPositionY());
			}

		}
	}
	curTileMap = SCENEMANAGER->getCurrentTileMap();
	if (curTileMap.getMapObj(_rc.left / 24, _rc.top / 24) == OBJ_BLOCK ||
		curTileMap.getMapObj(_rc.right / 24, _rc.top / 24) == OBJ_BLOCK ||
		curTileMap.getMapObj(_rc.left / 24, _rc.bottom / 24) == OBJ_BLOCK ||
		curTileMap.getMapObj(_rc.right / 24, _rc.bottom / 24) == OBJ_BLOCK)
	{
		_isFlick = false;

		_isPPX2Fill = false;
		_isPPX3Fill = false;
		_isPPX4Fill = false;
		_isPPX5Fill = false;

		_isActive = false;


	}
}

void agahnim_bolt::animation()
{
	_boltFrameCount++;
	_img->setFrameY(2);
	if (_boltFrameCount % 2 == 0)
	{
		_boltFrameCount = 0;
		_img->setFrameX(_boltFrameIndex++);
		if (_boltFrameIndex > _img->getMaxFrameX())
		{
			_boltFrameIndex = 0;
		}
	}
}

agahnim_bolt::agahnim_bolt()
{
}


agahnim_bolt::~agahnim_bolt()
{
}
