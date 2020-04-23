#include "stdafx.h"
#include "rocket.h"
#include"enemyManager.h"
//cpp�� ��Ŭ��� �ؾ� ������ ��ȣ������ �̷����.
rocket::rocket()
{
}

rocket::~rocket()
{
}

HRESULT rocket::init()
{
	//�����÷��̾� �ʱ�ȭ �� ��ġ����
	m_player = IMAGEMANAGER->addImage("rocket", "images/rocket.bmp", WINSIZEX / 2, WINSIZEY - 200, 52, 64, true, RGB(255, 0, 255));
	
	//�̻��� Ŭ���� �ʱ�ȭ
	_missile = new missile;
	_missile->init(10, 1000);
	
	_sonicboom = new sonicBoom;
	_sonicboom->init(10, 1000);


	//ü�¹� �ʱ�ȭ
	_maxHP = _currentHP = 100;

	//ü�¹�Ŭ����
	_hpBar = new progressBar;
	_hpBar->init("images/progressBarFront.bmp", "images/progressBarBack.bmp", 0, 0, 50, 10);
	_hpBar->setGauge(_currentHP, _maxHP);

	return S_OK;
}

void rocket::release()
{
	SAFE_DELETE(_missile);
	SAFE_DELETE(_sonicboom);
	SAFE_DELETE(_hpBar);
}

void rocket::update()
{
	//����
	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && m_player->getX() > 0)
	{
		m_player->setX(m_player->getX() - 5.0f);
	}

	//������
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && m_player->getX() + m_player->getWidth() < WINSIZEX)
	{
		m_player->setX(m_player->getX() + 5.0f);
	}

	//��
	if (KEYMANAGER->isStayKeyDown(VK_UP) && m_player->getY() > 0)
	{
		m_player->setY(m_player->getY() - 5.0f);
	}

	//����
	if (KEYMANAGER->isStayKeyDown(VK_DOWN) && m_player->getY() + m_player->getHeight() < WINSIZEY)
	{
		m_player->setY(m_player->getY() + 5.0f);
	}

	//�Ѿ� �߻�
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		_missile->fire(m_player->getX() + m_player->getWidth() / 2, m_player->getY() - 50);
	}
	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		_sonicboom->fire(m_player->getX() + m_player->getWidth() / 2, m_player->getY() - 50);
	}
	//�̻��� ������Ʈ
	_missile->update();
	_sonicboom->update();




	//���α׷���
	_hpBar->setX(m_player->getX());
	_hpBar->setY(m_player->getY() - 15);
	_hpBar->setGauge(_currentHP, _maxHP);
	_hpBar->update();

}

void rocket::render()
{
	m_player->render(getMemDC(), m_player->getX(), m_player->getY());

	//�̻��� ����
	_missile->render();
	_sonicboom->render();
	

	_hpBar->render();
}

void rocket::removeMissile(int arrNum)
{
	_sonicboom->removeSonicBoom(arrNum);
}

void rocket::hitDamage(float damage)
{
	_currentHP -= damage;
}

void rocket::collision()
{
	//���⸦ �ۼ��Ͻÿ�.(�̴Ͼ�� �浹ó���ؼ� ���ֱ�)

}


