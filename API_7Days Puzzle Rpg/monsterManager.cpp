#include "stdafx.h"
#include "monsterManager.h"
#include "monsters.h"

HRESULT monsterManager::init()
{
	IMAGEMANAGER->addFrameImage("베르쿠트", "images/Berkut.bmp", 128 * 1.5, 158 * 1.5, 4, 5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("바론", "images/Baron.bmp", 128 * 1.5, 158 * 1.5, 4, 5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("데이모스", "images/Deimos.bmp", 128 * 1.5, 158 * 1.5, 4, 5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("드래곤", "images/Dragon.bmp", 128 * 1.5, 158 * 1.5, 4, 5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("엔툼드", "images/Entombed.bmp", 128 * 1.5, 158 * 1.5, 4, 5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("가디언", "images/Guardian.bmp", 128 * 1.5, 158 * 1.5, 4, 5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("모갈", "images/Mogall.bmp", 128 * 1.5, 158 * 1.5, 4, 5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("레버넌트", "images/Revenant.bmp", 128 * 1.5, 158 * 1.5, 4, 5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("타이탄", "images/Titan.bmp", 128 * 1.5, 158 * 1.5, 4, 5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("베스탈", "images/Vestal.bmp", 128 * 1.5, 158 * 1.5, 4, 5, true, RGB(255, 0, 255));

	m_totalMonster = 0;
	return S_OK;
}

HRESULT monsterManager::init(STAGE _stage,int _monsterplus)
{
	
	m_stage = _stage;
	

	//지정 수 많큼 몬스터를 세팅하고
	for (int i = 0; i < _monsterplus; i++)
	{
		monsters* monster = new monsters;
		monster->init();
		monster->setMonsterNumber(m_totalMonster + i);

		//몬스터 설정해준다
		switch (m_stage)
		{
		case STAGE_NONE:
			break;
		case STAGE_ONE:
			if (i < 3)
			{
				monster->setMonster(TYPE_ENTOMBED);
			}
			else
			{
				monster->setMonster(TYPE_MOGALL);
			}
			break;
		case STAGE_TWO:
			if (i < 1)
			{
				monster->setMonster(TYPE_MOGALL);
			}
			else if (i == 1)
			{
				monster->setMonster(TYPE_DEIMOS);
			}
			else
			{
				monster->setMonster(TYPE_REVENANT);
			}
			break;
		case STAGE_THREE:
			if (i < 2)
			{
				monster->setMonster(TYPE_DEIMOS);
			}
			else if (i == 2 || i == 3)
			{
				monster->setMonster(TYPE_REVENANT);
			}
			else
			{
				monster->setMonster(TYPE_GUARDIAN);
			}

			break;
		case STAGE_FOUR:
			if (i == 0)
			{
				monster->setMonster(TYPE_REVENANT);
			}
			else
			{
				monster->setMonster(TYPE_GUARDIAN);
			}
			break;
		case STAGE_FIVE:
			if (i == 0)
			{
				monster->setMonster(TYPE_REVENANT);
			}
			else if (i == 1 || i == 2)
			{
				monster->setMonster(TYPE_GUARDIAN);
			}
			else
			{
				monster->setMonster(TYPE_BARON);
			}
			break;
		case STAGE_SIX:
			if (i < 3)
			{
				monster->setMonster(TYPE_GUARDIAN);
			}
			else if (i == 3 || i == 4)
			{
				monster->setMonster(TYPE_BARON);
			}
			else
			{
				monster->setMonster(TYPE_BERKUT);
			}
			break;
		case STAGE_SEVEN:
			if (i < 2)
			{
				monster->setMonster(TYPE_BERKUT);
			}
			else if (i == 2)
			{
				monster->setMonster(TYPE_TITAN);
			}
			else
			{
				monster->setMonster(TYPE_VESTAL);
			}

			break;
		case STAGE_EIGHT:
			if (i < 2)
			{
				monster->setMonster(TYPE_TITAN);
			}
			else if (i == 2 || i == 3)
			{
				monster->setMonster(TYPE_VESTAL);
			}
			else
			{
				monster->setMonster(TYPE_DRAGON);
			}

			break;
		case STAGE_NINE:
			if (i < 2)
			{
				monster->setMonster(TYPE_JEDAH);
			}
			else if (i == 2 || i == 3 || i == 4)
			{
				monster->setMonster(TYPE_DEATHGOYLE);
			}
			else
			{
				monster->setMonster(TYPE_NUIBABA);
			}
			break;
		case STAGE_TEN:
			if (i < 3)
			{
				monster->setMonster(TYPE_DEATHGOYLE);
			}
			else if (i == 3 || i == 4)
			{
				monster->setMonster(TYPE_NUIBABA);
			}
			else if (i == 5 || i == 6||i==7)
			{
				monster->setMonster(TYPE_TOMBLORD);
			}
			else
			{
				monster->setMonster(TYPE_FELLGOD);
				monster->setPosition(PointMake(510, 510));
			}
			break;
		default:
			break;
		}

		m_MonsterVector.push_back(monster);
	}
	m_totalMonster += _monsterplus;
	
	
	return S_OK;
}

void monsterManager::release()
{
	vector<monsters*>::iterator mIter = m_MonsterVector.begin();
	for (mIter; mIter != m_MonsterVector.end();)
	{
		SAFE_DELETE((*mIter));
		mIter = m_MonsterVector.erase(mIter);
	}

}

void monsterManager::update()
{
	for (int i = 0; i < m_MonsterVector.size(); i++)
	{
		m_MonsterVector[i]->update();
	}
}

void monsterManager::render()
{
	for (int i = 0; i < m_MonsterVector.size(); i++)
	{
		m_MonsterVector[i]->render();
	}
}

monsterManager::monsterManager()
{

}


monsterManager::~monsterManager()
{
}
