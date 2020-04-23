#include "stdafx.h"
#include "agahnim_boss.h"


agahnim_boss::agahnim_boss()
{
}


agahnim_boss::~agahnim_boss()
{
}

HRESULT agahnim_boss::init()
{
	_positionX = 0;					//몬스터의 중간위치
	_positionY = 0;					//몬스터의 중간위치
	_positionZ = 0;					//몬스터의 높이


	_monsterFrameCount = 0;			//조절
	_currentFrameX = 0;				//프레임이미지 x의 값
	_currentFrameY = 0;				//프레임이미지 y의 값
	_monsterFrameIndex = 0;

	_enemyAngle = 0;				//몬스터의 각도
	_enemySpeed = 0;				//몬스터의 속도

	_enemyDirection = ENEMY_IDLE;

	sprintf_s(_name, "agahnim");		//몬스터의 이름

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
	_isReady = false;
	_isBossOn = false;
	_isMoveSet = false;
	_isAttack = false;
	_isFading = false;
	_isCastLightning = false;
	_isCastBubble = false;
	_isCastBolt = false;
	_isCastPrep = false;

	_castPrepCount = 0;
	startTime = 0;
	actionTime = 0;
	lightningTime = 0;
	lightningCount = 0;
	_boltTime = 0;
	return S_OK;
	return S_OK;
}

void agahnim_boss::setEnemy(POINT position)
{
	_img = new image;
	_img->init("images/enemy/agahnim.bmp", 750, 1200, 5, 8, true, RGB(255, 0, 255));
	_imgBlack = new image;
	_imgBlack->init("images/enemy/agahnim_black.bmp", 750, 1200, 5, 8, true, RGB(255, 0, 255));
	_lightningImg = new image;
	_lightningImg->init("images/enemy/agahnim_lightning_56.bmp", 1245, 387, 5, 6, true, RGB(255, 0, 255));
	_boltImg = new image;
	_boltImg->init("images/enemy/agahnim_bolt.bmp", 300, 450, 2, 3, true, RGB(255, 0, 255));
	_positionX = position.x;
	_positionY = position.y;
	_enemyColor = BOSS;

	_rcAlpha = 255;
	_rc = RectMakeCenter(_positionX, _positionY, 100, 100);

	_hp = 5;
	_atk = 2;
	_enemySpeed = 4;

}

void agahnim_boss::release()
{
}

void agahnim_boss::update()
{
	curTileMap = SCENEMANAGER->getCurrentTileMap();

	_rc = RectMakeCenter(_positionX, _positionY, 70, 70);
	heading();
	move();
	animation();
	collision();
	//보스 등장 트리거
	if (KEYMANAGER->isStayKeyDown(VK_CONTROL))
	{
		_isReady = true;
		_isBossOn = true;
	}


}

