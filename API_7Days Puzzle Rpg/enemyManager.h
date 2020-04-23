#pragma once
#include"gameNode.h"
#include"bullet.h"
#include"minion.h"

class rocket;//전방선언

class enemyManager :  public gameNode
{

private : 
	typedef vector<enemy*> vEnemy;
	typedef vector<enemy*>::iterator viEnemy;

private:

	vEnemy _vMinion;
	viEnemy _viMinion;
	bullet* _bullet;


	rocket* _rocket;

public:
	enemyManager();
	~enemyManager();

	HRESULT init();
	void release();
	void update();
	void render();
	//미니언 셋팅
	void setMinion();
	//부하들 총알발사
	void minionBulletFire();

	//미니언 벡터,반복자
	vEnemy getVMinion() { return _vMinion; }
	viEnemy getViMinion() { return _viMinion; }


	//삭제
	void removeMinion(int arrNum);
	//충돌
	void collision();
	//로켓과 공유함.
	void setRocketmemoryLink(rocket* rocket) { _rocket = rocket; }


};

