#include "stdafx.h"
#include "worm_boss.h"


worm_boss::worm_boss()
{
}


worm_boss::~worm_boss()
{
}

HRESULT worm_boss::init()
{
	_positionX = 0;					//몬스터의 중간위치
	_positionY = 0;					//몬스터의 중간위치
	_positionZ = 0;					//몬스터의 높이


	_monsterFrameCount = 0;			//조절
	_currentFrameX = 0;				//프레임이미지 x의 값
	_currentFrameY = 0;				//프레임이미지 y의 값

	_enemyAngle = 0;				//몬스터의 각도
	_enemySpeed = 0;				//몬스터의 속도

	_enemyDirection = ENEMY_IDLE;

	sprintf_s(_name, "worm");		//몬스터의 이름

	_level = 0;					//몬스터의 스텟
	_hp = 0;
	_hpMax = 0;
	_mp = 0;
	_mpMax = 0;
	_atk = 0;
	_def = 0;
	bounceTime = 0;

	_isFrameImage = false;				//몬스터 프레임이미지 여부
	_isMove = false;					//움직임이 없을시 상태값
	_isDie = false;					//생사여부 상태값

	//방향 설정
	_isTurnLeft = false;
	_isTurnRight = false;
	_isTurnLeftHalf = false;
	_isTurnRightHalf = false;
	_isTurnLeftQuarter = false;
	_isTurnRightQuarter = false;
	_isTurnLeftHalfQ = false;
	_isTurnRightHalfQ = false;
	turnAngle = 0;

	_isHit = false;

	startTime = 0;
	return S_OK;
}

void worm_boss::setEnemy(POINT position)
{
	_img = new image;
	_img->init("images/enemy/worm_boss.bmp", 300, 900, 2, 6, true, RGB(255, 0, 255));
	_eyeImg = new image;
	_eyeImg->init("images/enemy/worm_boss_eye.bmp", 1200, 1200, 8, 8, true, RGB(255, 0, 255));
	_positionX = position.x;
	_positionY = position.y;
	body_OneX = position.x - 70;
	body_OneY = position.y;
	body_TwoX = body_OneX - 60;
	body_TwoY = position.y;
	body_ThreeX = body_TwoX - 50;
	body_ThreeY = position.y;
	_tailX = body_ThreeX - 40;
	_tailY = position.y;
	//_enemyColor = BOSS;

	//머리부분 렉트
	_attackRc = RectMakeCenter(_positionX, _positionY, 75, 75);

	//꼬리부분 렉트
	_rc = RectMakeCenter(_tailX, _tailY, 42, 42);

	_hp = 5;
	_atk = 2;
	_enemySpeed = 4;

	//바로 움직이도록 테스트용
	_isMove = true;

	mainRadius = 75 / 2;
}

void worm_boss::release()
{
}

void worm_boss::update()
{
	curTileMap = SCENEMANAGER->getCurrentTileMap();

	_attackRc = RectMakeCenter(_positionX, _positionY, 75, 75);
	if (_isKnockBack)
	{
		_rc = RectMakeCenter(_tailX, _tailY, 0, 0);
	}
	else
	{
		_rc = RectMakeCenter(_tailX, _tailY, 42, 42);
	}


	move();
	if (_hp <= 2)
	{
		_enemySpeed = 7;
	}
	if (_isMove)
	{
		_positionX += cosf(_enemyAngle)*_enemySpeed;
		_positionY += -sinf(_enemyAngle)*_enemySpeed;
	}

	collision();
	heading();
	animation();

}

