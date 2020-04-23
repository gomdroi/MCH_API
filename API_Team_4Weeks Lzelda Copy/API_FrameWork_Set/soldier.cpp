#include "stdafx.h"
#include "soldier.h"


soldier::soldier()
{
}


soldier::~soldier()
{
}

HRESULT soldier::init()
{
	_positionX = 0;					//몬스터의 중간위치
	_positionY = 0;					//몬스터의 중간위치
	_positionZ = 0;					//몬스터의 높이


	_monsterFrameCount = 0;			//조절
	_currentFrameX = 0;				//프레임이미지 x의 값
	_currentFrameY = 0;				//프레임이미지 y의 값

	_enemyAngle = 0;				//몬스터의 각도
	_enemySpeed = 0;				//몬스터의 속도

	_rcAlpha = 255;
	_rcAlphaTime = 0;

	_enemyDirection = ENEMY_IDLE;

	sprintf_s(_name, "바보");		//몬스터의 이름

	_level = 0;					//몬스터의 스텟
	_hp = 0;
	_hpMax = 0;
	_mp = 0;
	_mpMax = 0;
	_atk = 0;
	_def = 0;

	_isFrameImage = false;				//몬스터 프레임이미지 여부
	_isMove = false;					//움직임이 없을시 상태값
	_isDie = false;					//생사여부 상태값
	_isStunAtk = false;
	_isStun = false;

	_stunTime = 0;
	startTime = 0;

	return S_OK;
}

void soldier::setEnemy(ENEMY_COLOR soldiercolor, POINT position)
{
	switch (soldiercolor)
	{
	case GREEN:
		_img = new image;
		_img->init("images/enemy/soldier_green.bmp", 600, 600, 4, 4, true, RGB(255, 0, 255));
		_positionX = position.x;
		_positionY = position.y;
		_rc = RectMake(_positionX, _positionY, 60, 66);
		_hp = 3;
		_atk = 1;
		_enemySpeed = 0;

		break;
	case BLUE:
		_img = new image;
		_img->init("images/enemy/soldier_blue.bmp", 600, 600, 4, 4, true, RGB(255, 0, 255));
		_positionX = position.x;
		_positionY = position.y;
		_rc = RectMake(_positionX, _positionY, 60, 66);
		_hp = 4;
		_atk = 1;
		_enemySpeed = 0;

		break;
	default:
		break;
	}
}

void soldier::release()
{
}

void soldier::update()
{
	curTileMap = SCENEMANAGER->getCurrentTileMap();

	if (!_isKnockBack)
	{
		_rc = RectMakeCenter(_positionX, _positionY, 60, 66);
	}
	else
	{
		_rc = RectMakeCenter(_positionX, _positionY, 0, 0);
	}

	if (!_isStun)
	{
		//에너미 방향
		heading();
		//색적
		detection();
		//움직일 때
		if (_isMove)
		{
			animation();

			float positionTempX = _positionX + cosf(_enemyAngle) * _enemySpeed;
			float positionTempY = _positionY + -sinf(_enemyAngle) * _enemySpeed;

			RECT garbegeRect, rectTemp = RectMakeCenter(positionTempX, positionTempY, _rc.right - _rc.left, _rc.bottom - _rc.top);
			vector<tagInteractionObject> interObjTemp = curTileMap.getInteractionObject();

			for (int i = 0; i < interObjTemp.size(); i++)
			{
				if (interObjTemp[i].xPos*TILESIZE + interObjTemp[i].widthPos*TILESIZE< _player.getCameraX() || interObjTemp[i].xPos*TILESIZE>_player.getCameraX() + WINSIZEX)
				{
					if (!(i == interObjTemp.size() - 1))
					{
						continue;
					}
				}
				else if (interObjTemp[i].yPos*TILESIZE + interObjTemp[i].heightPos*TILESIZE< _player.getCameraY() || interObjTemp[i].yPos*TILESIZE>_player.getCameraY() + WINSIZEY)
				{
					if (!(i == interObjTemp.size() - 1))
					{
						continue;
					}
				}

				if (IntersectRect(&garbegeRect, &interObjTemp[i]._rc, &rectTemp))
				{
					return;
				}
				if (i == interObjTemp.size() - 1)
				{
					_positionX += cosf(_enemyAngle)*_enemySpeed;
					_positionY += -sinf(_enemyAngle)*_enemySpeed;
				}
			}

			//이동 예외처리
			move();
		}

	}



	collision();
	//플레이어 탐사 및 랜덤이동


}

