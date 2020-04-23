#include "stdafx.h"
#include "player.h"

player::player()
{
}

player::~player()
{
}

HRESULT player::init(const char * imageName, POINT position)
{
	character::init(imageName, position);
	EQUIPMENTMANAGER->init();
	//애니메이션 초기화
	playerAnimAdd();
	//크기 초기화
	_width = 45;
	_height = 45;

	//스피드
	_speed = 5.0f;

	//게임 내 스텟 , 재화
	_maxHP = 10.0;
	_curHP = _maxHP;
	_money = 0;
	_arrow = 0;
	_boom = 0;
	//
	_angle = 0;
	_knockbackForce = 150;
	//
	_playerAlphaTime = 0;
	_playerAlphaValue = 255;
	//
	nowAngle = 0;
	//
	_positionX = position.x;
	_positionY = position.y;
	_positionCenterX = position.x + 25;
	_positionCenterY = position.y + 30;
	//플레이어방향 , 애니메이션 초기화
	_playerDirection = DIRECTION_STATE::PLAYER_DOWN; // 플레이어방향
	_prevAnimationState = _curAnimationState; // 플레이어 이전 애니메이션
	_img = IMAGEMANAGER->findImage("player_left");
	_ani = ANIMATIONMANAGER->findAnimation("player_left");

	//캐릭터상태
	_isHit = false;		// 캐릭터 피격중인가?
	_isAlpha = false;	// 캐릭터 알파
	_isDie = false;		// 캐릭터 죽었나?
	_isAttack = false;  // 캐릭터 공격중인가?
	_isArrow = false;		// 활쏘는 중인가?
	_isBoomerang = false;   // 부메랑던지는중인가?
	_isLift = false;
	_isStay = true;		// 캐릭터가 안움직이는가?
	_startTime = 0;
	//플레이어 랙트
	_Rc = RectMakeCenter(_positionCenterX, _positionCenterY, _width, _height);
	//															50     ,   50
	_camera = new camera;
	_camera->init();
	return S_OK;
}

void player::release()
{
	EQUIPMENTMANAGER->release();
}