void worm_boss::render(HDC hdc)
{
	_img->frameRender(hdc, _tailX - _img->getFrameWidth() / 2, _tailY - _img->getFrameHeight() / 2, _img->getFrameX(), 4);
	_img->frameRender(hdc, body_ThreeX - _img->getFrameWidth() / 2, body_ThreeY - _img->getFrameHeight() / 2, _img->getFrameX(), 3);
	_img->frameRender(hdc, body_TwoX - _img->getFrameWidth() / 2, body_TwoY - _img->getFrameHeight() / 2, _img->getFrameX(), 2);
	_img->frameRender(hdc, body_OneX - _img->getFrameWidth() / 2, body_OneY - _img->getFrameHeight() / 2, _img->getFrameX(), 1);
	if (!_isBounce)
	{

		bounceTime = 0;
		_img->frameRender(hdc, _positionX - _img->getFrameWidth() / 2, _positionY - _img->getFrameHeight() / 2, _img->getFrameX(), 0);
	}
	else if (_isBounce)
	{
		bounceTime += 1;
		if (bounceTime % 5 == 0)
		{
			_img->frameRender(hdc, _positionX - _img->getFrameWidth() / 2, _positionY - _img->getFrameHeight() / 2, _img->getFrameX(), 0);

		}
		else
		{
			_img->frameRender(hdc, _positionX - _img->getFrameWidth() / 2, _positionY - _img->getFrameHeight() / 2, _img->getFrameX(), 5);
		}
			if (bounceTime > 60)
			{
				_isBounce = false;
			}
	}
	_eyeImg->frameRender(hdc, _positionX - _eyeImg->getFrameWidth() / 2, _positionY - _eyeImg->getFrameHeight() / 2, _eyeImg->getFrameX(), _eyeImg->getFrameY());

	if (_isDebug)
	{
		RectangleMakeCenter(hdc, _positionX, _positionY, 75, 75);
		RectangleMakeCenter(hdc, _tailX, _tailY, 42, 42);
	}

}

