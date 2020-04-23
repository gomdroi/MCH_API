#include "stdafx.h"
#include "rocket.h"
#include"enemyManager.h"
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
	collision();


	//���α׷���
	_hpBar->setX(m_player->getX());
	_hpBar->setY(m_player->getY() - 15);
	_hpBar->setGauge(_currentHP, _maxHP);
	_hpBar->update();

	//���̺�
	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		//itoa => ������ ���ڷ�
		char temp[64];
		vector<string> vStr;
		vStr.push_back(_itoa(_currentHP, temp, 10));
		vStr.push_back(_itoa(_maxHP, temp, 10));
		vStr.push_back(_itoa(m_player->getX(), temp, 10));
		vStr.push_back(_itoa(m_player->getY(), temp, 10));

		TXTDATA->txtSave("save/rocketData.txt", vStr);
	}

	if (KEYMANAGER->isOnceKeyDown('L'))
	{
		vector<string> vStr(TXTDATA->txtLoad("save/rocketData.txt"));
		//stoi :  ���ڿ��� ������
	
		_currentHP = stoi(vStr[0]);
		_maxHP = stoi(vStr[1]);
		m_player->setX(stoi(vStr[2]));
		m_player->setY(stoi(vStr[3]));

		//atoi�� string�� �����ϰ� ������
		/*_currentHP = atoi(vStr[0].c_str());
		_maxHP = atoi(vStr[1].c_str());
		m_player->setX(atoi(vStr[2].c_str()));
		m_player->setY(atoi(vStr[3].c_str()));*/

	}


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

void rocket::collision()
{
	for (int i = 0; i < _sonicboom->getVBullet().size(); i++)
	{
		for (int j = 0; j < _em->getVMinion().size(); j++)
		{
			RECT rc;
			if (IntersectRect(&rc, &_sonicboom->getVBullet()[i].rc,
				&_em->getVMinion()[j]->getRect()))
			{
				
				_sonicboom->removeSonicBoom(i);
				_em->removeMinion(j);
				break;
			}
		}
	}
}

void rocket::hitDamage(float damage)
{
	_currentHP -= damage;
}
