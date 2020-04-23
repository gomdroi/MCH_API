#pragma once
#include"gameNode.h"
#include"bullet.h"
#include"minion.h"
/*
	에너미를 인클루드 해서 해도 되고
	미니언 클래스 하나 만들어서 해도됨.

*/
class rocket;

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


	void removeMinion(int arrNum);

	void collision();

	void setRocketMemoryLick(rocket* rocket) { _rocket = rocket; }


};