void player::update(int mapSizeX, int mapSizeY, tileMap *nowTileMap)
{
	ending();

	if (_curHP > _maxHP) { _curHP = _maxHP; }

	if (_curHP < 4 && !_isHpLow)
	{
		_sound.effSoundPlay("체력없음");
		_isHpLow = true;
	}
	else if (_curHP >= 4 && _isHpLow)
	{
		_sound.effSoundOff("체력없음");
		_isHpLow = false;
	}
	else if (_curHP <= 0)
	{
		_sound.effSoundOff("체력없음");
		_isHpLow = true;
		ENEMYMANAGER->release();
		EQUIPMENTMANAGER->isActiveOff();
		SCENEMANAGER->changeScene("endingscene");
	}

	_positionCenterX = _positionX + 25;
	_positionCenterY = _positionY + 30;

	playerAttack();
	playerEquipment();
	playerMove(mapSizeX, mapSizeY, *nowTileMap);
	playerAnimMove();
	playerAnimCheck();
	playerInteactionObject(nowTileMap);
	Collision();
	EQUIPMENTMANAGER->update();
}
void player::playerAnimAdd()
{
	//애니메이션
	//플레이어 무브 애니메이션
	ANIMATIONMANAGER->addDefAnimation("player_left", "player_left", 8, false, true);
	ANIMATIONMANAGER->addDefAnimation("player_right", "player_right", 8, true, true);
	ANIMATIONMANAGER->addDefAnimation("player_up", "player_up", 8, false, true);
	ANIMATIONMANAGER->addDefAnimation("player_down", "player_down", 8, false, true);
	//플레이어 공격 애니메이션
	ANIMATIONMANAGER->addDefAnimation("player_down_attack", "player_down_attack", 10, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_right_attack", "player_right_attack", 10, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_left_attack", "player_left_attack", 10, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_up_attack", "player_up_attack", 8, false, false);
	//플레이어 활 애니메이션
	ANIMATIONMANAGER->addDefAnimation("player_up_bow", "player_up_bow", 5, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_down_bow", "player_down_bow", 5, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_left_bow", "player_left_bow", 5, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_right_bow", "player_right_bow", 5, false, false);
	//플레이어 부메랑 던지는 모션 애니메이션
	ANIMATIONMANAGER->addDefAnimation("player_up_boomerang", "player_up_boomerang", 2, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_down_boomerang", "player_down_boomerang", 2, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_left_boomerang", "player_left_boomerang", 2, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_right_boomerang", "player_right_boomerang", 2, false, false);

	//플레이어 물체들고 움직임 애니메이션
	ANIMATIONMANAGER->addDefAnimation("player_up_move_lift", "player_up_move_lift", 5, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_down_move_lift", "player_down_move_lift", 5, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_left_move_lift", "player_left_move_lift", 5, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_right_move_lift", "player_right_move_lift", 5, false, false);

	//플레이어 물체 들기 애니메이션
	ANIMATIONMANAGER->addDefAnimation("player_up_lift", "player_up_lift", 5, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_down_lift", "player_down_lift", 5, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_left_lift", "player_left_lift", 5, false, false);
	ANIMATIONMANAGER->addDefAnimation("player_right_lift", "player_right_lift", 5, false, false);

}
void player::playerAnimCheck()
{
	if (KEYMANAGER->isOnceKeyUp('A') || KEYMANAGER->isOnceKeyUp('D'))
	{
		_isStay = true;
	}

	if (KEYMANAGER->isOnceKeyUp(VK_DOWN) && !_isAttack && !_isArrow && !_isBoomerang) // 키보드 뗐을 때
	{
		if (EQUIPMENTMANAGER->getIsInteractionObject())
		{
			ANIMATIONMANAGER->stop("player_down_move_lift"); // 애니메이션 정지
			_ani->setFrameIndex(0); //기본 모션으로 설정
		}
		else
		{
			ANIMATIONMANAGER->stop("player_down"); // 애니메이션 정지
		}
		_isStay = true;
		_playerDirection = DIRECTION_STATE::PLAYER_DOWN;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_UP) && !_isAttack && !_isArrow && !_isBoomerang)
	{
		if (EQUIPMENTMANAGER->getIsInteractionObject())
		{
			ANIMATIONMANAGER->stop("player_up_move_lift"); // 애니메이션 정지
			_ani->setFrameIndex(0); //기본 모션으로 설정
		}
		else
		{
			ANIMATIONMANAGER->stop("player_up");
		}
		_isStay = true;
		_playerDirection = DIRECTION_STATE::PLAYER_UP;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && !_isAttack && !_isArrow && !_isBoomerang)
	{
		if (EQUIPMENTMANAGER->getIsInteractionObject())
		{
			ANIMATIONMANAGER->stop("player_left_move_lift"); // 애니메이션 정지
			_ani->setFrameIndex(0); //기본 모션으로 설정
		}
		else
		{
			ANIMATIONMANAGER->stop("player_left");
		}
		_isStay = true;
		_playerDirection = DIRECTION_STATE::PLAYER_LEFT;
	}
	if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && !_isAttack && !_isArrow && !_isBoomerang)
	{
		if (EQUIPMENTMANAGER->getIsInteractionObject())
		{
			ANIMATIONMANAGER->stop("player_right_move_lift"); // 애니메이션 정지
			_ani->setFrameIndex(0); //기본 모션으로 설정
		}
		else
		{
			ANIMATIONMANAGER->stop("player_right");
		}
		_isStay = true;
		_playerDirection = DIRECTION_STATE::PLAYER_RIGHT;
	}

	if (_isStay)
	{
		switch (_playerDirection)
		{
		case DIRECTION_STATE::PLAYER_DOWN: // 아래
			ANIMATIONMANAGER->stop("player_down");
			break;
		case DIRECTION_STATE::PLAYER_UP: //위
			ANIMATIONMANAGER->stop("player_up");
			break;
		case DIRECTION_STATE::PLAYER_LEFT://왼쪽
			ANIMATIONMANAGER->stop("player_left");
			break;
		case DIRECTION_STATE::PLAYER_RIGHT://오른쪽
			ANIMATIONMANAGER->stop("player_right");
			break;
		}
	}
}
void player::playerMove(int mapSizeX, int mapSizeY, tileMap nowTileMap)
{
	if (KEYMANAGER->isStayKeyDown(VK_DOWN) && !_isAttack && !_isArrow && !_isBoomerang)
	{
		POINT xy = { _positionX,_positionY + _speed };

		//캐릭터 이동 성공시
		if (obstacle(xy, PLAYER_DOWN, mapSizeX, mapSizeY, nowTileMap))
		{
			if (_isLadder)
			{
				setPlayerMove(0, 0, mapSizeX, mapSizeY, 0, _speed - 3);
			}
			else
			{
				setPlayerMove(0, 0, mapSizeX, mapSizeY, 0, _speed);
			}
		}
		else
			//실패시
		{
			if (!_isLadder)
			{
				xy = PointMake(_positionX - 24, _positionY + _speed);
				if (obstacle(xy, PLAYER_DOWN, mapSizeX, mapSizeY, nowTileMap))
				{
					setPlayerMove(0, 0, mapSizeX, mapSizeY, -_speed, 0);
				}
				else
				{
					xy = PointMake(_positionX + 24, _positionY + _speed);
					if (obstacle(xy, PLAYER_DOWN, mapSizeX, mapSizeY, nowTileMap))
					{
						setPlayerMove(0, 0, mapSizeX, mapSizeY, _speed, 0);
					}
				}
			}
		}

		//이미지와 프레임 초기화
		if (!_isDie&&_isStay == true)
		{
			if (EQUIPMENTMANAGER->getIsInteractionObject())
			{
				_img = IMAGEMANAGER->findImage("player_down_move_lift");
				_ani = ANIMATIONMANAGER->findAnimation("player_down_move_lift");
				ANIMATIONMANAGER->start("player_down_move_lift");
			}
			else
			{
				_img = IMAGEMANAGER->findImage("player_down");
				_ani = ANIMATIONMANAGER->findAnimation("player_down");
				ANIMATIONMANAGER->start("player_down");
			}
			_isStay = false;
			_playerDirection = PLAYER_DOWN;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP) && !_isAttack && !_isArrow && !_isBoomerang)
	{
		POINT xy = { _positionX,_positionY - _speed };

		//캐릭터 이동 성공시
		if (obstacle(xy, PLAYER_UP, mapSizeX, mapSizeY, nowTileMap))
		{
			if (_isLadder)
			{
				setPlayerMove(0, 0, mapSizeX, mapSizeY, 0, -(_speed - 3));
			}
			else
			{
				setPlayerMove(0, 0, mapSizeX, mapSizeY, 0, -_speed);
			}
		}
		else
			//실패시
		{
			if (!_isLadder)
			{
				xy = PointMake(_positionX - 24, _positionY - _speed);
				if (obstacle(xy, PLAYER_DOWN, mapSizeX, mapSizeY, nowTileMap))
				{
					setPlayerMove(0, 0, mapSizeX, mapSizeY, -_speed, 0);
				}
				else
				{
					xy = PointMake(_positionX + 24, _positionY - _speed);
					if (obstacle(xy, PLAYER_DOWN, mapSizeX, mapSizeY, nowTileMap))
					{
						setPlayerMove(0, 0, mapSizeX, mapSizeY, _speed, 0);
					}
				}
			}
		}

		//이미지와 프레임 초기화
		if (!_isDie&&_isStay == true) // 살아있는상태이고 플레이어가 움직이지 않는상태 일때
		{
			if (EQUIPMENTMANAGER->getIsInteractionObject())
			{
				_img = IMAGEMANAGER->findImage("player_up_move_lift");
				_ani = ANIMATIONMANAGER->findAnimation("player_up_move_lift");
				ANIMATIONMANAGER->start("player_up_move_lift");
			}
			else
			{
				_img = IMAGEMANAGER->findImage("player_up");
				_ani = ANIMATIONMANAGER->findAnimation("player_up");
				ANIMATIONMANAGER->start("player_up");
			}
			_isStay = false;
			_playerDirection = PLAYER_UP;
		}

	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && !_isAttack && !_isArrow && !_isBoomerang)
	{
		POINT xy = { _positionX - _speed, _positionY };

		//캐릭터 이동 성공시
		if (obstacle(xy, PLAYER_LEFT, mapSizeX, mapSizeY, nowTileMap))
		{
			setPlayerMove(0, 0, mapSizeX, mapSizeY, -_speed, 0);
		}
		else
			//실패시
		{
			if (!_isLadder)
			{
				xy = PointMake(_positionX - _speed, _positionY - 24);
				if (obstacle(xy, PLAYER_DOWN, mapSizeX, mapSizeY, nowTileMap))
				{
					setPlayerMove(0, 0, mapSizeX, mapSizeY, 0, -_speed);
				}
				else
				{
					xy = PointMake(_positionX - _speed, _positionY + 24);
					if (obstacle(xy, PLAYER_DOWN, mapSizeX, mapSizeY, nowTileMap))
					{
						setPlayerMove(0, 0, mapSizeX, mapSizeY, 0, _speed);
					}
				}
			}
		}

		//이미지와 프레임 초기화
		if (!_isDie&&_isStay == true) //
		{
			if (EQUIPMENTMANAGER->getIsInteractionObject())
			{
				_img = IMAGEMANAGER->findImage("player_left_move_lift");
				_ani = ANIMATIONMANAGER->findAnimation("player_left_move_lift");
				ANIMATIONMANAGER->start("player_left_move_lift");
			}
			else
			{
				_img = IMAGEMANAGER->findImage("player_left");
				_ani = ANIMATIONMANAGER->findAnimation("player_left");
				ANIMATIONMANAGER->start("player_left");
			}
			_playerDirection = PLAYER_LEFT;
			_isStay = false;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && !_isAttack && !_isArrow && !_isBoomerang)
	{
		POINT xy = { _positionX + _speed, _positionY };
		//캐릭터 이동 성공시
		if (obstacle(xy, PLAYER_RIGHT, mapSizeX, mapSizeY, nowTileMap))
		{
			setPlayerMove(0, 0, mapSizeX, mapSizeY, _speed, 0);
		}
		else
			//실패시
		{
			if (!_isLadder)
			{
				xy = PointMake(_positionX + _speed, _positionY - 24);
				if (obstacle(xy, PLAYER_DOWN, mapSizeX, mapSizeY, nowTileMap))
				{
					setPlayerMove(0, 0, mapSizeX, mapSizeY, 0, -_speed);
				}
				else
				{
					xy = PointMake(_positionX + _speed, _positionY + 24);
					if (obstacle(xy, PLAYER_DOWN, mapSizeX, mapSizeY, nowTileMap))
					{
						setPlayerMove(0, 0, mapSizeX, mapSizeY, 0, _speed);
					}
				}
			}
		}

		if (!_isDie&&_isStay == true)
		{
			if (EQUIPMENTMANAGER->getIsInteractionObject())
			{
				_img = IMAGEMANAGER->findImage("player_right_move_lift");
				_ani = ANIMATIONMANAGER->findAnimation("player_right_move_lift");
				ANIMATIONMANAGER->start("player_right_move_lift");
			}
			else
			{
				_img = IMAGEMANAGER->findImage("player_right");
				_ani = ANIMATIONMANAGER->findAnimation("player_right");
				ANIMATIONMANAGER->start("player_right");
			}
			_playerDirection = PLAYER_RIGHT;
			_isStay = false;
		}
	}
}
void player::playerAnimMove()
{
	if (_ani->getFrameNum() - 1 == _ani->getFrameIndex())
	{
		if (_playerDirection == DIRECTION_STATE::PLAYER_DOWN)
		{
			_ani->setFrameIndex(0);
			_isAttack = false;
			_isArrow = false;
			_isBoomerang = false;
			_isStay = true;
			if (EQUIPMENTMANAGER->getIsInteractionObject())
			{
				_img = IMAGEMANAGER->findImage("player_down_move_lift");
				_ani = ANIMATIONMANAGER->findAnimation("player_down_move_lift");
			}
			else
			{
				_img = IMAGEMANAGER->findImage("player_down");
				_ani = ANIMATIONMANAGER->findAnimation("player_down");
			}
		}
		if (_playerDirection == DIRECTION_STATE::PLAYER_UP)
		{
			_ani->setFrameIndex(0);
			_isAttack = false;
			_isArrow = false;
			_isBoomerang = false;
			_isStay = true;
			if (EQUIPMENTMANAGER->getIsInteractionObject())
			{
				_img = IMAGEMANAGER->findImage("player_up_move_lift");
				_ani = ANIMATIONMANAGER->findAnimation("player_up_move_lift");
			}
			else
			{
				_img = IMAGEMANAGER->findImage("player_up");
				_ani = ANIMATIONMANAGER->findAnimation("player_up");
			}
		}
		if (_playerDirection == DIRECTION_STATE::PLAYER_LEFT)
		{
			_ani->setFrameIndex(0);
			_isAttack = false;
			_isArrow = false;
			_isBoomerang = false;
			_isStay = true;

			if (EQUIPMENTMANAGER->getIsInteractionObject())
			{
				_img = IMAGEMANAGER->findImage("player_left_move_lift");
				_ani = ANIMATIONMANAGER->findAnimation("player_left_move_lift");
			}
			else
			{
				_img = IMAGEMANAGER->findImage("player_left");
				_ani = ANIMATIONMANAGER->findAnimation("player_left");
			}
		}
		if (_playerDirection == DIRECTION_STATE::PLAYER_RIGHT)
		{
			_ani->setFrameIndex(0);
			_isAttack = false;
			_isArrow = false;
			_isBoomerang = false;
			_isStay = true;
			if (EQUIPMENTMANAGER->getIsInteractionObject())
			{
				_img = IMAGEMANAGER->findImage("player_right_move_lift");
				_ani = ANIMATIONMANAGER->findAnimation("player_right_move_lift");
			}
			else
			{
				_img = IMAGEMANAGER->findImage("player_right");
				_ani = ANIMATIONMANAGER->findAnimation("player_right");
			}
		}
	}
}
void player::playerEquipment()
{
	if (KEYMANAGER->isOnceKeyDown('D') && !_isAttack)
	{
		if (INVENTORYMANAGER->getNowItem() == BOW)
		{
			if (INVENTORYMANAGER->getArrow() > 0)
			{
				INVENTORYMANAGER->setArrow(INVENTORYMANAGER->getArrow() - 1);

				_ani->setFrameIndex(0); // 애니메이션 프레임을 0으로 초기화시킨다
				_isArrow = true;
				_usedEquipment = USED_EQUIPMENT::USED_BOW;
				switch (_playerDirection)
				{
				case DIRECTION_STATE::PLAYER_DOWN: // 아래
					_img = IMAGEMANAGER->findImage("player_down_bow");
					_ani = ANIMATIONMANAGER->findAnimation("player_down_bow");
					ANIMATIONMANAGER->start("player_down_bow");
					EQUIPMENTMANAGER->FireEquipment(EQUIPMENT_KIND_ARROW, PointMake(_positionX, _positionY), -PI_2, _speed, true);
					break;
				case DIRECTION_STATE::PLAYER_UP: //위
					_img = IMAGEMANAGER->findImage("player_up_bow");
					_ani = ANIMATIONMANAGER->findAnimation("player_up_bow");
					ANIMATIONMANAGER->start("player_up_bow");
					EQUIPMENTMANAGER->FireEquipment(EQUIPMENT_KIND_ARROW, PointMake(_positionX, _positionY), PI_2, _speed, true);
					break;
				case DIRECTION_STATE::PLAYER_LEFT://왼쪽
					_img = IMAGEMANAGER->findImage("player_left_bow");
					_ani = ANIMATIONMANAGER->findAnimation("player_left_bow");
					ANIMATIONMANAGER->start("player_left_bow");
					EQUIPMENTMANAGER->FireEquipment(EQUIPMENT_KIND_ARROW, PointMake(_positionX, _positionY), PI, _speed, true);
					break;
				case DIRECTION_STATE::PLAYER_RIGHT://오른쪽
					_img = IMAGEMANAGER->findImage("player_right_bow");
					_ani = ANIMATIONMANAGER->findAnimation("player_right_bow");
					ANIMATIONMANAGER->start("player_right_bow");
					EQUIPMENTMANAGER->FireEquipment(EQUIPMENT_KIND_ARROW, PointMake(_positionX, _positionY), PI2, _speed, true);
					break;
				}
			}
		}//BOW끝

		if (INVENTORYMANAGER->getNowItem() == BOOMERANG)
		{
			_ani->setFrameIndex(0); // 애니메이션 프레임을 0으로 초기화시킨다
			_isBoomerang = true;
			_usedEquipment = USED_EQUIPMENT::USED_BOOMERANG;
			if (_usedEquipment == USED_EQUIPMENT::USED_BOOMERANG)
			{
				switch (_playerDirection)
				{
				case DIRECTION_STATE::PLAYER_DOWN: // 아래
					_img = IMAGEMANAGER->findImage("player_down_boomerang");
					_ani = ANIMATIONMANAGER->findAnimation("player_down_boomerang");
					ANIMATIONMANAGER->start("player_down_boomerang");
					EQUIPMENTMANAGER->FireEquipment(EQUIPMENT_KIND_BOOMERANG, PointMake(_positionX, _positionY), -PI_2, _speed, true);
					break;
				case DIRECTION_STATE::PLAYER_UP: //위
					_img = IMAGEMANAGER->findImage("player_up_boomerang");
					_ani = ANIMATIONMANAGER->findAnimation("player_up_boomerang");
					ANIMATIONMANAGER->start("player_up_boomerang");
					EQUIPMENTMANAGER->FireEquipment(EQUIPMENT_KIND_BOOMERANG, PointMake(_positionX, _positionY), PI_2, _speed, true);
					break;
				case DIRECTION_STATE::PLAYER_LEFT://왼쪽
					_img = IMAGEMANAGER->findImage("player_left_boomerang");
					_ani = ANIMATIONMANAGER->findAnimation("player_left_boomerang");
					ANIMATIONMANAGER->start("player_left_boomerang");
					EQUIPMENTMANAGER->FireEquipment(EQUIPMENT_KIND_BOOMERANG, PointMake(_positionX, _positionY), PI, _speed, true);
					break;
				case DIRECTION_STATE::PLAYER_RIGHT://오른쪽
					_img = IMAGEMANAGER->findImage("player_right_boomerang");
					_ani = ANIMATIONMANAGER->findAnimation("player_right_boomerang");
					ANIMATIONMANAGER->start("player_right_boomerang");
					EQUIPMENTMANAGER->FireEquipment(EQUIPMENT_KIND_BOOMERANG, PointMake(_positionX, _positionY), PI2, _speed, true);
					break;
				}
			}
		}//부메랑끝

		if (INVENTORYMANAGER->getNowItem() == BOMB)
		{
			_ani->setFrameIndex(0); // 애니메이션 프레임을 0으로 초기화시킨다
			_usedEquipment = USED_EQUIPMENT::USED_BOOM;
			if (_usedEquipment == USED_EQUIPMENT::USED_BOOM)
			{
				if (!EQUIPMENTMANAGER->getIsBoom())
				{
					INVENTORYMANAGER->setBomb(INVENTORYMANAGER->getBomb() - 1);
					if (INVENTORYMANAGER->getBomb() <= 0)
					{
						INVENTORYMANAGER->setIsItem(BOMB, false);
						INVENTORYMANAGER->setNowItem(ITEMNULL);
					}
					EQUIPMENTMANAGER->FireEquipment(EQUIPMENT_KIND_BOOM, PointMake(_positionX, _positionY), NULL, NULL, true);
				}
			}
		}//폭탄끝

		if (INVENTORYMANAGER->getNowItem() == HPMAX)
		{
			_curHP = _maxHP;
			INVENTORYMANAGER->setIsItem(HPMAX, false);
			INVENTORYMANAGER->setNowItem(ITEMNULL);
		}

	}
	else if (EQUIPMENTMANAGER->getIsInteractionObject())
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && !_isAttack && !_isArrow && !_isBoomerang)
		{
			switch (_playerDirection)
			{
			case DIRECTION_STATE::PLAYER_DOWN: // 아래
				EQUIPMENTMANAGER->setIsFireInteractionObject(true, -PI_2);
				_img = IMAGEMANAGER->findImage("player_down");
				_ani = ANIMATIONMANAGER->findAnimation("player_down");
				break;
			case DIRECTION_STATE::PLAYER_UP: //위
				EQUIPMENTMANAGER->setIsFireInteractionObject(true, PI_2);
				_img = IMAGEMANAGER->findImage("player_up");
				_ani = ANIMATIONMANAGER->findAnimation("player_up");
				break;
			case DIRECTION_STATE::PLAYER_LEFT://왼쪽
				EQUIPMENTMANAGER->setIsFireInteractionObject(true, PI);
				_img = IMAGEMANAGER->findImage("player_left");
				_ani = ANIMATIONMANAGER->findAnimation("player_left");
				break;
			case DIRECTION_STATE::PLAYER_RIGHT://오른쪽
				EQUIPMENTMANAGER->setIsFireInteractionObject(true, PI2);
				_img = IMAGEMANAGER->findImage("player_right");
				_ani = ANIMATIONMANAGER->findAnimation("player_right");
				break;
			}

			_sound.effSoundPlay("던지기");
			_isLift = false;
		}
	}
}
void player::playerInteactionObject(tileMap *nowTileMap)
{
	//상호작용 오브젝트와 충돌처리를 합니다
	POINT setPoint = { _player.getPositionX() + 25, _player.getPositionY() + 25 };
	vector<tagInteractionObject> interObjTemp = nowTileMap->getInteractionObject();

	if (!EQUIPMENTMANAGER->getIsInteractionObject() && !EQUIPMENTMANAGER->getIsFireInteractionObject())
	{
		if ((KEYMANAGER->isStayKeyDown('A') || KEYMANAGER->isOnceKeyDown(VK_SPACE) && !_isAttack && !_isArrow && !_isBoomerang))
		{

			switch (_playerDirection)
			{
			case DIRECTION_STATE::PLAYER_DOWN: // 아래
				setPoint.y += (_height / 2 + TILESIZE / 2);
				break;
			case DIRECTION_STATE::PLAYER_UP: //위
				setPoint.y -= (_height / 2 + TILESIZE / 2);
				break;
			case DIRECTION_STATE::PLAYER_LEFT://왼쪽
				setPoint.x -= (_width / 2 + TILESIZE / 2);
				break;
			case DIRECTION_STATE::PLAYER_RIGHT://오른쪽
				setPoint.x += (_width / 2 + TILESIZE / 2);
				break;
			}

			//오브젝트 던지기
			for (int i = 0; i < interObjTemp.size(); i++)
			{

				if (interObjTemp[i].xPos*TILESIZE + interObjTemp[i].widthPos*TILESIZE< _camera->getCameraX() || interObjTemp[i].xPos*TILESIZE>_camera->getCameraX() + WINSIZEX)
				{
					continue;
				}
				else if (interObjTemp[i].yPos*TILESIZE + interObjTemp[i].heightPos*TILESIZE< _camera->getCameraY() || interObjTemp[i].yPos*TILESIZE>_camera->getCameraY() + WINSIZEY)
				{
					continue;
				}

				if (PtInRect(&interObjTemp[i]._rc, setPoint))
				{
					if (!(KEYMANAGER->isStayKeyDown('A')))
					{
						if (interObjTemp[i].obj == INTERACTIONOBJ_ROCKEDDOOR)
						{
							if (INVENTORYMANAGER->getKey() > 0)
							{
								INVENTORYMANAGER->setKey(INVENTORYMANAGER->getKey() - 1);
								nowTileMap->addInteractionObject(interObjTemp[i].xPos, interObjTemp[i].yPos, interObjTemp[i].widthPos, interObjTemp[i].heightPos,
									interObjTemp[i]._rc, INTERACTIONOBJ_OPENDOOR, IMAGEMANAGER->findImage("maptool_opendoor"), interObjTemp[i].imageNum + 1);

								//오브젝트 삭제
								RECT rectTemp = interObjTemp[i]._rc;

								nowTileMap->deleteInteractionObject(i);
								SCENEMANAGER->reDrawPosition(false, SCENEMANAGER->getNowFloor(), rectTemp.left, rectTemp.top, rectTemp.right - rectTemp.left, rectTemp.bottom - rectTemp.top, _camera->getCameraX(), _camera->getCameraY());
							}
						}
					}

					if (interObjTemp[i].obj == INTERACTIONOBJ_GRASS || interObjTemp[i].obj == INTERACTIONOBJ_SMALLROCK ||
						interObjTemp[i].obj == INTERACTIONOBJ_JAR || interObjTemp[i].obj == INTERACTIONOBJ_TREASUREBOX ||
						interObjTemp[i].obj == INTERACTIONOBJ_BIGTREASUREBOX)
					{
						if (interObjTemp[i].obj == INTERACTIONOBJ_GRASS && KEYMANAGER->isStayKeyDown('A'))
						{
							_sound.effSoundPlay("풀제거");
							EFFECTMANAGER->play("destory_grass_sprite", interObjTemp[i]._rc.left + (interObjTemp[i]._rc.right - interObjTemp[i]._rc.left) / 2, interObjTemp[i]._rc.top + (interObjTemp[i]._rc.bottom - interObjTemp[i]._rc.top) / 2);
							ITEMMANAGER->dropMoneyItem(interObjTemp[i]._rc.left + (interObjTemp[i]._rc.right - interObjTemp[i]._rc.left) / 2,
								interObjTemp[i]._rc.top + (interObjTemp[i]._rc.bottom - interObjTemp[i]._rc.top) / 2, 50);

							//오브젝트 삭제

							RECT rectTemp = interObjTemp[i]._rc;
							nowTileMap->deleteInteractionObject(i);
							SCENEMANAGER->reDrawPosition(false, SCENEMANAGER->getNowFloor(), rectTemp.left, rectTemp.top, rectTemp.right - rectTemp.left, rectTemp.bottom - rectTemp.top, _camera->getCameraX(), _camera->getCameraY());
						}

						if (!(KEYMANAGER->isStayKeyDown('A')))
						{
							if (interObjTemp[i].obj == INTERACTIONOBJ_GRASS || interObjTemp[i].obj == INTERACTIONOBJ_SMALLROCK ||
								interObjTemp[i].obj == INTERACTIONOBJ_JAR)
							{
								if (!_isAttack)
								{
									//오브젝트 들기
									EQUIPMENTMANAGER->setIsInteractionObject(true, PointMake(_positionX, _positionY), interObjTemp[i].obj);
									_sound.effSoundPlay("들기");
									_isLift = true;
								}
								//항아리 하트생성
								if (interObjTemp[i].obj == INTERACTIONOBJ_JAR)
								{
									ITEMMANAGER->dropRandItem(interObjTemp[i]._rc.left + (interObjTemp[i]._rc.right - interObjTemp[i]._rc.left) / 2,
										interObjTemp[i]._rc.top + (interObjTemp[i]._rc.bottom - interObjTemp[i]._rc.top) / 2);
								}
								//숲 돈생성
								if (interObjTemp[i].obj == INTERACTIONOBJ_GRASS)
								{
									ITEMMANAGER->dropMoneyItem(interObjTemp[i]._rc.left + (interObjTemp[i]._rc.right - interObjTemp[i]._rc.left) / 2,
										interObjTemp[i]._rc.top + (interObjTemp[i]._rc.bottom - interObjTemp[i]._rc.top) / 2, 50);
								}
							}

							//작은 보물상자 hpMax++아이템 생성
							if (interObjTemp[i].obj == INTERACTIONOBJ_TREASUREBOX)
							{
								ITEMMANAGER->dropMaxHp(interObjTemp[i]._rc.left + (interObjTemp[i]._rc.right - interObjTemp[i]._rc.left) / 2,
									interObjTemp[i]._rc.top + (interObjTemp[i]._rc.bottom - interObjTemp[i]._rc.top) / 2);
							}
							//큰 보물상자 녹색팬던트
							if (interObjTemp[i].obj == INTERACTIONOBJ_BIGTREASUREBOX)
							{
								ITEMMANAGER->dropPendent(interObjTemp[i]._rc.left + (interObjTemp[i]._rc.right - interObjTemp[i]._rc.left) / 2,
									interObjTemp[i]._rc.top + (interObjTemp[i]._rc.bottom - interObjTemp[i]._rc.top) / 2, GREENPENDENT);
							}

							//오브젝트 삭제

							RECT rectTemp = interObjTemp[i]._rc;
							nowTileMap->deleteInteractionObject(i);
							SCENEMANAGER->reDrawPosition(false, SCENEMANAGER->getNowFloor(), rectTemp.left, rectTemp.top, rectTemp.right - rectTemp.left, rectTemp.bottom - rectTemp.top, _camera->getCameraX(), _camera->getCameraY());
						}
					}

					if (!_isAttack)
					{
						switch (_playerDirection)
						{
						case DIRECTION_STATE::PLAYER_DOWN: // 아래
							_img = IMAGEMANAGER->findImage("player_down_lift");
							_ani = ANIMATIONMANAGER->findAnimation("player_down_lift");
							ANIMATIONMANAGER->start("player_down_lift");
							break;
						case DIRECTION_STATE::PLAYER_UP: //위
							_img = IMAGEMANAGER->findImage("player_up_lift");
							_ani = ANIMATIONMANAGER->findAnimation("player_up_lift");
							ANIMATIONMANAGER->start("player_up_lift");
							break;
						case DIRECTION_STATE::PLAYER_LEFT://왼쪽
							_img = IMAGEMANAGER->findImage("player_left_lift");
							_ani = ANIMATIONMANAGER->findAnimation("player_left_lift");
							ANIMATIONMANAGER->start("player_left_lift");
							break;
						case DIRECTION_STATE::PLAYER_RIGHT://오른쪽
							_img = IMAGEMANAGER->findImage("player_right_lift");
							_ani = ANIMATIONMANAGER->findAnimation("player_right_lift");
							ANIMATIONMANAGER->start("player_right_lift");
							break;
						}
					}

				}
			}
		}
	}
}
void player::Collision()
{
	//플레이어 공격 에너미 충돌처리
	vector<enemySet*> enemyList = ENEMYMANAGER->getEnemyList();
	vector<equipment*> equipmentList = EQUIPMENTMANAGER->getEquipmentVector();
	RECT rTemp;

	for (int i = 0; i < enemyList.size(); i++)
	{
		if (IntersectRect(&rTemp, &_atkRc, &enemyList[i]->getAtkRect())) // 플레이어 공격랙트와 에너미 공격렉트의충돌
		{
			if (strcmp(enemyList[i]->getName(), "worm") == 0)
			{
				_startTime = GetTickCount();
				_isHit = true;
				_isKnockBack = true;
				enemyList[i]->setIsBounce(true);
				break;
			}
		}if (IntersectRect(&rTemp, &_Rc, &enemyList[i]->getAtkRect())) // 플레이어 랙트와 에너미 공격렉트의충돌
		{
			if (!_isAlpha)
			{
				if (strcmp(enemyList[i]->getName(), "worm") == 0)
				{
					_startTime = GetTickCount();
					_isHit = true;
					_isKnockBack = true;
					_isAlpha = true;
					_curHP -= 1.0f;
				}
			}
		}
		if (IntersectRect(&rTemp, &_atkRc, &enemyList[i]->getRect())) // 플레이어 공격랙트와 에너미의충돌
		{
			if (enemyList[i]->getEnemyColor() == BOSS) continue;
			if (!enemyList[i]->getIsFrameImage())
			{
				enemyList[i]->setHp(enemyList[i]->getHp() - 1);
				enemyList[i]->setIsKnockBack(true);
			}
			if (enemyList[i]->getHp() == 0)
			{
				int rendom = RND->getInt(10);
				if (rendom == 5)
				{
					ITEMMANAGER->dropKey(enemyList[i]->getPositionX(), enemyList[i]->getPositionY());
				}
				else
				{
					ITEMMANAGER->dropRandItem(enemyList[i]->getPositionX(), enemyList[i]->getPositionY());
				}
			}

		}

		if (!_isAlpha)
		{
			if (IntersectRect(&rTemp, &_Rc, &enemyList[i]->getRect())) // 플레이어와 에너미의 충돌
			{
				nowAngle = UTIL::getAngle(_hitStartPositionX, _hitStartPositionY, enemyList[i]->getPositionX(), enemyList[i]->getPositionY());
				_startTime = GetTickCount();
				_isHit = true;
				_isKnockBack = true;
				_isAlpha = true;
				_curHP -= 1.0f;

			}

			RECT *rectLightningTemp = enemyList[i]->getLightningRect();
			for (int i = 0; i < 6; ++i)
			{
				if (IntersectRect(&rTemp, &_Rc, &rectLightningTemp[i]))
				{
					_startTime = GetTickCount();
					_isHit = true;
					_isKnockBack = true;
					_isAlpha = true;
					_curHP -= 4.0f;
				}
			}

			for (int i = 0; i < equipmentList.size(); i++)
			{
				RECT sTemp;
				if (equipmentList[i]->getEquipKind() != EQUIPMENT_KIND_AGAHNIMBUBBLE && equipmentList[i]->getEquipKind() != EQUIPMENT_KIND_AGAHNIMSMALLBUBBLE &&
					equipmentList[i]->getEquipKind() != EQUIPMENT_KIND_AGAHNIMBOLT && !equipmentList[i]->getisActive()) continue; // agahnimbolt가 아니면 아래구문을 건너뛰고 탈출함

				if (equipmentList[i]->getEquipKind() == EQUIPMENT_KIND_AGAHNIMBOLT && equipmentList[i]->getisActive())
				{
					if (IntersectRect(&sTemp, &_atkRc, &equipmentList[i]->getRect()))												//플레이어 공격랙트와 보스 에너지볼트 충돌
					{
						equipmentList[i]->setIsKnockBack(true);
						equipmentList[i]->setIsFlick(true);
						equipmentList[i]->setEquipAngle(UTIL::getAngle(_positionCenterX, _positionCenterY, equipmentList[i]->getPositionX(), equipmentList[i]->getPositionY()));
					}
					else if (IntersectRect(&sTemp, &_Rc, &equipmentList[i]->getRect()))
					{
						_startTime = GetTickCount();
						_isHit = true;
						_isKnockBack = true;
						_isAlpha = true;
						_curHP -= 1.0f;
					}
				}
				else if ((equipmentList[i]->getEquipKind() == EQUIPMENT_KIND_AGAHNIMBUBBLE && equipmentList[i]->getisActive()) || (equipmentList[i]->getEquipKind() == EQUIPMENT_KIND_AGAHNIMSMALLBUBBLE && equipmentList[i]->getisActive()))
				{
					if (IntersectRect(&sTemp, &_atkRc, &equipmentList[i]->getRect()))
					{
						if ((equipmentList[i]->getEquipKind() == EQUIPMENT_KIND_AGAHNIMBUBBLE && equipmentList[i]->getisActive()))
						{
							equipmentList[i]->setIsKnockBack(true);
							equipmentList[i]->setIsFlick(true);
							equipmentList[i]->setEquipAngle(UTIL::getAngle(_positionCenterX, _positionCenterY, equipmentList[i]->getPositionX(), equipmentList[i]->getPositionY()));
						}
					}
					else if (IntersectRect(&sTemp, &_Rc, &equipmentList[i]->getRect()))
					{
						_startTime = GetTickCount();
						_isHit = true;
						_isKnockBack = true;
						_isAlpha = true;
						_curHP -= 1.0f;
					}
				}
			}

		}
	}

	if (_isHit)
	{
		if (_isAlpha)
		{
			_playerAlphaTime += 1;
			if (_playerAlphaTime % 10 == 0)
			{
				_playerAlphaValue = 0;
			}
			else
			{
				_playerAlphaValue = 255;
			}

			if (_playerAlphaTime >= 200)
			{
				_playerAlphaValue = 255;
				_playerAlphaTime = 0;
			}
		}

		if (_isKnockBack)
		{
			float _hitPositionX = _positionX - cosf(nowAngle) * _speed * 3;
			float _hitPositionY = _positionY - -sinf(nowAngle) * _speed * 3;

			if (obstacle(PointMake(_hitPositionX, _hitPositionY), _playerDirection,
				SCENEMANAGER->getCurrentTileMap().getMapSizeX() * 24, SCENEMANAGER->getCurrentTileMap().getMapSizeY() * 24, SCENEMANAGER->getCurrentTileMap()))
			{
				setPlayerSceneMove(_hitPositionX, _hitPositionY, SCENEMANAGER->getCurrentTileMap().getMapSizeX(), SCENEMANAGER->getCurrentTileMap().getMapSizeY());
			}
			else
			{
				_isKnockBack = false;
			}

			if ((getDistance(_hitStartPositionX, _hitStartPositionY, _positionCenterX, _positionCenterY) >= _knockbackForce && _isKnockBack))
			{
				_isKnockBack = false;
			}
		}

		if ((getDistance(_hitStartPositionX, _hitStartPositionY, _positionCenterX, _positionCenterY) >= _knockbackForce && GetTickCount() - _startTime > 1500) || GetTickCount() - _startTime > 1500)
		{
			_isKnockBack = false;

			_startTime = GetTickCount();
			_isAlpha = false;
			_isHit = false;
			_playerAlphaValue = 255;
		}
	}//_isHit끝

	if (!_isKnockBack && !_isAlpha)
	{
		_hitStartPositionX = _positionCenterX;
		_hitStartPositionY = _positionCenterY;
	}


	for (int i = 0; i < equipmentList.size(); i++)
	{
		RECT sTemp;

		if (equipmentList[i]->getEquipKind() != EQUIPMENT_KIND_AGAHNIMBOLT &&
			equipmentList[i]->getEquipKind() != EQUIPMENT_KIND_AGAHNIMBUBBLE &&
			!equipmentList[i]->getisActive()) continue;

		if (IntersectRect(&sTemp, &_atkRc, &equipmentList[i]->getRect()))
		{
			if (equipmentList[i]->getEquipKind() == EQUIPMENT_KIND_AGAHNIMBOLT)
			{
				equipmentList[i]->setIsKnockBack(true);
				equipmentList[i]->setIsFlick(true);
				equipmentList[i]->setEquipAngle(UTIL::getAngle(_positionCenterX, _positionCenterY, equipmentList[i]->getPositionX(), equipmentList[i]->getPositionY()));
			}
			else if (equipmentList[i]->getEquipKind() == EQUIPMENT_KIND_AGAHNIMBUBBLE)
			{
				equipmentList[i]->setIsBubblePop(true);
			}

		}
	}
}

void player::playerAttack()
{
	if (KEYMANAGER->isOnceKeyDown('A') && !_isArrow && !_isLift)
	{
		_ani->setFrameIndex(0); // 애니메이션 프레임을 0으로 초기화시킨다
		_isAttack = true; // 공격상태로 전환
		_curAnimationState = ANIMATION_STATE::PLAYER_ATTACK; //공격애니메이션으로 상태를 바꾼다.

		switch (_playerDirection)
		{
		case DIRECTION_STATE::PLAYER_DOWN: // 아래
			_img = IMAGEMANAGER->findImage("player_down_attack");
			_ani = ANIMATIONMANAGER->findAnimation("player_down_attack");
			ANIMATIONMANAGER->start("player_down_attack");
			_atkRc = RectMakeCenter(_player.getPositionX() + 25, _player.getPositionY() + 65, 80, 40);
			break;
		case DIRECTION_STATE::PLAYER_UP: //위
			_img = IMAGEMANAGER->findImage("player_up_attack");
			_ani = ANIMATIONMANAGER->findAnimation("player_up_attack");
			ANIMATIONMANAGER->start("player_up_attack");
			_atkRc = RectMakeCenter(_player.getPositionX() + 25, _player.getPositionY() - 15, 80, 40);
			break;
		case DIRECTION_STATE::PLAYER_LEFT://왼쪽
			_img = IMAGEMANAGER->findImage("player_left_attack");
			_ani = ANIMATIONMANAGER->findAnimation("player_left_attack");
			ANIMATIONMANAGER->start("player_left_attack");
			_atkRc = RectMakeCenter(_player.getPositionX() - 15, _player.getPositionY() + 25, 40, 80);
			break;
		case DIRECTION_STATE::PLAYER_RIGHT://오른쪽
			_img = IMAGEMANAGER->findImage("player_right_attack");
			_ani = ANIMATIONMANAGER->findAnimation("player_right_attack");
			ANIMATIONMANAGER->start("player_right_attack");
			_atkRc = RectMakeCenter(_player.getPositionX() + 65, _player.getPositionY() + 25, 40, 80);
			break;
		}

		_sound.effSoundPlay("검");
	}
	if (_ani->getFrameIndex() == 4) // 애니메이션 루프가 false일때 랙트 초기화
	{
		_atkRc = RectMakeCenter(0, 0, 0, 0);
		_isAttack = false;
	}
}

void player::render(HDC hdc, bool isRectRend)
{
	//플레이어 그려주는 함수
	if (_isDebug)
	{
		Rectangle(hdc, _Rc.left, _Rc.top, _Rc.right, _Rc.bottom);
		Rectangle(hdc, _atkRc.left, _atkRc.top, _atkRc.right, _atkRc.bottom);
	}
	_img->aniAlphaRender(hdc, _positionX - 50, _positionY - 50, _ani, _playerAlphaValue);

	EQUIPMENTMANAGER->render(hdc);
}
bool player::obstacle(POINT movePosition, int direction, int mapSizeX, int mapSizeY, tileMap nowTileMap)
{
	RECT temp;

	//이동했을때에 렉트를 그려준다
	//이동전에 렉트충돌로 판단하기 위해서이다
	RECT moveRect = RectMakeCenter(movePosition.x + 25, movePosition.y + 30, _width, _height);

	if (!(MAPSTARTVIEWX < movePosition.x + 25 - (_width / 2) && movePosition.x + 25 + (_width / 2) < mapSizeX && MAPSTARTVIEWY < movePosition.y + 30 - (_height / 2) && movePosition.y + 30 + (_height / 2) < mapSizeY))
	{
		//캐릭터 렉트가 맵안에 존재하지 않으면
		return false;
	}

	//장애물과 충돌처리
	//이미지 중심점을 맞춰주기 위해서  x+25, y+30을 더해준다(렉트 사이즈 조정 크기와 같아야한다)
	//값을 24로 나눠 현재 타일맵이나 이동할 타일맵의 좌표를 구한다
	if (nowTileMap.getMapObj((moveRect.left + 2) / 24, (moveRect.top + 2) / 24) == OBJ_BLOCK || nowTileMap.getMapObj((moveRect.right - 2) / 24, (moveRect.top + 2) / 24) == OBJ_BLOCK
		|| nowTileMap.getMapObj((moveRect.left + 2) / 24, (moveRect.bottom - 2) / 24) == OBJ_BLOCK || nowTileMap.getMapObj((moveRect.right - 2) / 24, (moveRect.bottom - 2) / 24) == OBJ_BLOCK)
	{
		return false;
	}

	//상호작용 오브젝트와 충돌처리를 합니다
	vector<tagInteractionObject> interObjTemp = nowTileMap.getInteractionObject();
	for (int i = 0; i < interObjTemp.size(); i++)
	{
		if (interObjTemp[i].xPos*TILESIZE + interObjTemp[i].widthPos*TILESIZE< _camera->getCameraX() || interObjTemp[i].xPos*TILESIZE>_camera->getCameraX() + WINSIZEX)
		{
			continue;
		}
		else if (interObjTemp[i].yPos*TILESIZE + interObjTemp[i].heightPos*TILESIZE< _camera->getCameraY() || interObjTemp[i].yPos*TILESIZE>_camera->getCameraY() + WINSIZEY)
		{
			continue;
		}

		if (IntersectRect(&temp, &interObjTemp[i]._rc, &moveRect))
		{
			if (interObjTemp[i].obj == INTERACTIONOBJ_OPENDOOR)
			{
				return true;
			}
			if (interObjTemp[i].obj == INTERACTIONOBJ_LADDER)
			{
				if (KEYMANAGER->isStayKeyDown(VK_LEFT) || KEYMANAGER->isStayKeyDown(VK_RIGHT))
				{
					return false;
				}
				else if (KEYMANAGER->isStayKeyDown(VK_DOWN))
				{
					if (!_isLadder || _isLadderDirectionUp)
					{
						SCENEMANAGER->setNowFloor(3 - SCENEMANAGER->getNowFloor());
						_isLadder = true;
						_isLadderDirectionUp = false;
					}
					return true;
				}
				else if (KEYMANAGER->isStayKeyDown(VK_UP))
				{
					if (!_isLadder || !_isLadderDirectionUp)
					{
						SCENEMANAGER->setNowFloor(3 - SCENEMANAGER->getNowFloor());
						_isLadder = true;
						_isLadderDirectionUp = true;
					}
					return true;
				}


				return false;
			}

			if (interObjTemp[i].obj != INTERACTIONOBJ_NONE)
			{
				return false;
			}
		}
	}

	if (_isLadder)
	{
		_isLadder = false;
	}

	return true;
}

void player::ending()
{
	if (INVENTORYMANAGER->getGreenPendent() && INVENTORYMANAGER->getBluePendent() && INVENTORYMANAGER->getRedPendent())
	{
		_sound.effSoundOff("체력없음");
		_isHpLow = true;
		ENEMYMANAGER->release();
		EQUIPMENTMANAGER->isActiveOff();

		SCENEMANAGER->changeScene("endingscene");
	}
}

void player::setPlayerMove(int mapStartX, int mapStartY, int mapSizeX, int mapSizeY, float moveX, float moveY)
{
	//x이동
	if (_positionX < WINSIZEX / 2)
	{
		_positionX += moveX; // 이동
	}
	else if (_camera->getCameraX() >= 0 && _positionX <= mapSizeX - WINSIZEX / 2) // 카메라
	{
		_camera->setCameraX(_camera->getCameraX() + moveX);
		_positionX += moveX;

		if (_camera->getCameraX() < 0)
		{
			_camera->setCameraX(0);
		}
		if (_camera->getCameraX() > mapSizeX - WINSIZEX)
		{
			_camera->setCameraX(mapSizeX - WINSIZEX);
		}
	}
	else
	{
		_positionX += moveX;
	}


	//y이동
	if (_positionY <= WINSIZEY / 2)
	{
		_positionY += moveY;
	}
	else if (_camera->getCameraY() >= 0 && _positionY <= mapSizeY - WINSIZEY / 2)
	{
		_camera->setCameraY(_camera->getCameraY() + moveY);
		_positionY += moveY;

		if (_camera->getCameraY() < 0)
		{
			_camera->setCameraY(0);
		}

		if (_camera->getCameraY() > mapSizeY - WINSIZEY)
		{
			_camera->setCameraY(mapSizeY - WINSIZEY);
		}
	}
	else
	{
		_positionY += moveY;
	}

	_Rc = RectMakeCenter(_positionX + 25, _positionY + 30, _width, _height);
}

void player::setPlayerSceneMove(int x, int y, int mapWidth, int mapHeight)
{
	_positionX = x;
	_positionY = y;

	_Rc = RectMakeCenter(_positionX + 25, _positionY + 25, _width, _height);

	if ((WINSIZEX) / 2 > x)
	{
		_camera->setCameraX(0);
	}
	else
	{
		_camera->setCameraX(x - (WINSIZEX) / 2);

		if (_camera->getCameraX() > mapWidth*TILESIZE - (WINSIZEX))
		{
			_camera->setCameraX(mapWidth*TILESIZE - (WINSIZEX));
		}
	}

	if ((WINSIZEY) / 2 > y)
	{
		_camera->setCameraY(0);
	}
	else
	{
		_camera->setCameraY(y - (WINSIZEY) / 2);

		if (_camera->getCameraY() > mapHeight*TILESIZE - (WINSIZEY))
		{
			_camera->setCameraY(mapHeight*TILESIZE - (WINSIZEY));
		}
	}
}