void soldier::render(HDC hdc)
{
	/*_img->frameRender(hdc, _positionX - _img->getFrameWidth() / 2, _positionY - _img->getFrameHeight() / 2, _img->getFrameX(), _img->getFrameY());*/

	_img->alphaRender(hdc, _positionX - _img->getFrameWidth() / 2, _positionY - _img->getFrameHeight() / 2, _img->getFrameX() * 150, _img->getFrameY() * 150, 150, 150, _rcAlpha);

	if (_isDebug)
	{
		switch (_enemyDirection)
		{
		case ENEMY_IDLE:
			break;
		case ENEMY_UP:
			RectangleMakeCenter(hdc, _positionX, _positionY - 150, 650, 300);
			RectangleMake(hdc, _rc.right - 15, _rc.top - 25, 15, 25);
			break;
		case ENEMY_DOWN:
			RectangleMakeCenter(hdc, _positionX, _positionY + 150, 650, 300);
			RectangleMake(hdc, _rc.left - 5, _rc.bottom, 15, 35);
			break;
		case ENEMY_LEFT:
			RectangleMakeCenter(hdc, _positionX - 150, _positionY, 300, 650);
			RectangleMake(hdc, _rc.left - 35, _rc.bottom - 25, 30, 15);
			break;
		case ENEMY_RIGHT:
			RectangleMakeCenter(hdc, _positionX + 150, _positionY, 300, 650);
			RectangleMake(hdc, _rc.right + 5, _rc.bottom - 25, 30, 15);
			break;
		default:
			break;
		}
		RectangleMakeCenter(hdc, _positionX, _positionY, 60, 66);
	}
}

void soldier::collision()
{

	if (_isKnockBack)
	{
		_enemyAngle = getAngle(_positionX, _positionY, _player.getPositionCenterX(), _player.getPositionCenterY());

		_rcAlphaTime += 1;
		if (_rcAlphaTime % 4 == 0)
		{
			_rcAlpha = 0;
		}
		else
		{
			_rcAlpha = 255;
		}

		_isMove = false;


		if (getDistance(_postPositionX, _postPositionY, _positionX, _positionY) <= 150)
		{
			float positionTempX = _positionX - cosf(_enemyAngle) * 15;
			float positionTempY = _positionY + sinf(_enemyAngle) * 15;

			RECT garbegeRect, rectTemp = RectMakeCenter(positionTempX, positionTempY, 60, 66);

			if (curTileMap.getMapObj((rectTemp.left - 5) / 24, (rectTemp.top - 5) / 24) != OBJ_BLOCK &&
				curTileMap.getMapObj((rectTemp.right + 5) / 24, (rectTemp.top - 5) / 24) != OBJ_BLOCK &&
				curTileMap.getMapObj((rectTemp.left - 5) / 24, (rectTemp.bottom + 5) / 24) != OBJ_BLOCK &&
				curTileMap.getMapObj((rectTemp.right + 5) / 24, (rectTemp.bottom + 5) / 24) != OBJ_BLOCK)
			{

				vector<tagInteractionObject> interObjTemp = curTileMap.getInteractionObject();

				for (int i = 0; i < interObjTemp.size(); i++)
				{
					if (interObjTemp[i].xPos*TILESIZE + interObjTemp[i].widthPos*TILESIZE< _player.getCameraX() || interObjTemp[i].xPos*TILESIZE>_player.getCameraX() + WINSIZEX)
					{
						if (!(i == interObjTemp.size() - 1))
						{
							continue;
						}
					}
					else if (interObjTemp[i].yPos*TILESIZE + interObjTemp[i].heightPos*TILESIZE< _player.getCameraY() || interObjTemp[i].yPos*TILESIZE>_player.getCameraY() + WINSIZEY)
					{
						if (!(i == interObjTemp.size() - 1))
						{
							continue;
						}
					}

					if (IntersectRect(&garbegeRect, &interObjTemp[i]._rc, &rectTemp))
					{
						break;
					}
					if (i == interObjTemp.size() - 1)
					{
						_positionX = positionTempX;
						_positionY = positionTempY;
					}
				}
			}
		}

		if (_rcAlphaTime >= 50)
		{
			_rcAlpha = 255;
			_isKnockBack = false;
			_rcAlphaTime = 0;
			if (_isStunAtk && !_isStun)
			{
				_isStun = true;
			}
		}

	}
	else if (!_isKnockBack)
	{
		_postPositionX = _positionX;
		_postPositionY = _positionY;
	}

	if (_isStun)
	{
		_stunTime += 1;

		if (_stunTime >= 150 && _stunTime % 5 == 1)
		{
			_positionX -= 4;
		}
		else if (_stunTime >= 150 && _stunTime % 5 == 0)
		{
			_positionX += 4;
		}

		if (_stunTime >= 200)
		{
			_isStun = false;
			_stunTime = 0;
		}
	}
}

