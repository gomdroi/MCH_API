#include "stdafx.h"
#include "monsters.h"


HRESULT monsters::init()
{
	
	m_image = IMAGEMANAGER->findImage("베스탈");
	/*m_stickedTilePosition = nullptr;*/
	m_position = { 0,0 };
	m_collider = RectMakeCenter(m_position.x, m_position.y,32*1.5,32*1.5);
	/*monsterName = "바보";*/
	sprintf_s(monsterName, "바보");
	m_monsterMaxHp = 0;
	m_monsterCurHp = 0;
	m_monsterAtk = 0;
	m_monsterDef = 0;
	m_monsterExp = 0;
	m_monsterGold = 0;
	
	monsterLevel = LEVEL_0;
	m_direction = MONSTER_IDLE;
	m_monsterIndex = 0;
	m_monsterCount = 0;
	isInvenPlaced = false;
	isMapPlaced = false;
	return S_OK;
}

void monsters::release()
{
		
}

void monsters::update()
{
	/*m_position.x = (*m_stickedTilePosition).x;
	m_position.y = (*m_stickedTilePosition).y;*/

	animation();
}

void monsters::render()
{
	m_collider = RectMakeCenter(m_position.x, m_position.y, 32 * 1.5, 32 * 1.5);
	m_image->frameRender(getMemDC(), m_collider.left, m_collider.top, m_image->getFrameX(), m_image->getFrameY());

	if (PtInRect(&m_collider, m_ptMouse))
	{
		m_image->frameRender(getMemDC(), 10, 160, m_image->getFrameX(), m_image->getFrameY());
		
		HFONT myFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "휴먼매직체");
		HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
		SetBkMode(getMemDC(), TRANSPARENT);

		sprintf_s(monsterHp, "H  P : %0.f / %0.f", m_monsterCurHp, m_monsterMaxHp);
		TextOut(getMemDC(), 70, 160, monsterHp, strlen(monsterHp));
		sprintf_s(monsterAtk, "ATK : %0.f", m_monsterAtk);
		TextOut(getMemDC(), 70, 180, monsterAtk, strlen(monsterAtk));
		sprintf_s(monsterDef, "DEF : %0.f", m_monsterDef);
		TextOut(getMemDC(), 70, 200, monsterDef, strlen(monsterDef));
		sprintf_s(monsterExpGold, "EXP : %0.f    GOLD : %0.f", m_monsterExp, m_monsterGold);
		TextOut(getMemDC(), 20, 240, monsterExpGold, strlen(monsterExpGold));
		/*wsprintf(cMonsterName, "LEVEL : %d", m_player->getLevel());*/
		TextOut(getMemDC(), 70, 140, monsterName, strlen(monsterName));

		SelectObject(getMemDC(), oldFont);
		DeleteObject(myFont);

	}
}

void monsters::animation()
{
	switch (m_direction)
	{
	case MONSTER_IDLE:

		m_monsterCount++;
		m_image->setFrameY(0);
		if (m_monsterCount % 4 == 0)
		{
			m_monsterCount = 0;
			m_image->setFrameX(m_image->getMaxFrameX() - m_monsterIndex++);
			if (m_monsterIndex > m_image->getMaxFrameX()) m_monsterIndex = 0;
		}

		break;
	case MONSTER_UP:
		
		m_monsterCount++;
		m_image->setFrameY(4);
		if (m_monsterCount % 4 == 0)
		{
			m_monsterCount = 0;
			m_image->setFrameX(m_image->getMaxFrameX() - m_monsterIndex++);
			if (m_monsterIndex > m_image->getMaxFrameX()) m_monsterIndex = 0;
		}
		
		break;
	case MONSTER_DOWN:
		
			m_monsterCount++;
			m_image->setFrameY(3);
			if (m_monsterCount % 4 == 0)
			{
				m_monsterCount = 0;
				m_image->setFrameX(m_image->getMaxFrameX() - m_monsterIndex++);
				if (m_monsterIndex > m_image->getMaxFrameX()) m_monsterIndex = 0;
			}
		
		break;
	case MONSTER_LEFT:
		
			m_monsterCount++;
			m_image->setFrameY(1);
			if (m_monsterCount % 4 == 0)
			{
				m_monsterCount = 0;
				m_image->setFrameX(m_image->getMaxFrameX() - m_monsterIndex++);
				if (m_monsterIndex > m_image->getMaxFrameX()) m_monsterIndex = 0;
			}
		
		break;
	case MONSTER_RIGHT:
	
			m_monsterCount++;
			m_image->setFrameY(2);
			if (m_monsterCount % 4 == 0)
			{
				m_monsterCount = 0;
				m_image->setFrameX(m_image->getMaxFrameX() - m_monsterIndex++);
				if (m_monsterIndex > m_image->getMaxFrameX()) m_monsterIndex = 0;
			}
		
		break;
	default:
		break;
	}
}

