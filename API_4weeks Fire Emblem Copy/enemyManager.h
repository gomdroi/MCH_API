#pragma once
#include"gameNode.h"
#include"bullet.h"
#include"minion.h"
/*
	���ʹ̸� ��Ŭ��� �ؼ� �ص� �ǰ�
	�̴Ͼ� Ŭ���� �ϳ� ���� �ص���.

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
	//�̴Ͼ� ����
	void setMinion();
	//���ϵ� �Ѿ˹߻�
	void minionBulletFire();

	//�̴Ͼ� ����,�ݺ���
	vEnemy getVMinion() { return _vMinion; }
	viEnemy getViMinion() { return _viMinion; }


	void removeMinion(int arrNum);

	void collision();

	void setRocketMemoryLick(rocket* rocket) { _rocket = rocket; }


};

