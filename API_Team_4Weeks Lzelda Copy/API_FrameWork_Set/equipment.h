#pragma once
#include "gameNode.h"
#include "enemySet.h"
enum OBJECT_TYPE;
enum EQUIPMENT_KIND;
class equipment : public gameNode
{
protected:
	image *_img;
	animation *_ani;
	RECT _rc;
	EQUIPMENT_KIND _kind;
	enemySet* _enemySet;
protected:
	float _positionX;
	float _positionY;
	float _positionZ;
	float _equipAngle;			//각도
	float _speed;			//속도
	float _range;			//사거리
	float _damage;			//데미지
	float _fireX, _fireY;		//쏘는지점
	float _gravity;		//중력
	int _startTime;
protected:
	bool _isActive; // 활성화
	bool _isKnockBack;
	bool _isFlick;
	bool _isBubblePop;

protected:
	OBJECT_TYPE _targetObjectType; // 적,플레이어,NPC,벽,풀 등등 오브젝트구분 타입

public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	RECT getRect() { return _rc; }
	void setRect(RECT rc) { _rc = rc; }

	bool getIsKnockBack() { return _isKnockBack; }
	void setIsKnockBack(bool isknockback) { _isKnockBack = isknockback; }

	void setIsFlick(bool isflick) { _isFlick = isflick; }

	void setIsBubblePop(bool isbubblepop) { _isBubblePop = isbubblepop; }

	int getStartTime() { return _startTime; }
	void setStartTime(int startTime) { _startTime = startTime; }

	image* getImage() { return _img; }
	void setImage(image* image) { _img = image; }

	animation* getAnimation() { return _ani; }
	void setAnimation(animation* animation) { _ani = animation; }

	OBJECT_TYPE getTargetObjectType() { return _targetObjectType; }
	void setTargetObjectType(OBJECT_TYPE targetObjectType) { _targetObjectType = targetObjectType; }

	EQUIPMENT_KIND getEquipKind() { return _kind; }
	void setEquipKind(EQUIPMENT_KIND kind) { _kind = kind; }

	float getFireX() { return _fireX; }
	void setFireX(float fireX) { _fireX = fireX; }

	float getFireY() { return _fireY; }
	void setFireY(float fireY) { _fireY = fireY; }

	bool getisActive() { return _isActive; }
	void setisActive(bool isActive) { _isActive = isActive; }

	float getPositionX() { return _positionX; }
	void setPositionX(float positionX) { _positionX = positionX; }

	float getPositionY() { return _positionY; }
	void setPositionY(float positionY) { _positionY = positionY; }

	float getPositionZ() { return _positionZ; }
	void setPositionZ(float positionZ) { _positionZ = positionZ; }

	float getEquipAngle() { return _equipAngle; }
	void setEquipAngle(float angle) { _equipAngle = angle; }

	float getSpeed() { return _speed; }
	void setSpeed(float speed) { _speed = speed; }

	float getRange() { return _range; }
	void setRange(float range) { _range = range; }

	float getDamage() { return _damage; }
	void setDamage(float damage) { _range = damage; }

	void aniStart() { _ani->start(); };
	void aniStop() { _ani->stop(); };
	bool aniPlay() { _ani->isPlay(); }

	virtual void setInterectObjKind(INTERACTIONOBJECT) { return; };

	equipment();
	~equipment();
};

