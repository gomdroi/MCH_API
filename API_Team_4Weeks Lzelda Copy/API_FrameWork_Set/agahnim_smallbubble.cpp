#include "stdafx.h"
#include "agahnim_smallbubble.h"


agahnim_smallbubble::agahnim_smallbubble()
{
}


agahnim_smallbubble::~agahnim_smallbubble()
{
}

HRESULT agahnim_smallbubble::init()
{
	_isActive = false;
	_positionX = 0;
	_positionY = 0;
	_positionZ = 0;
	_equipAngle = 0;
	_speed = 6.0f;
	_damage = 0;
	_range = 1000.0f;
	_kind = EQUIPMENT_KIND_AGAHNIMSMALLBUBBLE;
	_img = new image;
	_img->init("images/enemy/agahnim_smallbubble.bmp", 300, 450, 2, 3, true, RGB(255, 0, 255));
	_isFlick = false;
	startTime = 0;
	bubbleTailTime = 0;

	_isPPX2Fill = false;
	_isPPX4Fill = false;
	_isPPX3Fill = false;

	PPX2Timer = 300;
	PPX3Timer = 300;
	PPX4Timer = 300;
	return S_OK;
}

void agahnim_smallbubble::release()
{
}

void agahnim_smallbubble::update()
{
	_rc = RectMakeCenter(_positionX, _positionY, 5, 5);
	Move();
	animation();
	Collision();

	if (_isActive)
	{
		if (GetTickCount() - bubbleTailTime >= 50)
		{
			bubbleTailTime = GetTickCount();

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
		}
	}
}

void agahnim_smallbubble::render(HDC hdc)
{

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

	if (_isDebug)
	{
		RectangleMakeCenter(hdc, _positionX, _positionY, 5, 5);
	}
}

void agahnim_smallbubble::Move()
{
	_positionX += cosf(_equipAngle) * _speed;
	_positionY += -sinf(_equipAngle) * _speed;
}

void agahnim_smallbubble::Collision()
{
	curTileMap = SCENEMANAGER->getCurrentTileMap();
	if (curTileMap.getMapObj(_rc.left / 24, _rc.top / 24) == OBJ_BLOCK ||
		curTileMap.getMapObj(_rc.right / 24, _rc.top / 24) == OBJ_BLOCK ||
		curTileMap.getMapObj(_rc.left / 24, _rc.bottom / 24) == OBJ_BLOCK ||
		curTileMap.getMapObj(_rc.right / 24, _rc.bottom / 24) == OBJ_BLOCK)
	{
		_isPPX2Fill = false;
		_isPPX3Fill = false;
		_isPPX4Fill = false;

		_isActive = false;
	}
}

void agahnim_smallbubble::animation()
{
	_bubbleFrameCount++;
	_img->setFrameY(2);
	if (_bubbleFrameCount % 2 == 0)
	{
		_bubbleFrameCount = 0;
		_img->setFrameX(_bubbleFrameIndex++);
		if (_bubbleFrameIndex > _img->getMaxFrameX())
		{
			_bubbleFrameIndex = 0;
		}
	}
}
