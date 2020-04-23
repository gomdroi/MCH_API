#pragma once
#include "gameNode.h"
#include"bullet.h"
#include "progressBar.h"

//상호참조용 적 매니져 클래스 전방선언
class enemyManager;
class rocket : public gameNode
{
private:
	image * m_player;
	missile* _missile;
	sonicBoom* _sonicboom;

	//프로그래스 바
	progressBar* _hpBar;
	float _maxHP;
	float _currentHP;
	
	enemyManager* _em;	//적 매니져 클래스 선언만
public:
	rocket();
	~rocket();

	HRESULT init();
	void release();
	void update();
	void render();




	//미사일 삭제
	void removeMissile(int arrNum);
	//충돌함수
	void collision();
	//피깍는 함수
	void hitDamage(float damage);

	//미사일 M1 가져오기
	sonicBoom* getMissile() { return _sonicboom; }

	//로켓 이미지 가져오기
	image* getRocketImage() { return m_player; }

	//상호참조 하기 위한 적 매니져 주소값 공유
	void setEnemyMemoryLick(enemyManager* em) { _em = em; }
};