void agahnim_boss::render(HDC hdc)
{
	if (_isCastPrep)
	{
		_castPrepCount += 1;
		switch (_enemyDirection)
		{
		case ENEMY_UP:
			if (_castPrepCount <= 30)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 49, _positionY - 150 + 33, _boltImg->getFrameX(), 0);
				_boltImg->frameRender(hdc, _positionX - 150 + 103, _positionY - 150 + 33, _boltImg->getFrameX(), 0);
			}
			else if (_castPrepCount > 30 && _castPrepCount <= 60)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 49, _positionY - 150 + 33, _boltImg->getFrameX(), 1);
				_boltImg->frameRender(hdc, _positionX - 150 + 103, _positionY - 150 + 33, _boltImg->getFrameX(), 1);
			}
			else if (_castPrepCount > 60 && _castPrepCount <= 90)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 49, _positionY - 150 + 33, _boltImg->getFrameX(), 2);
				_boltImg->frameRender(hdc, _positionX - 150 + 103, _positionY - 150 + 33, _boltImg->getFrameX(), 2);
			}
			else if (_castPrepCount > 90 && _castPrepCount <= 120)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 64, _positionY - 150 + 33, _boltImg->getFrameX(), 2);
				_boltImg->frameRender(hdc, _positionX - 150 + 88, _positionY - 150 + 33, _boltImg->getFrameX(), 2);
			}
			else if (_castPrepCount > 120 && _castPrepCount <= 140)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 76, _positionY - 150 + 33, _boltImg->getFrameX(), 2);
			}
			break;
		}
	}
	_img->alphaRender(hdc, _positionX - _img->getFrameWidth() / 2, _positionY - _img->getFrameHeight() / 2, _img->getFrameX() * 150, _img->getFrameY() * 150, 150, 150, _alpha);
	_imgBlack->alphaRender(hdc, _positionX - _img->getFrameWidth() / 2, _positionY - _img->getFrameHeight() / 2, _img->getFrameX() * 150, _img->getFrameY() * 150, 150, 150, _alpha2);

	//번개 렌더
	for (int i = 0; i < 6; i++)
	{
		if (lightning[i].right - lightning[i].left <= 0) continue;
		_lightningImg->frameRender(hdc, lightning[i].left, lightning[i].top, _lightningImg->getFrameX(), i);
	}

	if (_isCastPrep)
	{
		_castPrepCount += 1;

		switch (_enemyDirection)
		{
		case ENEMY_IDLE:
			break;
		case ENEMY_DOWN:
			if (_castPrepCount <= 30)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 42, _positionY - 150 + 47, _boltImg->getFrameX(), 0);
				_boltImg->frameRender(hdc, _positionX - 150 + 110, _positionY - 150 + 47, _boltImg->getFrameX(), 0);
			}
			else if (_castPrepCount > 30 && _castPrepCount <= 60)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 42, _positionY - 150 + 47, _boltImg->getFrameX(), 1);
				_boltImg->frameRender(hdc, _positionX - 150 + 110, _positionY - 150 + 47, _boltImg->getFrameX(), 1);
			}
			else if (_castPrepCount > 60 && _castPrepCount <= 90)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 42, _positionY - 150 + 47, _boltImg->getFrameX(), 2);
				_boltImg->frameRender(hdc, _positionX - 150 + 110, _positionY - 150 + 47, _boltImg->getFrameX(), 2);
			}
			else if (_castPrepCount > 90 && _castPrepCount <= 120)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 57, _positionY - 150 + 47, _boltImg->getFrameX(), 2);
				_boltImg->frameRender(hdc, _positionX - 150 + 95, _positionY - 150 + 47, _boltImg->getFrameX(), 2);
			}
			else if (_castPrepCount > 120)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 76, _positionY - 150 + 47, _boltImg->getFrameX(), 2);
			}
			break;
		case ENEMY_LEFT:
			if (_castPrepCount <= 30)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 42, _positionY - 150 + 50, _boltImg->getFrameX(), 0);
				_boltImg->frameRender(hdc, _positionX - 150 + 42, _positionY - 150 + 95, _boltImg->getFrameX(), 0);
			}
			else if (_castPrepCount > 30 && _castPrepCount <= 60)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 42, _positionY - 150 + 50, _boltImg->getFrameX(), 1);
				_boltImg->frameRender(hdc, _positionX - 150 + 42, _positionY - 150 + 95, _boltImg->getFrameX(), 1);
			}
			else if (_castPrepCount > 60 && _castPrepCount <= 90)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 42, _positionY - 150 + 50, _boltImg->getFrameX(), 2);
				_boltImg->frameRender(hdc, _positionX - 150 + 42, _positionY - 150 + 95, _boltImg->getFrameX(), 2);
			}
			else if (_castPrepCount > 90 && _castPrepCount <= 120)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 42, _positionY - 150 + 60, _boltImg->getFrameX(), 2);
				_boltImg->frameRender(hdc, _positionX - 150 + 42, _positionY - 150 + 85, _boltImg->getFrameX(), 2);
			}
			else if (_castPrepCount > 120)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 42, _positionY - 150 + 72, _boltImg->getFrameX(), 2);
			}
			break;
		case ENEMY_RIGHT:
			if (_castPrepCount <= 30)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 109, _positionY - 150 + 50, _boltImg->getFrameX(), 0);
				_boltImg->frameRender(hdc, _positionX - 150 + 109, _positionY - 150 + 95, _boltImg->getFrameX(), 0);
			}
			else if (_castPrepCount > 30 && _castPrepCount <= 60)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 109, _positionY - 150 + 50, _boltImg->getFrameX(), 1);
				_boltImg->frameRender(hdc, _positionX - 150 + 109, _positionY - 150 + 95, _boltImg->getFrameX(), 1);
			}
			else if (_castPrepCount > 60 && _castPrepCount <= 90)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 109, _positionY - 150 + 50, _boltImg->getFrameX(), 2);
				_boltImg->frameRender(hdc, _positionX - 150 + 109, _positionY - 150 + 95, _boltImg->getFrameX(), 2);
			}
			else if (_castPrepCount > 90 && _castPrepCount <= 120)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 109, _positionY - 150 + 60, _boltImg->getFrameX(), 2);
				_boltImg->frameRender(hdc, _positionX - 150 + 109, _positionY - 150 + 85, _boltImg->getFrameX(), 2);
			}
			else if (_castPrepCount > 120)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 109, _positionY - 150 + 72, _boltImg->getFrameX(), 2);
			}
			break;
		case ENEMY_DOWN_LEFT:
			if (_castPrepCount <= 30)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 43, _positionY - 150 + 43, _boltImg->getFrameX(), 0);
				_boltImg->frameRender(hdc, _positionX - 150 + 70, _positionY - 150 + 70, _boltImg->getFrameX(), 0);
			}
			else if (_castPrepCount > 30 && _castPrepCount <= 60)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 43, _positionY - 150 + 43, _boltImg->getFrameX(), 1);
				_boltImg->frameRender(hdc, _positionX - 150 + 70, _positionY - 150 + 70, _boltImg->getFrameX(), 1);
			}
			else if (_castPrepCount > 60 && _castPrepCount <= 90)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 43, _positionY - 150 + 43, _boltImg->getFrameX(), 2);
				_boltImg->frameRender(hdc, _positionX - 150 + 70, _positionY - 150 + 70, _boltImg->getFrameX(), 2);
			}
			else if (_castPrepCount > 90 && _castPrepCount <= 120)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 53, _positionY - 150 + 53, _boltImg->getFrameX(), 2);
				_boltImg->frameRender(hdc, _positionX - 150 + 60, _positionY - 150 + 60, _boltImg->getFrameX(), 2);
			}
			else if (_castPrepCount > 120)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 56, _positionY - 150 + 56, _boltImg->getFrameX(), 2);
			}
			break;
		case ENEMY_DOWN_RIGHT:
			if (_castPrepCount <= 30)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 109, _positionY - 150 + 43, _boltImg->getFrameX(), 0);
				_boltImg->frameRender(hdc, _positionX - 150 + 83, _positionY - 150 + 70, _boltImg->getFrameX(), 0);
			}
			else if (_castPrepCount > 30 && _castPrepCount <= 60)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 109, _positionY - 150 + 43, _boltImg->getFrameX(), 1);
				_boltImg->frameRender(hdc, _positionX - 150 + 83, _positionY - 150 + 70, _boltImg->getFrameX(), 1);
			}
			else if (_castPrepCount > 60 && _castPrepCount <= 90)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 109, _positionY - 150 + 43, _boltImg->getFrameX(), 2);
				_boltImg->frameRender(hdc, _positionX - 150 + 83, _positionY - 150 + 70, _boltImg->getFrameX(), 2);
			}
			else if (_castPrepCount > 90 && _castPrepCount <= 120)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 99, _positionY - 150 + 53, _boltImg->getFrameX(), 2);
				_boltImg->frameRender(hdc, _positionX - 150 + 93, _positionY - 150 + 60, _boltImg->getFrameX(), 2);
			}
			else if (_castPrepCount > 120)
			{
				_boltImg->frameRender(hdc, _positionX - 150 + 95, _positionY - 150 + 56, _boltImg->getFrameX(), 2);
			}
			break;
			break;
		default:
			break;
		}

	}
	if (_isDebug)
	{
		RectangleMakeCenter(hdc, _positionX, _positionY, 70, 70);
		RectangleMake(hdc, lightning[0].left, lightning[0].top, 249, 66);
		RectangleMake(hdc, lightning[1].left, lightning[1].top, 249, 66);
		RectangleMake(hdc, lightning[2].left, lightning[2].top, 249, 66);
		RectangleMake(hdc, lightning[3].left, lightning[3].top, 249, 66);
		RectangleMake(hdc, lightning[4].left, lightning[4].top, 249, 66);
		RectangleMake(hdc, lightning[5].left, lightning[5].top, 249, 66);

	}

}

