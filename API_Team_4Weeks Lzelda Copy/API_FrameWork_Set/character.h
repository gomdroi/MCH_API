#pragma once
#include "gameNode.h"
#include "enemySet.h"
enum DIRECTION_STATE;
enum ANIMATION_STATE;
enum USED_EQUIPMENT;
class character : public gameNode
{
protected:

protected:
	image *_img;					//캐릭터의 이미지
	RECT _Rc;						//캐릭터의 랙트
	RECT _atkRc;					//공격범위 랙트
	enemySet* _enemySet;			//
protected://정수형
	float _positionX;
	float _positionY;
	float _positionCenterX;
	float _positionCenterY;
	float _angle;			//캐릭터의 각도
	float _speed;			//캐릭터의 속도
	float _range;					//각종 무기의 사거리(활,부메랑)
	float _maxHP;			// 최대체력
	float _curHP;			// 현재체력
	float _maxMP;			// 최대마력
	float _curMP;			// 현재마력
	float _knockbackForce;	// 넉백거리
	float _knockbackTime;
	float nowAngle;

	int _hitStartPositionX; //
	int _hitStartPositionY; //
	int _playerAlphaValue;		//플레이어 이미지 알파값
	int _playerAlphaTime;		//알파 시간
	int _startTime;			//
	int _money;				// 돈
	int _arrow;				// 화살갯수
	int _boom;				// 폭탄갯수
	int _currentFrameX;				//프레임이미지 x의 값
	int _currentFrameY;				//프레임이미지 y의 값
protected://bool형
	bool _isHit;					//피격중인가
	bool _isAttack;					//공격중인가
	bool _isKnockBack;				//넉백중인가
	bool _isArrow;					//활쏘는중인가
	bool _isBoomerang;				//부메랑던지는중인가?
	bool _isStay;					//움직임이 없을시 상태값
	bool _isDie;					//생사여부 상태값
	bool _isFrameImage;				//캐릭터 프레임이미지 여부
	bool _isAlpha;
	bool _isMove;
protected:
	int _width;						//플레이어 넓이
	int _height;					//플레이어 높이
	int _count;						//프레임을 돌리기 위한 변수
	int _playerDir;					//플레이어방향
protected:
	DIRECTION_STATE _playerDirection;					//플레이어 방향(enum)
	ANIMATION_STATE _curAnimationState; // 플레이어 현재 애니메이션 변수
	ANIMATION_STATE _prevAnimationState;// 플레이어 이전 애니메이션 변수
	USED_EQUIPMENT _usedEquipment;
public:
	character();
	~character();

	DIRECTION_STATE getDirState() { return _playerDirection; }
	void setDirState(DIRECTION_STATE dirState) { _playerDirection = dirState; }

	USED_EQUIPMENT getUsedEquip() { return _usedEquipment; }
	void setUsedEquip(USED_EQUIPMENT usedEquip) { _usedEquipment = usedEquip; }

	ANIMATION_STATE getCurAnimationState() { return _curAnimationState; }
	void setCurAnimationState(ANIMATION_STATE curAnimationState) { _curAnimationState = _curAnimationState; }

	ANIMATION_STATE getPrevAnimationState() { return _prevAnimationState; }
	void setPrevAnimationState(ANIMATION_STATE prevAnimationState) { _prevAnimationState = _prevAnimationState; }

	virtual HRESULT init(const char* imageName, POINT position);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc, bool isRectRend);

	//character클래스에 있는 변수
	image** getImage() { return &_img; };
	void setImage(string imgName);

	POINT getPosition() { return POINT{ (long)_positionX,(long)_positionY }; };
	void setPosition(int x, int y) { _positionX = x, _positionY = y; };

	int getPositionX() { return _positionX; };
	void setPositionX(int x) { _positionX = x; };

	int getPositionY() { return _positionY; };
	void setPositionY(int y) { _positionY = y; };

	int getPositionCenterX() { return _positionCenterX; };
	void setPostionCenterX(int x) { _positionCenterX = x; };

	int getPositionCenterY() { return _positionCenterY; };
	void setPostionCenterY(int y) { _positionCenterY = y; };

	RECT getRect() { return _Rc; };
	void setRect(RECT rc) { _Rc = rc; };

	RECT getAttackRect() { return _atkRc; };
	void setAttackRect(RECT rc) { _atkRc = rc; };

	float getAngle() { return _angle; };
	void setAngle(int angle) {
		if (angle <= 360)_angle = angle;
		else _angle = angle - 360;
	};

	float getSpeed() { return _speed; };
	void setSpeed(float speed) { _speed = speed; };

	int getCurrentFrameX() { return _currentFrameX; };
	void setCurrentFrameX(int currentFrameX) { _currentFrameX = currentFrameX; };

	int getCurrentFrameY() { return _currentFrameY; };
	void setCurrentFrameY(int currentFrameY) { _currentFrameY = currentFrameY; };

	bool getIsFrameImage() { return _isFrameImage; };
	void getIsFrameImage(bool inFrameImage) { _isFrameImage = inFrameImage; };

	bool getIsStay() { return _isStay; };
	void setIsStay(bool isStay) { _isStay = isStay; };

	bool getIsDie() { return _isDie; };
	void setIsDie(bool isDie) { _isDie = isDie; };

	float getMaxHP() { return _maxHP; }
	void setMaxHP(float maxHP) { _maxHP = maxHP; }

	float getCurHP() { return _curHP; }
	void setCurHP(float curHP) { _curHP = curHP; }

	float getMaxMP() { return _maxMP; }
	void setMaxMP(float maxMP) { _maxMP = maxMP; }

	float getCurMP() { return _curMP; }
	void setCurMP(float curMP) { _curMP = curMP; }

	bool getIsHit() { return _isHit; };
	void setIsHit(bool isHit) { _isHit = isHit; };

	bool getIsAttack() { return _isAttack; };
	void setIsAttack(bool isAttack) { _isAttack = isAttack; };

};