void worm_boss::animation()
{
	_monsterFrameCount++;
	//_img->setFrameY(0);
	if (_monsterFrameCount % 3 == 0)
	{
		_monsterFrameCount = 0;
		_img->setFrameX(_img->getMaxFrameX() - _monsterFrameIndex++);
		if (_monsterFrameIndex > _img->getMaxFrameX()) _monsterFrameIndex = 0;
	}

	switch (_enemyDirection)
	{
	case ENEMY_IDLE:
		break;
	case ENEMY_UP:
		_eyeImg->setFrameY(0);
		if (_isHit)
		{
			_monsterFrameCount2++;
			if (_monsterFrameCount2 % 2 == 0)
			{
				_monsterFrameCount2 = 0;
				if (_monsterFrameIndex2 > 8) _monsterFrameIndex2 = 0;
				_eyeImg->setFrameX(_monsterFrameIndex2++);
			}
		}
		else if (!_isHit)
		{
			_eyeImg->setFrameX(0);
		}
		break;
	case ENEMY_DOWN:
		_eyeImg->setFrameY(1);
		if (_isHit)
		{
			_monsterFrameCount2++;
			if (_monsterFrameCount2 % 2 == 0)
			{
				_monsterFrameCount2 = 0;
				if (_monsterFrameIndex2 > 8) _monsterFrameIndex2 = 0;
				_eyeImg->setFrameX(_monsterFrameIndex2++);
			}
		}
		else if (!_isHit)
		{
			_eyeImg->setFrameX(0);
		}
		break;
	case ENEMY_LEFT:
		_eyeImg->setFrameY(2);
		if (_isHit)
		{
			_monsterFrameCount2++;
			if (_monsterFrameCount2 % 2 == 0)
			{
				_monsterFrameCount2 = 0;
				if (_monsterFrameIndex2 > 8) _monsterFrameIndex2 = 0;
				_eyeImg->setFrameX(_monsterFrameIndex2++);
			}
		}
		else if (!_isHit)
		{
			_eyeImg->setFrameX(0);
		}
		break;
	case ENEMY_RIGHT:
		_eyeImg->setFrameY(3);
		if (_isHit)
		{
			_monsterFrameCount2++;
			if (_monsterFrameCount2 % 2 == 0)
			{
				_monsterFrameCount2 = 0;
				if (_monsterFrameIndex2 > 8) _monsterFrameIndex2 = 0;
				_eyeImg->setFrameX(_monsterFrameIndex2++);
			}
		}
		else if (!_isHit)
		{
			_eyeImg->setFrameX(0);
		}
		break;
	case ENEMY_UP_LEFT:
		_eyeImg->setFrameY(4);
		if (_isHit)
		{
			_monsterFrameCount2++;
			if (_monsterFrameCount2 % 2 == 0)
			{
				_monsterFrameCount2 = 0;
				if (_monsterFrameIndex2 > 8) _monsterFrameIndex2 = 0;
				_eyeImg->setFrameX(_monsterFrameIndex2++);
			}
		}
		else if (!_isHit)
		{
			_eyeImg->setFrameX(0);
		}
		break;
	case ENEMY_UP_RIGHT:
		_eyeImg->setFrameY(5);
		if (_isHit)
		{
			_monsterFrameCount2++;
			if (_monsterFrameCount2 % 2 == 0)
			{
				_monsterFrameCount2 = 0;
				if (_monsterFrameIndex2 > 8) _monsterFrameIndex2 = 0;
				_eyeImg->setFrameX(_monsterFrameIndex2++);
			}
		}
		else if (!_isHit)
		{
			_eyeImg->setFrameX(0);
		}
		break;
	case ENEMY_DOWN_LEFT:
		_eyeImg->setFrameY(7);
		if (_isHit)
		{
			_monsterFrameCount2++;
			if (_monsterFrameCount2 % 2 == 0)
			{
				_monsterFrameCount2 = 0;
				if (_monsterFrameIndex2 > 8) _monsterFrameIndex2 = 0;
				_eyeImg->setFrameX(_monsterFrameIndex2++);
			}
		}
		else if (!_isHit)
		{
			_eyeImg->setFrameX(0);
		}
		break;
	case ENEMY_DOWN_RIGHT:
		_eyeImg->setFrameY(6);
		if (_isHit)
		{
			_monsterFrameCount2++;
			if (_monsterFrameCount2 % 2 == 0)
			{
				_monsterFrameCount2 = 0;
				if (_monsterFrameIndex2 > 8) _monsterFrameIndex2 = 0;
				_eyeImg->setFrameX(_monsterFrameIndex2++);
			}
		}
		else if (!_isHit)
		{
			_eyeImg->setFrameX(0);
		}
		break;
	default:
		break;
	}
}

void worm_boss::collision()
{
	if (_isKnockBack)
	{
		_isHit = true;
		_isMove = false;
		_rcAlphaTime += 1;
		if (_rcAlphaTime >= 100)
		{
			_rcAlpha = 255;
			_isKnockBack = false;
			_rcAlphaTime = 0;
			_isHit = false;
			_isMove = true;
		}

	}
}