void agahnim_boss::animation()
{
	if (!_isFading)
	{
		switch (_enemyDirection)
		{
		case ENEMY_IDLE:
			_img->setFrameY(0);
			break;
		case ENEMY_UP:

			if (_isAttack || _isCastPrep)
			{
				_img->setFrameY(1);
				_img->setFrameX(1);
			}
			else if (_isMove)
			{
				_img->setFrameY(0);
				_img->setFrameX(0);
			}
			else if (!_isAttack)
			{
				_img->setFrameY(1);
				_img->setFrameX(0);
			}
			break;
		case ENEMY_DOWN:

			if (_isAttack || _isCastPrep)
			{
				_img->setFrameY(2);
				_img->setFrameX(1);
			}
			else if (_isMove)
			{
				_img->setFrameY(0);
				_img->setFrameX(0);
			}
			else if (!_isAttack)
			{
				_img->setFrameY(2);
				_img->setFrameX(0);
			}
			break;
		case ENEMY_LEFT:

			if (_isAttack || _isCastPrep)
			{
				_img->setFrameY(3);
				_img->setFrameX(1);
			}

			else if (_isMove)
			{
				_img->setFrameY(0);
				_img->setFrameX(0);
			}
			else if (!_isAttack)
			{
				_img->setFrameY(3);
				_img->setFrameX(0);
			}
			break;
		case ENEMY_RIGHT:

			if (_isAttack || _isCastPrep)
			{
				_img->setFrameY(4);
				_img->setFrameX(1);
			}
			else if (_isMove)
			{
				_img->setFrameY(0);
				_img->setFrameX(0);
			}
			else if (!_isAttack)
			{
				_img->setFrameY(4);
				_img->setFrameX(0);
			}

			break;

		case ENEMY_DOWN_LEFT:

			if (_isAttack || _isCastPrep)
			{
				_img->setFrameY(5);
				_img->setFrameX(1);
			}
			else if (_isMove)
			{
				_img->setFrameY(0);
				_img->setFrameX(0);
			}
			else if (!_isAttack)
			{
				_img->setFrameY(5);
				_img->setFrameX(0);
			}
			break;
		case ENEMY_DOWN_RIGHT:

			if (_isAttack || _isCastPrep)
			{
				_img->setFrameY(6);
				_img->setFrameX(1);
			}
			else if (_isMove)
			{
				_img->setFrameY(0);
				_img->setFrameX(0);
			}
			else if (!_isAttack)
			{
				_img->setFrameY(6);
				_img->setFrameX(0);
			}
			break;
		default:
			break;
		}
	}

	_monsterFrameCount2++;
	if (_monsterFrameCount2 % 2 == 0)
	{
		_monsterFrameCount2 = 0;

		_lightningImg->setFrameX(_monsterFrameIndex2++);
		if (_monsterFrameIndex2 > _lightningImg->getMaxFrameX())
		{
			_monsterFrameIndex2 = 0;
		}
	}

	_boltFrameCount++;
	if (_boltFrameCount % 3 == 0)
	{
		_boltFrameCount = 0;
		_boltImg->setFrameX(_boltFrameIndex++);
		if (_boltFrameIndex > _boltImg->getMaxFrameX())
		{
			_boltFrameIndex = 0;
		}
	}
}