void soldier::move()
{
	if (_isMove)
	{
		if (_checkDirection == ENEMY_UP)
		{
			if (curTileMap.getMapObj(_positionX / 24, _rc.top / 24) == OBJ_BLOCK ||
				curTileMap.getMapObj(_rc.left / 24, _rc.top / 24) == OBJ_BLOCK ||
				curTileMap.getMapObj(_rc.right / 24, _rc.top / 24) == OBJ_BLOCK)
			{
				_positionY += 1;
				_positionY += -sinf(PI2 - _enemyAngle)*(_enemySpeed);
			}
			if (curTileMap.getMapObj(_rc.left / 24, _positionY / 24) == OBJ_BLOCK)
			{
				_positionX += 1;
				_positionX += cosf(PI - _enemyAngle)*(_enemySpeed);
			}
			if (curTileMap.getMapObj(_rc.right / 24, _positionY / 24) == OBJ_BLOCK)
			{
				_positionX -= 1;
				_positionX += cosf(PI - _enemyAngle)*(_enemySpeed);
			}
		}

		if (_checkDirection == ENEMY_DOWN)
		{
			if (curTileMap.getMapObj(_positionX / 24, _rc.bottom / 24) == OBJ_BLOCK ||
				curTileMap.getMapObj(_rc.left / 24, _rc.bottom / 24) == OBJ_BLOCK ||
				curTileMap.getMapObj(_rc.right / 24, _rc.bottom / 24) == OBJ_BLOCK)
			{
				_positionY -= 1;
				_positionY += -sinf(PI2 - _enemyAngle)*(_enemySpeed);
			}

			if (curTileMap.getMapObj(_rc.left / 24, _positionY / 24) == OBJ_BLOCK)
			{
				_positionX += 1;
				_positionX += cosf(PI - _enemyAngle)*(_enemySpeed);
			}
			if (curTileMap.getMapObj(_rc.right / 24, _positionY / 24) == OBJ_BLOCK)
			{
				_positionX -= 1;
				_positionX += cosf(PI - _enemyAngle)*(_enemySpeed);
			}
		}

		if (_checkDirection == ENEMY_LEFT)
		{
			if (curTileMap.getMapObj(_rc.left / 24, _rc.top / 24) == OBJ_BLOCK ||
				curTileMap.getMapObj(_rc.left / 24, _positionY / 24) == OBJ_BLOCK ||
				curTileMap.getMapObj(_rc.left / 24, _rc.bottom / 24) == OBJ_BLOCK)
			{
				_positionX += 1;
				_positionX += cosf(PI - _enemyAngle)*(_enemySpeed);
			}

			if (curTileMap.getMapObj(_positionX / 24, _rc.top / 24) == OBJ_BLOCK)
			{
				_positionY += 1;
				_positionY += -sinf(PI2 - _enemyAngle)*(_enemySpeed);
			}
			if (curTileMap.getMapObj(_positionX / 24, _rc.bottom / 24) == OBJ_BLOCK)
			{
				_positionY -= 1;
				_positionY += -sinf(PI2 - _enemyAngle)*(_enemySpeed);
			}
		}

		if (_checkDirection == ENEMY_RIGHT)
		{
			if (curTileMap.getMapObj(_rc.right / 24, _rc.top / 24) == OBJ_BLOCK ||
				curTileMap.getMapObj(_rc.right / 24, _positionY / 24) == OBJ_BLOCK ||
				curTileMap.getMapObj(_rc.right / 24, _rc.bottom / 24) == OBJ_BLOCK)
			{
				_positionX -= 1;
				_positionX += cosf(PI - _enemyAngle)*(_enemySpeed);
			}

			if (curTileMap.getMapObj(_positionX / 24, _rc.top / 24) == OBJ_BLOCK)
			{
				_positionY += 1;
				_positionY += -sinf(PI2 - _enemyAngle)*(_enemySpeed);
			}
			if (curTileMap.getMapObj(_positionX / 24, _rc.bottom / 24) == OBJ_BLOCK)
			{
				_positionY -= 1;
				_positionY += -sinf(PI2 - _enemyAngle)*(_enemySpeed);
			}
		}


	}

	if (_rc.right + 10 >= curTileMap.getMapSizeX() * 24)
	{
		_positionX -= 10;
	}
	if (_rc.left - 10 <= MAPSTARTVIEWX)
	{
		_positionX += 10;
	}
	if (_rc.bottom + 10 >= curTileMap.getMapSizeY() * 24)
	{
		_positionY -= 10;
	}
	if (_rc.top - 10 <= MAPSTARTVIEWY)
	{
		_positionY += 10;
	}
}