void monsters::setMonster(MONSTERTYPE _monsterType)
{
	monsterType = _monsterType;
	switch (_monsterType)
	{
	case TYPE_NONE:
		break;
	case TYPE_ENTOMBED:
		m_image = new image;
		m_image->init("images/Entombed.bmp", 192, 237,4,5,true,RGB(255,0,255));
		monsterLevel = LEVEL_1;
		/*monsterName = "ENTOMBED";*/
		sprintf_s(monsterName, "ENTOMBED");
		m_monsterMaxHp = 20;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 3;
		m_monsterDef = 1;
		m_monsterExp = 1;
		m_monsterGold = 5;

		break;
	case TYPE_MOGALL:
		m_image = new image;
		m_image->init("images/Mogall.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_1;
		/*monsterName = "MOGALL";*/
		sprintf_s(monsterName, "MOGALL");
		m_monsterMaxHp = 16;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 5;
		m_monsterDef = 0;
		m_monsterExp = 2;
		m_monsterGold = 7;

		break;
	case TYPE_DEIMOS:
		m_image = new image;
		m_image->init("images/Deimos.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_2;
		/*monsterName = "DEIMOS";*/
		sprintf_s(monsterName, "DEIMOS");
		m_monsterMaxHp = 40;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 4;
		m_monsterDef = 2;
		m_monsterExp = 3;
		m_monsterGold = 10;

		break;
	case TYPE_REVENANT:
		m_image = new image;
		m_image->init("images/Revenant.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_2;
		/*monsterName = "REVENANT";*/
		sprintf_s(monsterName, "REVENANT");
		m_monsterMaxHp = 35;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 5;
		m_monsterDef = 2;
		m_monsterExp = 5;
		m_monsterGold = 15;

		break;
	case TYPE_GUARDIAN:
		m_image = new image;
		m_image->init("images/Guardian.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_3;
		/*monsterName = "GUARDIAN";*/
		sprintf_s(monsterName, "GUARDIAN");
		m_monsterMaxHp = 60;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 10;
		m_monsterDef = 5;
		m_monsterExp = 9;
		m_monsterGold = 20;

		break;
	case TYPE_BARON:
		m_image = new image;
		m_image->init("images/Baron.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_3;
		/*monsterName = "BARON";*/
		sprintf_s(monsterName, "BARON");
		m_monsterMaxHp = 70;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 13;
		m_monsterDef = 5;
		m_monsterExp = 12;
		m_monsterGold = 40;

		break;
	case TYPE_BERKUT:
		m_image = new image;
		m_image->init("images/Berkut.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_4;
		/*monsterName = "BERKUT";*/
		sprintf_s(monsterName, "BERKUT");
		m_monsterMaxHp = 60;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 15;
		m_monsterDef = 3;
		m_monsterExp = 15;
		m_monsterGold = 40;

		break;
	case TYPE_TITAN:
		m_image = new image;
		m_image->init("images/Titan.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_4;
		/*monsterName = "TITAN";*/
		sprintf_s(monsterName, "TITAN");
		m_monsterMaxHp = 100;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 18;
		m_monsterDef = 2;
		m_monsterExp = 20;
		m_monsterGold = 30;

		break;
	case TYPE_VESTAL:
		m_image = new image;
		m_image->init("images/Vestal.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_5;
		/*monsterName = "VESTAL";*/
		sprintf_s(monsterName, "VESTAL");
		m_monsterMaxHp = 50;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 30;
		m_monsterDef = 1;
		m_monsterExp = 25;
		m_monsterGold = 0;

		break;
	case TYPE_DRAGON:
		m_image = new image;
		m_image->init("images/Dragon.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_6;
		/*monsterName = "DRAGON";*/
		sprintf_s(monsterName, "DRAGON");

		m_monsterMaxHp = 150;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 25;
		m_monsterDef = 5;
		m_monsterExp = 100;
		m_monsterGold = 100;

		break;
	case TYPE_JEDAH:
		m_image = new image;
		m_image->init("images/Jedah.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_5;
		/*monsterName = "DRAGON";*/
		sprintf_s(monsterName, "JEDAH");

		m_monsterMaxHp = 50;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 45;
		m_monsterDef = 4;
		m_monsterExp = 40;
		m_monsterGold = 80;
		break;
	case TYPE_DEATHGOYLE:
		m_image = new image;
		m_image->init("images/Deathgoyle.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_5;
		/*monsterName = "DRAGON";*/
		sprintf_s(monsterName, "DEATHGOYLE");

		m_monsterMaxHp = 80;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 40;
		m_monsterDef = 10;
		m_monsterExp = 40;
		m_monsterGold = 80;
		break;
	case TYPE_NUIBABA:
		m_image = new image;
		m_image->init("images/Nuibaba.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_6;
		/*monsterName = "DRAGON";*/
		sprintf_s(monsterName, "NUIBABA");

		m_monsterMaxHp = 120;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 60;
		m_monsterDef = 15;
		m_monsterExp = 50;
		m_monsterGold = 100;
		break;
	case TYPE_TOMBLORD:
		m_image = new image;
		m_image->init("images/TombLord.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_6;
		/*monsterName = "DRAGON";*/
		sprintf_s(monsterName, "TOMBLORD");

		m_monsterMaxHp = 200;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 45;
		m_monsterDef = 30;
		m_monsterExp = 80;
		m_monsterGold = 200;
		break;
	case TYPE_FELLGOD:
		m_image = new image;
		m_image->init("images/Fellgod.bmp", 192, 237, 4, 5, true, RGB(255, 0, 255));
		monsterLevel = LEVEL_7;
		/*monsterName = "DRAGON";*/
		sprintf_s(monsterName, "FELLGOD");

		m_monsterMaxHp = 500;
		m_monsterCurHp = m_monsterMaxHp;
		m_monsterAtk = 50;
		m_monsterDef = 30;
		m_monsterExp = 1000;
		m_monsterGold = 1000;
		break;
	default:
		break;
	}

	m_position = { 0,0 };
	m_collider = RectMakeCenter(m_collider.left, m_collider.top, 32 * 1.5, 32 * .15);
	m_direction = MONSTER_IDLE;
	m_monsterIndex = 0;
	m_monsterCount = 0;
}

monsters::monsters()
{
}


monsters::~monsters()
{
}