void agahnim_boss::collision()
{
	if (_isKnockBack)
	{
		_enemyAngle = getAngle(_positionX, _positionY, _player.getPositionCenterX(), _player.getPositionCenterY());

		_rcAlphaTime += 1;
		if (_rcAlphaTime % 4 == 0)
		{
			_alpha = 0;
		}
		else
		{
			_alpha = 255;
		}


		if (getDistance(_postPositionX, _postPositionY, _positionX, _positionY) <= 150)
		{
			float positionTempX = _positionX - cosf(_enemyAngle) * 15;
			float positionTempY = _positionY + sinf(_enemyAngle) * 15;

			RECT rectTemp = RectMakeCenter(positionTempX, positionTempY, _rc.right - _rc.left, _rc.bottom - _rc.top);

			if (curTileMap.getMapObj((rectTemp.left) / 24, (rectTemp.top) / 24) != OBJ_BLOCK &&
				curTileMap.getMapObj((rectTemp.right) / 24, (rectTemp.top) / 24) != OBJ_BLOCK &&
				curTileMap.getMapObj((rectTemp.left) / 24, (rectTemp.bottom) / 24) != OBJ_BLOCK &&
				curTileMap.getMapObj((rectTemp.right) / 24, (rectTemp.bottom) / 24) != OBJ_BLOCK)
			{
				_positionX = positionTempX;
				_positionY = positionTempY;
			}
		}

		if (_rcAlphaTime >= 100)
		{
			_alpha = 255;
			_isKnockBack = false;
			_rcAlphaTime = 0;
		}

	}
	else if (!_isKnockBack)
	{
		_postPositionX = _positionX;
		_postPositionY = _positionY;
	}

}


