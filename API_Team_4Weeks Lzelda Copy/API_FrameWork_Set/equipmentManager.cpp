#include "stdafx.h"
#include "equipmentManager.h"
#include "equipment.h"


HRESULT equipmentManager::init()
{
	_equipVector.clear();
	//플레이어 화살
	ANIMATIONMANAGER->addDefAnimation("player_up_arrow", "player_up_arrow", 10, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_down_arrow", "player_down_arrow", 10, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_left_arrow", "player_left_arrow", 10, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_right_arrow", "player_right_arrow", 10, false, false);

	ANIMATIONMANAGER->addDefAnimation("player_throw_boomerang", "player_throw_boomerang", 10, false, true);

	ANIMATIONMANAGER->addDefAnimation("player_boom", "player_boom", 5, false, true);
	ANIMATIONMANAGER->addDefAnimation("player_boom_explosion", "player_boom_explosion", 10, false, false);

	for (int i = 0; i < 30; i++)
	{
		equipment* arrowSet = new arrow;
		arrowSet->init();
		addEquipment(arrowSet);
	}

	equipment* boomerangSet = new boomerang;
	boomerangSet->init();
	addEquipment(boomerangSet);

	equipment* boomSet = new boom;
	boomSet->init();
	addEquipment(boomSet);

	equipment* interactionObjSet = new playerInteractionObject;
	interactionObjSet->init();
	addEquipment(interactionObjSet);

	equipment* agahnimBolt = new agahnim_bolt;
	agahnimBolt->init();
	addEquipment(agahnimBolt);

	equipment* agahnimBubble = new agahnim_bubble;
	agahnimBubble->init();
	addEquipment(agahnimBubble);

	for (int i = 0; i < 6; i++)
	{
		equipment* agahnimSmallbubble = new agahnim_smallbubble;
		agahnimSmallbubble->init();
		addEquipment(agahnimSmallbubble);
	}

	_isBoom = false;
	_isInteractionObject = false;		//상호작용 오브젝트를 가지고 있는지 여부
	 _isFireInteractionObject = false;	//상호작용 오브젝트를 발사하는지 여부


	return S_OK;
}

void equipmentManager::render(HDC hdc)
{
	for (int i = 0; i < _equipVector.size(); i++)
	{
		if (!_equipVector[i]->getisActive()) continue;
		_equipVector[i]->render(hdc);
	}
}

void equipmentManager::release()
{
	vector<equipment*>::iterator eIter = _equipVector.begin();
	for (eIter; eIter != _equipVector.end();)
	{
		SAFE_DELETE((*eIter));
		eIter = _equipVector.erase(eIter);
	}
}

void equipmentManager::update()
{
	for (int i = 0; i < _equipVector.size(); i++)
	{
		if (!_equipVector[i]->getisActive()) continue;

		if (_equipVector[i]->getEquipKind() == EQUIPMENT_KIND::EQUIPMENT_KIND_BOOM)
		{
			if (!_isBoom&&GetTickCount() - _startTime < 3000)
			{
				_startTime = GetTickCount();
				_equipVector[i]->aniStart();
				_isBoom = true;
			}
			else if (_isBoom&&GetTickCount() - _startTime > 3000)
			{
				_sound.effSoundPlay("폭탄");
				_equipVector[i]->aniStop();
				_equipVector[i]->setImage(IMAGEMANAGER->findImage("player_boom_explosion"));
				_equipVector[i]->setAnimation(ANIMATIONMANAGER->findAnimation("player_boom_explosion"));
				_equipVector[i]->aniStart();
				_equipVector[i]->setRect(RectMakeCenter(_equipVector[i]->getPositionX()+30, _equipVector[i]->getPositionY()+30, 135, 135));
				_isBoom = false;
			}

			if (!_isBoom&&_equipVector[i]->getAnimation()->getFrameIndex() == 6)
			{
				_equipVector[i]->setisActive(false);
			}
		}
		_equipVector[i]->update();

	}
}

void equipmentManager::FireEquipment(EQUIPMENT_KIND kind, POINT position, float angle, float speed, bool isPlayer)
{
	for (int i = 0; i < _equipVector.size(); i++)
	{
		if (_equipVector[i]->getisActive()) continue;
		if (_equipVector[i]->getEquipKind() != kind) continue;
		_equipVector[i]->setPositionX(position.x);
		_equipVector[i]->setPositionY(position.y);
		_equipVector[i]->setEquipAngle(angle);
		_equipVector[i]->setSpeed(speed);
		_equipVector[i]->setisActive(true);

		if (isPlayer)
		{
			if (_player.getUsedEquip() == USED_EQUIPMENT::USED_BOW)
			{
				_sound.effSoundPlay("화살");
				switch (_player.getDirState())
				{
				case DIRECTION_STATE::PLAYER_UP:
					_equipVector[i]->setImage(IMAGEMANAGER->findImage("player_up_arrow"));
					break;
				case DIRECTION_STATE::PLAYER_DOWN:
					_equipVector[i]->setImage(IMAGEMANAGER->findImage("player_down_arrow"));
					break;
				case DIRECTION_STATE::PLAYER_LEFT:
					_equipVector[i]->setImage(IMAGEMANAGER->findImage("player_left_arrow"));
					break;
				case DIRECTION_STATE::PLAYER_RIGHT:
					_equipVector[i]->setImage(IMAGEMANAGER->findImage("player_right_arrow"));
					break;
				}
				break;
			}
			if (_player.getUsedEquip() == USED_EQUIPMENT::USED_BOOMERANG)
			{
				_sound.effSoundPlay("부메랑");
				_equipVector[i]->setImage(IMAGEMANAGER->findImage("player_throw_boomerang"));
				_equipVector[i]->setAnimation(ANIMATIONMANAGER->findAnimation("player_throw_boomerang"));
				_equipVector[i]->aniStart();
				break;
			}
			if (_player.getUsedEquip() == USED_EQUIPMENT::USED_BOOM)
			{
				_startTime = GetTickCount();
				_equipVector[i]->setImage(IMAGEMANAGER->findImage("player_boom"));
				_equipVector[i]->setAnimation(ANIMATIONMANAGER->findAnimation("player_boom"));
				break;
			}
		}
	}
}

equipmentManager::equipmentManager()
{
}


equipmentManager::~equipmentManager()
{
}

void equipmentManager::setIsInteractionObject(bool isInteractionObject, POINT position, INTERACTIONOBJECT obj)
{
	_isInteractionObject = isInteractionObject;
	for (int i = 0; i < _equipVector.size(); i++)
	{
		if (_equipVector[i]->getEquipKind() == EQUIPMENT_KIND_INTERACTIONOBJECT)
		{
			_equipVector[i]->setisActive(true);
			_equipVector[i]->setPositionX(position.x);
			_equipVector[i]->setPositionY(position.y-50);
			
			if (obj == INTERACTIONOBJ_GRASS)
			{
				_equipVector[i]->setImage(IMAGEMANAGER->findImage("maptool_grass"));
				_equipVector[i]->setInterectObjKind(INTERACTIONOBJ_GRASS);
			}
			else if (obj == INTERACTIONOBJ_SMALLROCK)
			{
				_equipVector[i]->setImage(IMAGEMANAGER->findImage("maptool_smallrock"));
				_equipVector[i]->setInterectObjKind(INTERACTIONOBJ_SMALLROCK);
			}
			else if (obj == INTERACTIONOBJ_JAR)
			{
				_equipVector[i]->setImage(IMAGEMANAGER->findImage("maptool_jar"));
				_equipVector[i]->setInterectObjKind(INTERACTIONOBJ_JAR);
			}
			break;
		}
	}
}

void equipmentManager::setIsFireInteractionObject(bool isFireInteractionObject, float angle)
{
	for (int i = 0; i < _equipVector.size(); i++)
	{
		if (_equipVector[i]->getEquipKind() == EQUIPMENT_KIND_INTERACTIONOBJECT)
		{
			_equipVector[i]->setEquipAngle(angle);
		}
	}
	_isFireInteractionObject = isFireInteractionObject;

	EQUIPMENTMANAGER->setIsInteractionObject(false);
}

void equipmentManager::isActiveOff()
{
	for (int i = 0; i < _equipVector.size(); i++)
	{
		_equipVector[i]->setisActive(false);
		EQUIPMENTMANAGER->setIsInteractionObject(false);
		EQUIPMENTMANAGER->setIsFireInteractionObject(false);
		_player.setIsLift(false);
	}
}
