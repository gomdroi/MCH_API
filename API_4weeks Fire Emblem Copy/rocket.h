#pragma once
#include "gameNode.h"
#include"bullet.h"
#include "progressBar.h"

//��ȣ������ �� �Ŵ��� Ŭ���� ���漱��
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
	
	enemyManager* _em;	//�� �Ŵ��� Ŭ���� ����
public:
	rocket();
	~rocket();

	HRESULT init();
	void release();
	void update();
	void render();




	//�̻��� ����
	void removeMissile(int arrNum);
	//�浹�Լ�
	void collision();
	//�Ǳ�� �Լ�
	void hitDamage(float damage);

	//�̻��� M1 ��������
	sonicBoom* getMissile() { return _sonicboom; }

	//���� �̹��� ��������
	image* getRocketImage() { return m_player; }

	//��ȣ���� �ϱ� ���� �� �Ŵ��� �ּҰ� ����
	void setEnemyMemoryLick(enemyManager* em) { _em = em; }
};