void soldier::animation()
{
	if (!_isFindPlayer)
	{
		switch (_enemyDirection)
		{

		case ENEMY_UP:

			_monsterFrameCount++;
			_img->setFrameY(0);
			if (_monsterFrameCount % 8 == 0)
			{
				_monsterFrameCount = 0;
				_img->setFrameX(_img->getMaxFrameX() - _monsterFrameIndex++);
				if (_monsterFrameIndex > _img->getMaxFrameX()) _monsterFrameIndex = 0;
			}

			break;
		case ENEMY_DOWN:

			_monsterFrameCount++;
			_img->setFrameY(1);
			if (_monsterFrameCount % 8 == 0)
			{
				_monsterFrameCount = 0;
				_img->setFrameX(_img->getMaxFrameX() - _monsterFrameIndex++);
				if (_monsterFrameIndex > _img->getMaxFrameX()) _monsterFrameIndex = 0;
			}

			break;
		case ENEMY_LEFT:

			_monsterFrameCount++;
			_img->setFrameY(2);
			if (_monsterFrameCount % 8 == 0)
			{
				_monsterFrameCount = 0;
				_img->setFrameX(_monsterFrameIndex++);
				if (_monsterFrameIndex == 3) _monsterFrameIndex = 0;
			}

			break;
		case ENEMY_RIGHT:

			_monsterFrameCount++;
			_img->setFrameY(3);
			if (_monsterFrameCount % 8 == 0)
			{
				_monsterFrameCount = 0;
				_img->setFrameX(_monsterFrameIndex++);
				if (_monsterFrameIndex == 3) _monsterFrameIndex = 0;
			}

			break;

		default:
			break;
		}
	}
	else
	{
		switch (_enemyDirection)
		{

		case ENEMY_UP:

			_monsterFrameCount++;
			_img->setFrameY(0);
			if (_monsterFrameCount % 4 == 0)
			{
				_monsterFrameCount = 0;
				_img->setFrameX(_img->getMaxFrameX() - _monsterFrameIndex++);
				if (_monsterFrameIndex > _img->getMaxFrameX()) _monsterFrameIndex = 0;
			}

			break;
		case ENEMY_DOWN:

			_monsterFrameCount++;
			_img->setFrameY(1);
			if (_monsterFrameCount % 4 == 0)
			{
				_monsterFrameCount = 0;
				_img->setFrameX(_img->getMaxFrameX() - _monsterFrameIndex++);
				if (_monsterFrameIndex > _img->getMaxFrameX()) _monsterFrameIndex = 0;
			}

			break;
		case ENEMY_LEFT:

			_monsterFrameCount++;
			_img->setFrameY(2);
			if (_monsterFrameCount % 4 == 0)
			{
				_monsterFrameCount = 0;
				_img->setFrameX(_monsterFrameIndex++);
				if (_monsterFrameIndex == 3) _monsterFrameIndex = 0;
			}

			break;
		case ENEMY_RIGHT:

			_monsterFrameCount++;
			_img->setFrameY(3);
			if (_monsterFrameCount % 4 == 0)
			{
				_monsterFrameCount = 0;
				_img->setFrameX(_monsterFrameIndex++);
				if (_monsterFrameIndex == 3) _monsterFrameIndex = 0;
			}

			break;

		default:
			break;
		}
	}

}