void agahnim_boss::heading()
{
	if (_enemyAngle >= 2 * (PI / 8) && _enemyAngle <= 6 * (PI / 8))
	{
		/*if (_moveDirection != ENEMY_UP)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_UP;
		}*/
		_enemyDirection = ENEMY_UP;
	}
	else if (_enemyAngle >= 6 * (PI / 8) && _enemyAngle <= 9 * (PI / 8))
	{
		/*if (_moveDirection != ENEMY_LEFT)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_LEFT;
		}*/
		_enemyDirection = ENEMY_LEFT;
	}
	else if (_enemyAngle >= 9 * (PI / 8) && _enemyAngle <= 11 * (PI / 8))
	{
		/*if (_moveDirection != ENEMY_DOWN_LEFT)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_DOWN_LEFT;
		}*/
		_enemyDirection = ENEMY_DOWN_LEFT;
	}
	else if (_enemyAngle >= 11 * (PI / 8) && _enemyAngle <= 13 * (PI / 8))
	{
		/*if (_moveDirection != ENEMY_DOWN)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_DOWN;
		}*/
		_enemyDirection = ENEMY_DOWN;
	}
	else if (_enemyAngle >= 13 * (PI / 8) && _enemyAngle <= 15 * (PI / 8))
	{
		/*if (_moveDirection != ENEMY_DOWN_RIGHT)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_DOWN_RIGHT;
		}*/
		_enemyDirection = ENEMY_DOWN_RIGHT;
	}
	else if (_enemyAngle >= 15 * (PI / 8) && _enemyAngle <= PI2 || _enemyAngle >= 0 && _enemyAngle <= 2 * (PI / 8))
	{
		/*if (_moveDirection != ENEMY_RIGHT)
		{
			_monsterFrameIndex = 0;
			_moveDirection = ENEMY_RIGHT;
		}*/
		_enemyDirection = ENEMY_RIGHT;
	}
}

