#include "stdafx.h"
#include "enemySet.h"


enemySet::enemySet()
{
}


enemySet::~enemySet()
{
}

HRESULT enemySet::init()
{
	_positionX = 0;					//몬스터의 중간위치
	_positionY = 0;					//몬스터의 중간위치
	_positionZ = 0;					//몬스터의 높이

	_currentFrameX = 0;				//프레임이미지 x의 값
	_currentFrameY = 0;				//프레임이미지 y의 값

	_enemyAngle = 0;				//몬스터의 각도
	_enemySpeed = 0;				//몬스터의 속도

	_rcAlpha = 255;

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
	_isStay = false;					//움직임이 없을시 상태값
	_isDie = false;					//생사여부 상태값

	return S_OK;
}

void enemySet::setEnemy()
{
}

void enemySet::release()
{
}

void enemySet::update()
{

}

void enemySet::render(HDC hdc)
{

}

void enemySet::animation()
{
}