void soldier::heading()
{
	if (_enemyAngle >= PI / 4 && _enemyAngle <= 3 * PI / 4)
	{
		if (_moveDirection != ENEMY_UP)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_UP;
		}
		_enemyDirection = ENEMY_UP;
		_checkDirection = ENEMY_UP;
	}
	else if (_enemyAngle >= 3 * PI / 4 && _enemyAngle <= 5 * PI / 4)
	{
		if (_moveDirection != ENEMY_LEFT)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_LEFT;
		}
		_enemyDirection = ENEMY_LEFT;
		_checkDirection = ENEMY_LEFT;
	}
	else if (_enemyAngle >= 5 * PI / 4 && _enemyAngle <= 7 * PI / 4)
	{
		if (_moveDirection != ENEMY_DOWN)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_DOWN;
		}
		_enemyDirection = ENEMY_DOWN;
		_checkDirection = ENEMY_DOWN;
	}
	else if (_enemyAngle >= 7 * PI / 4 && _enemyAngle <= PI2 || _enemyAngle >= 0 && _enemyAngle <= PI / 4)
	{
		if (_moveDirection != ENEMY_RIGHT)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_RIGHT;
		}
		_enemyDirection = ENEMY_RIGHT;
		_checkDirection = ENEMY_RIGHT;
	}

	//방향에 따라 칼렉트와 탐지렉트를 바꿔준다.	
	switch (_enemyDirection)
	{
	case ENEMY_IDLE:
		break;
	case ENEMY_UP:
		_detectionRc = RectMake(_positionX - 300, _positionY - 300, 650, 300);
		_attackRc = RectMake(_rc.right - 15, _rc.top - 25, 15, 25);
		break;
	case ENEMY_DOWN:
		_detectionRc = RectMake(_positionX - 300, _positionY + 66, 650, 300);
		_attackRc = RectMake(_rc.left - 5, _rc.bottom, 15, 35);
		break;
	case ENEMY_LEFT:
		_detectionRc = RectMake(_positionX - 300, _positionY - 300, 300, 650);
		_attackRc = RectMake(_rc.left - 35, _rc.bottom - 25, 30, 15);
		break;
	case ENEMY_RIGHT:
		_detectionRc = RectMake(_positionX + 60, _positionY - 300, 300, 650);
		_attackRc = RectMake(_rc.right + 5, _rc.bottom - 25, 30, 15);
		break;
	default:
		break;
	}
}

void soldier::detection()
{
	//플레이어 발견
	if (PtInRect(&_detectionRc, _player.getPosition()))
	{
		if (!_isReady)
		{
			_enemySpeed = 0.0f;
			actionTime = GetTickCount();
		}
		_isReady = true;
		_isFindPlayer = true;
	}

	//플레이어를 발견하지 못하면
	if (!_isFindPlayer)
	{
		_enemySpeed = 2.0f;

		//5초마다 목적지 설정
		if (GetTickCount() - startTime > 5000)
		{
			_monsterFrameIndex = 0;
			_isMove = true;

			startTime = GetTickCount();
			int tempMove = RND->getInt(3);
			switch (tempMove)
			{
			case 0:
				_enemyAngle = getAngle(_positionX, _positionY, _positionX, _positionY - 200);
				destination = PointMake(_positionX, _positionY - 200);
				break;
			case 1:
				_enemyAngle = getAngle(_positionX, _positionY, _positionX, _positionY + 200);
				destination = PointMake(_positionX, _positionY + 200);
				break;
			case 2:
				_enemyAngle = getAngle(_positionX, _positionY, _positionX - 200, _positionY);
				destination = PointMake(_positionX - 200, _positionY);
				break;
			case 3:
				_enemyAngle = getAngle(_positionX, _positionY, _positionX + 200, _positionY);
				destination = PointMake(_positionX + 200, _positionY);
				break;
			default:
				break;
			}
		}

		//목적지에 도착하면 잠시 정지
		if (getDistance(_positionX, _positionY, destination.x, destination.y) < 20)
		{
			_isMove = false;
		}
	}
	//플레이어를 발견하면
	else if (_isFindPlayer)
	{
		if (GetTickCount() - actionTime > 1000)
		{
			_enemySpeed = 4.0f;
		}
		_enemyAngle = getAngle(_positionX, _positionY, _player.getPositionX(), _player.getPositionY());
		_isMove = true;

		//5초마다 체크	
		if (GetTickCount() - startTime > 5000)
		{
			//플레이어가 감지범위에 없으면
			if (PtInRect(&_detectionRc, m_ptMouse) == false)
			{
				_isMove = false;
				startTime = GetTickCount();
				_isFindPlayer = false;
				_isReady = false;
			}
		}

	}
}