void worm_boss::heading()
{
	if (_enemyAngle >= 3 * (PI / 8) && _enemyAngle <= 5 * (PI / 8))
	{
		if (_moveDirection != ENEMY_UP)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_UP;
		}
		_enemyDirection = ENEMY_UP;
	}
	else if (_enemyAngle >= 5 * (PI / 8) && _enemyAngle <= 7 * (PI / 8))
	{
		if (_moveDirection != ENEMY_UP_LEFT)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_UP_LEFT;
		}
		_enemyDirection = ENEMY_UP_LEFT;
	}
	else if (_enemyAngle >= 7 * (PI / 8) && _enemyAngle <= 9 * (PI / 8))
	{
		if (_moveDirection != ENEMY_LEFT)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_LEFT;
		}
		_enemyDirection = ENEMY_LEFT;
	}
	else if (_enemyAngle >= 9 * (PI / 8) && _enemyAngle <= 11 * (PI / 8))
	{
		if (_moveDirection != ENEMY_DOWN_LEFT)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_DOWN_LEFT;
		}
		_enemyDirection = ENEMY_DOWN_LEFT;
	}
	else if (_enemyAngle >= 11 * (PI / 8) && _enemyAngle <= 13 * (PI / 8))
	{
		if (_moveDirection != ENEMY_DOWN)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_DOWN;
		}
		_enemyDirection = ENEMY_DOWN;
	}
	else if (_enemyAngle >= 13 * (PI / 8) && _enemyAngle <= 15 * (PI / 8))
	{
		if (_moveDirection != ENEMY_DOWN_RIGHT)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_DOWN_RIGHT;
		}
		_enemyDirection = ENEMY_DOWN_RIGHT;
	}
	else if (_enemyAngle >= 15 * (PI / 8) && _enemyAngle <= PI2 || _enemyAngle >= 0 && _enemyAngle <= (PI / 8))
	{
		if (_moveDirection != ENEMY_RIGHT)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_RIGHT;
		}
		_enemyDirection = ENEMY_RIGHT;
	}
	else if (_enemyAngle >= (PI / 8) && _enemyAngle <= 3 * (PI / 8))
	{
		if (_moveDirection != ENEMY_UP_RIGHT)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_UP_RIGHT;
		}
		_enemyDirection = ENEMY_UP_RIGHT;
	}

	//////////////////////////////////////////////////////////////////////
	if (_enemyAngle >= PI / 4 && _enemyAngle <= 3 * PI / 4)
	{
		_checkDirection = ENEMY_UP;
	}
	else if (_enemyAngle >= 3 * PI / 4 && _enemyAngle <= 5 * PI / 4)
	{
		_checkDirection = ENEMY_LEFT;
	}
	else if (_enemyAngle >= 5 * PI / 4 && _enemyAngle <= 7 * PI / 4)
	{
		_checkDirection = ENEMY_DOWN;
	}
	else if (_enemyAngle >= 7 * PI / 4 && _enemyAngle <= PI2 || _enemyAngle >= 0 && _enemyAngle <= PI / 4)
	{
		_checkDirection = ENEMY_RIGHT;
	}
}

