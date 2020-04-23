#include "stdafx.h"
#include "agahnim_bubble.h"


agahnim_bubble::agahnim_bubble()
{
}


agahnim_bubble::~agahnim_bubble()
{
}

HRESULT agahnim_bubble::init()
{
	_isActive = false;
	_positionX = 0;
	_positionY = 0;
	_positionZ = 0;
	_equipAngle = 0;
	_speed = 5.0f;
	_damage = 0;
	_range = 1000.0f;
	_kind = EQUIPMENT_KIND_AGAHNIMBUBBLE;
	_img = new image;
	_img->init("images/enemy/agahnim_bubble.bmp", 300, 150, 2, 1, true, RGB(255, 0, 255));
	_isFlick = false;
	startTime = 0;

	_isBubblePop = false;

	bubbleNum = 1;

	return S_OK;
}

void agahnim_bubble::release()
{
}

void agahnim_bubble::update()
{
	if (!_isBubblePop)
	{
		_rc = RectMakeCenter(_positionX, _positionY, 20, 20);
		Move();
		animation();
		Collision();
	}
	else if (_isBubblePop)
	{
		_rc = RectMakeCenter(_positionX, _positionY, 0, 0);
		pop();
	}

}

void agahnim_bubble::render(HDC hdc)
{
	if (!_isBubblePop)
	{
		_img->frameRender(hdc, _positionX - _img->getFrameWidth() / 2, _positionY - _img->getFrameHeight() / 2, _img->getFrameX(), 0);
	}

}

void agahnim_bubble::Move()
{
	_positionX += cosf(_equipAngle) * _speed;
	_positionY += -sinf(_equipAngle) * _speed;
}

void agahnim_bubble::Collision()
{
	vector<equipment*> equipmentList = EQUIPMENTMANAGER->getEquipmentVector();
	curTileMap = SCENEMANAGER->getCurrentTileMap();
	if (curTileMap.getMapObj(_rc.left / 24, _rc.top / 24) == OBJ_BLOCK ||
		curTileMap.getMapObj(_rc.right / 24, _rc.top / 24) == OBJ_BLOCK ||
		curTileMap.getMapObj(_rc.left / 24, _rc.bottom / 24) == OBJ_BLOCK ||
		curTileMap.getMapObj(_rc.right / 24, _rc.bottom / 24) == OBJ_BLOCK)
	{
		_isBubblePop = true;
	}
}

void agahnim_bubble::animation()
{
	_bubbleFrameCount++;
	_img->setFrameY(2);
	if (_bubbleFrameCount % 4 == 0)
	{
		_bubbleFrameCount = 0;
		_img->setFrameX(_bubbleFrameIndex++);
		if (_bubbleFrameIndex > _img->getMaxFrameX())
		{
			_bubbleFrameIndex = 0;
		}
	}
}

void agahnim_bubble::pop()
{

	vector<equipment*> equipVector = EQUIPMENTMANAGER->getEquipmentVector();

	for (int i = 0; i < equipVector.size(); i++)
	{
		if (equipVector[i]->getisActive()) continue;
		if (equipVector[i]->getEquipKind() != EQUIPMENT_KIND_AGAHNIMSMALLBUBBLE) continue;
		equipVector[i]->setPositionX(_positionX);
		equipVector[i]->setPositionY(_positionY);
		equipVector[i]->setEquipAngle((PI / 4) * bubbleNum);
		if (bubbleNum == 3)
		{
			bubbleNum += 2;
		}
		else
		{
			bubbleNum += 1;
		}
		equipVector[i]->setisActive(true);
	}
	bubbleNum = 1;
	_isBubblePop = false;
	_isActive = false;

}


