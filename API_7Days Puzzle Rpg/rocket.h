#pragma once
#include "gameNode.h"
#include"bullet.h"
#include "progressBar.h"
/*

참고 :  헤더 파일이 많아질수록 컴파일 속도가 저하된다.

class A a.h에 있고 class B가 b.h에 있으면
B가 A를 쓰고 A가B를 사용하면
헤더꼬임 상태가 발생한다.

에이가 비의 데이터를 알기위해선 비의 구조를 알아야 하고 비의 헤더를 포함해야한다.
하지만 서로 인클루드를 하게 되면 상호참조 에러

이런 녀석을 해결위해 전방선언을 사용한다.
(장점 :  참조하려는 헤더파일에 변경이 생겨도 참조하는 헤더파일에서는
재컴파일이 이루어지지 않는다.)
단!!! 전방선언을 한다면 그 클래스 관력 객체는 반드시 포인터로 선언해야 한다.
포인터형이 아니라면 전방선언의 특징상 객체의 크기를 정확하게 파악하지 못한다.
(해당 클래스만 있는 것만 알고 있고 사이즈 모름)
*/

//cpp에 인클루드를 해야 상호참조가 된다.

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
	
	enemyManager* _em;	//클래스 선언
public:
	rocket();
	~rocket();

	HRESULT init();
	void release();
	void update();
	void render();


	void removeMissile(int arrNum);

	void hitDamage(float damage);

	void collision();
	

	/////////////
	//이미지 가져오자
	image* getRocketImage() { return m_player; }

	//소닉붐 가져와라
	sonicBoom* getMissile() { return _sonicboom; }

	//적과의 공유
	void setEnemyMemoryLink(enemyManager* em) { _em = em; }

	
};