void agahnim_boss::move()
{
	//퇴장 준비, 퇴장 애니메이션
	if (_isReady)
	{
		_isFading = true;
		_alpha2 += 10;

		if (_alpha2 >= 255)
		{
			_monsterFrameCount++;
			_img->setFrameY(8);
			if (_monsterFrameCount % 6 == 0)
			{
				_monsterFrameCount = 0;

				_img->setFrameX(_monsterFrameIndex++);
				if (_monsterFrameIndex > _img->getMaxFrameX())
				{
					_alpha2 = 255;
					_isMove = true;
					_isMoveSet = true;
					_isReady = false;
					_isFading = false;
					_monsterFrameIndex = 0;

				}
			}
		}
	}

	if (_isMove)
	{
		//랜덤 이동 부분
		if (_isMoveSet)
		{
			if (lightningCount == 3)
			{
				_enemyAngle = getAngle(_positionX, _positionY, 385, 325);
				fDestinationX = 385;
				fDestinationY = 325;
				lightningCount = 0;
				_isCastLightning = true;
			}
			else
			{
				int rndMoveNum = RND->getInt(6);
				switch (rndMoveNum)
				{

				case 0: //좌상
					_enemyAngle = getAngle(_positionX, _positionY, 220, 325);
					fDestinationX = 220;
					fDestinationY = 325;
					break;
				case 1: //상상
					_enemyAngle = getAngle(_positionX, _positionY, 385, 220);
					fDestinationX = 385;
					fDestinationY = 220;
					break;
				case 2: //상
					_enemyAngle = getAngle(_positionX, _positionY, 385, 325);
					fDestinationX = 385;
					fDestinationY = 325;
					break;
				case 3: //우상
					_enemyAngle = getAngle(_positionX, _positionY, 550, 325);
					fDestinationX = 550;
					fDestinationY = 325;
					break;
				case 4: //좌
					_enemyAngle = getAngle(_positionX, _positionY, 220, 475);
					fDestinationX = 220;
					fDestinationY = 475;
					break;
				case 5: //중앙
					_enemyAngle = getAngle(_positionX, _positionY, 385, 475);
					fDestinationX = 385;
					fDestinationY = 475;
					break;
				case 6: //우
					_enemyAngle = getAngle(_positionX, _positionY, 550, 475);
					fDestinationX = 550;
					fDestinationY = 475;
				default:
					break;
				}

				int rndAtkNum = RND->getInt(1);
				switch (rndAtkNum)
				{
				case 0:					
					_isCastBolt = true;
					break;
				case 1:
					_isCastBubble = true;
					break;
				default:
					break;
				}
			}
			_isMoveSet = false;
		}
		if (getDistance(_positionX, _positionY, fDestinationX, fDestinationY) >= 7)
		{

			_positionX += cosf(_enemyAngle)*_enemySpeed;
			_positionY += -sinf(_enemyAngle)*_enemySpeed;
		}
		//목적지에 도착하면 등장 애니메이션
		else if (getDistance(_positionX, _positionY, fDestinationX, fDestinationY) <= 7)
		{
			//프레임 튀는걸 방지하려고 미리 앵글을 잡아준다
			_enemyAngle = getAngle(_positionX, _positionY, _player.getPositionX(), _player.getPositionY());

			//등장 애니메이션하기전에 잠깐 텀을 준다
			startTime += 7;
			if (startTime >= 200) _isFading = true;

			if (_isFading)
			{
				_monsterFrameCount++;
				_img->setFrameY(0);
				if (_monsterFrameCount % 6 == 0)
				{
					_monsterFrameCount = 0;

					_img->setFrameX(_monsterFrameIndex++);
					if (_monsterFrameIndex > _img->getMaxFrameX())
					{
						_isMove = false;
						_isFading = false;
						_monsterFrameIndex = 0;
						startTime = 0;
						lightningCount += 1;
					}
				}
			}
		}
	}
	//등장이 끝나면 
	else if (!_isMove && !_isReady)
	{

		if (!_isCastLightning)
		{
			_enemyAngle = getAngle(_positionX, _positionY, _player.getPositionX(), _player.getPositionY());
		}
		else if (_isCastLightning)
		{
			_enemyAngle = getAngle(_positionX, _positionY, _positionX, _positionY + 1);
		}

		//다시 밝아지고
		if (_alpha2 > 0)
		{
			_alpha2 -= 10;
		}
		//다 밝아지면 공격시작
		else if (_alpha2 < 0)
		{
			_alpha2 = 0;
			_isCastPrep = true;
		}

		if (_castPrepCount >= 140)
		{
			_isAttack = true;
			_isCastPrep = false;
			_castPrepCount = 0;
		}

		if (_isAttack)
		{
			if (_isCastLightning)
			{
				int lightningCastPositionY = _positionY + 50;
				lightningTime += 20;
				if (lightningTime == 100)
				{
					lightning[0] = RectMakeCenter(_positionX, lightningCastPositionY, 249, 66);
				}
				else if (lightningTime == 700)
				{
					lightning[0] = RectMakeCenter(_positionX, lightningCastPositionY, 0, 0);
				}
				if (lightningTime == 200)
				{
					lightning[1] = RectMakeCenter(_positionX, lightningCastPositionY + _lightningImg->getFrameHeight(), 249, 66);
				}
				else if (lightningTime == 800)
				{
					lightning[1] = RectMakeCenter(_positionX, lightningCastPositionY + _lightningImg->getFrameHeight(), 0, 0);
				}
				if (lightningTime == 300)
				{
					lightning[2] = RectMakeCenter(_positionX, lightningCastPositionY + _lightningImg->getFrameHeight() * 2, 249, 66);
				}
				else if (lightningTime == 900)
				{
					lightning[2] = RectMakeCenter(_positionX, lightningCastPositionY + _lightningImg->getFrameHeight() * 2, 0, 0);
				}
				if (lightningTime == 400)
				{
					lightning[3] = RectMakeCenter(_positionX, lightningCastPositionY + _lightningImg->getFrameHeight() * 3, 249, 66);
				}
				else if (lightningTime == 1000)
				{
					lightning[3] = RectMakeCenter(_positionX, lightningCastPositionY + _lightningImg->getFrameHeight() * 3, 0, 0);
				}
				if (lightningTime == 500)
				{
					lightning[4] = RectMakeCenter(_positionX, lightningCastPositionY + _lightningImg->getFrameHeight() * 4, 249, 66);
				}
				else if (lightningTime == 1100)
				{
					lightning[4] = RectMakeCenter(_positionX, lightningCastPositionY + _lightningImg->getFrameHeight() * 4, 0, 0);
				}
				if (lightningTime == 600)
				{
					lightning[5] = RectMakeCenter(_positionX, lightningCastPositionY + _lightningImg->getFrameHeight() * 5, 249, 66);
				}
				else if (lightningTime == 1200)
				{
					lightning[5] = RectMakeCenter(_positionX, lightningCastPositionY + _lightningImg->getFrameHeight() * 5, 0, 0);
				}

				if (lightningTime == 1400)
				{
					_isAttack = false;
					_isReady = true;
					lightningTime = 0;
					_isCastLightning = false;
				}
			}

			else if (_isCastBolt)
			{
				_boltTime += 1;
				if (_boltTime <= 2)
				{
					EQUIPMENTMANAGER->FireEquipment(EQUIPMENT_KIND_AGAHNIMBOLT, PointMake(_positionX, _positionY),
						getAngle(_positionX, _positionY, _player.getPositionCenterX(), _player.getPositionCenterY()), 5, false);
				}

				if (_boltTime >= 200)
				{
					_isAttack = false;
					_isReady = true;
					_boltTime = 0;
					_isCastBolt = false;
				}
			}
			else if (_isCastBubble)
			{
				_boltTime += 1;
				if (_boltTime <= 2)
				{
					EQUIPMENTMANAGER->FireEquipment(EQUIPMENT_KIND_AGAHNIMBUBBLE, PointMake(_positionX, _positionY),
						getAngle(_positionX, _positionY, _player.getPositionCenterX(), _player.getPositionCenterY()), 5, false);
				}

				if (_boltTime >= 200)
				{
					_isAttack = false;
					_isReady = true;
					_boltTime = 0;
					_isCastBubble = false;
				}
			}
			else
			{
				actionTime += 2;
				if (actionTime >= 200)
				{
					_isAttack = false;
					_isReady = true;
					actionTime = 0;

				}
			}

		}
	}
}