void worm_boss::move()
{
	if (getDistance(body_OneX, body_OneY, _positionX, _positionY) >= 65)
	{
		body_OneAngle = getAngle(body_OneX, body_OneY, _positionX, _positionY);
	}
	if (getDistance(body_TwoX, body_TwoY, body_OneX, body_OneY) >= 55)
	{
		body_TwoAngle = getAngle(body_TwoX, body_TwoY, body_OneX, body_OneY);
	}
	if (getDistance(body_ThreeX, body_ThreeY, body_TwoX, body_TwoY) >= 45)
	{
		body_ThreeAngle = getAngle(body_ThreeX, body_ThreeY, body_TwoX, body_TwoY);
	}
	if (getDistance(_tailX, _tailY, body_ThreeX, body_ThreeY) >= 35)
	{
		_tailAngle = getAngle(_tailX, _tailY, body_ThreeX, body_ThreeY);
	}

	if (getDistance(body_OneX, body_OneY, _positionX, _positionY) >= 5)
	{
		body_OneX += cosf(body_OneAngle)*_enemySpeed;
		body_OneY += -sinf(body_OneAngle)*_enemySpeed;
	}
	if (getDistance(body_TwoX, body_TwoY, body_OneX, body_OneY) >= 5)
	{
		body_TwoX += cosf(body_TwoAngle)*_enemySpeed;
		body_TwoY += -sinf(body_TwoAngle)*_enemySpeed;
	}
	if (getDistance(body_ThreeX, body_ThreeY, body_TwoX, body_TwoY) >= 5)
	{
		body_ThreeX += cosf(body_ThreeAngle)*_enemySpeed;
		body_ThreeY += -sinf(body_ThreeAngle)*_enemySpeed;
	}
	if (getDistance(_tailX, _tailY, body_ThreeX, body_ThreeY) >= 5)
	{
		_tailX += cosf(_tailAngle)*_enemySpeed;
		_tailY += -sinf(_tailAngle)*_enemySpeed;
	}


	if (GetTickCount() - startTime > 3000 && !_isHit || _isBlock)
	{
		turnAngle = 0;

		startTime = GetTickCount();

		rndMoveNum = RND->getInt(5);

		switch (rndMoveNum)
		{
		case 0:
			_isTurnLeftQuarter = true;
			break;
		case 1:
			_isTurnLeftHalf = true;
			break;
		case 2:
			_isTurnLeftHalfQ = true;
			break;
		case 3:
			_isTurnRightQuarter = true;
			break;
		case 4:
			_isTurnRightHalf = true;
			break;
		case 5:
			_isTurnRightHalfQ = true;
			break;
		default:
			break;
		}
		_isBlock = false;
	}

	if (_isTurnLeftHalf)
	{
		_enemyAngle += 0.05;
		turnAngle += 0.05;

		if (turnAngle >= 4 * (PI / 4)) _isTurnLeftHalf = false;
		if (_enemyAngle >= PI2) _enemyAngle = 0;
	}
	else if (_isTurnRightHalf)
	{
		_enemyAngle -= 0.05;
		turnAngle += 0.05;

		if (turnAngle >= 4 * (PI / 4)) _isTurnRightHalf = false;
		if (_enemyAngle <= 0) _enemyAngle = PI2;
	}
	else if (_isTurnLeftQuarter)
	{
		_enemyAngle += 0.05;
		turnAngle += 0.05;

		if (turnAngle >= 2 * (PI / 4)) _isTurnLeftQuarter = false;
		if (_enemyAngle >= PI2) _enemyAngle = 0;
	}
	else if (_isTurnRightQuarter)
	{
		_enemyAngle -= 0.05;
		turnAngle += 0.05;

		if (turnAngle >= 2 * (PI / 4)) _isTurnRightQuarter = false;
		if (_enemyAngle <= 0) _enemyAngle = PI2;
	}
	else if (_isTurnLeftHalfQ)
	{
		_enemyAngle += 0.05;
		turnAngle += 0.05;

		if (turnAngle >= 5 * (PI / 4)) _isTurnLeftHalfQ = false;
		if (_enemyAngle >= PI2) _enemyAngle = 0;
	}
	else if (_isTurnRightHalfQ)
	{
		_enemyAngle -= 0.05;
		turnAngle += 0.05;

		if (turnAngle >= 5 * (PI / 4)) _isTurnRightHalfQ = false;
		if (_enemyAngle <= 0) _enemyAngle = PI2;
	}


	if (curTileMap.getMapObj(_positionX / 24, (_positionY + mainRadius) / 24) == OBJ_BLOCK ||
		_positionY + mainRadius >= (curTileMap.getMapSizeY() * 24))
	{
		_positionY -= 1;
		_enemyAngle = PI2 - _enemyAngle;
		_isBlock = true;
	}
	if (curTileMap.getMapObj(_positionX / 24, (_positionY - mainRadius) / 24) == OBJ_BLOCK ||
		_positionY - mainRadius <= 0)
	{
		_positionY += 1;
		_enemyAngle = PI2 - _enemyAngle;
		_isBlock = true;
	}
	if (curTileMap.getMapObj((_positionX + mainRadius) / 24, _positionY / 24) == OBJ_BLOCK ||
		_positionX + mainRadius >= (curTileMap.getMapSizeX() * 24))
	{
		_positionX -= 1;
		_enemyAngle = PI - _enemyAngle;
		_isBlock = true;
	}
	if (curTileMap.getMapObj((_positionX - mainRadius) / 24, _positionY / 24) == OBJ_BLOCK || _positionX - mainRadius <= 0)
	{
		_positionX += 1;
		_enemyAngle = PI - _enemyAngle;
		_isBlock = true;
	}

	if (_enemyAngle > PI2)
	{
		_enemyAngle -= PI2;
	}
	else if (_enemyAngle < 0)
	{
		_enemyAngle += PI2;
	}

}