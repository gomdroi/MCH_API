#pragma once
#include "gameNode.h"
#include"bullet.h"
#include "progressBar.h"
/*

���� :  ��� ������ ���������� ������ �ӵ��� ���ϵȴ�.

class A a.h�� �ְ� class B�� b.h�� ������
B�� A�� ���� A��B�� ����ϸ�
������� ���°� �߻��Ѵ�.

���̰� ���� �����͸� �˱����ؼ� ���� ������ �˾ƾ� �ϰ� ���� ����� �����ؾ��Ѵ�.
������ ���� ��Ŭ��带 �ϰ� �Ǹ� ��ȣ���� ����

�̷� �༮�� �ذ����� ���漱���� ����Ѵ�.
(���� :  �����Ϸ��� ������Ͽ� ������ ���ܵ� �����ϴ� ������Ͽ�����
���������� �̷������ �ʴ´�.)
��!!! ���漱���� �Ѵٸ� �� Ŭ���� ���� ��ü�� �ݵ�� �����ͷ� �����ؾ� �Ѵ�.
���������� �ƴ϶�� ���漱���� Ư¡�� ��ü�� ũ�⸦ ��Ȯ�ϰ� �ľ����� ���Ѵ�.
(�ش� Ŭ������ �ִ� �͸� �˰� �ְ� ������ ��)
*/

//cpp�� ��Ŭ��带 �ؾ� ��ȣ������ �ȴ�.

class enemyManager;
class rocket : public gameNode
{
private:
	image * m_player;
	missile* _missile;
	sonicBoom* _sonicboom;

	//���α׷��� ��
	progressBar* _hpBar;
	float _maxHP;
	float _currentHP;
	
	enemyManager* _em;	//Ŭ���� ����
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
	//�̹��� ��������
	image* getRocketImage() { return m_player; }

	//�Ҵк� �����Ͷ�
	sonicBoom* getMissile() { return _sonicboom; }

	//������ ����
	void setEnemyMemoryLink(enemyManager* em) { _em = em; }

	
};

