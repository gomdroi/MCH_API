#include "stdafx.h"
#include "player.h"
#include "tile.h"
#include "monsters.h"
#include "events.h"
#include "item.h"


HRESULT player::init()
{
	m_image = IMAGEMANAGER->addFrameImage("플레이어", "images/Celica.bmp", 128*1.5, 159*1.5, 4, 5, true, RGB(255, 0, 255));
	m_startPosition = { 380,40 };
	m_position = m_startPosition;
	m_collider = RectMakeCenter(m_position.x, m_position.y, 32*1.5, 32*1.5);
	moveLength = 0.0f;
	m_playerMaxHp = 50;
	m_playerCurHp = m_playerMaxHp;
	m_playerAtk = 5;
	m_playerDef = 1;
	m_playerExp = 0;
	m_playerGold = 0;
	m_playerLevel = 1;
	m_playerExpMax = 2;
	isBattle = false;

	m_poisonClawCount = 0;
	m_defShrineCount = 0;
	isDefShrineActive = false;
	isClassChange1 = false;
	isClassChange2 = false;

	isWin = false;

	startTime = 0;
	curTime = 0;

	m_direction = PLAYER_IDLE;
	m_playerSpeed = 1.0f;
	distance = 0.0f;


	stageCount = 1;

	return S_OK;
}

void player::release()
{

}

void player::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		int i = 0;
		for (i; i < TILEMANAGER->getTileMapVector().size(); i++)
		{
			if (TILEMANAGER->getTileMapVector()[i]->getIsForbid() == false)break;
		}
		if(i == 15)
		{
			isMoving = !isMoving;
		}
	
	}
	//물건사기
	buying();
	//플레이어를 진행 시키면
	if (isMoving)
	{
		if (isBattle == false)
		{
			move();
		}		
	}
	//전투판정 + 딜레이
	if (GetTickCount() - startTime > 500)
	{
		startTime = GetTickCount();
		battle();
	}
	//HP판정
	if (m_playerCurHp > m_playerMaxHp)
	{
		m_playerCurHp = m_playerMaxHp;
	}
	//이벤트 판정
	eventIncounter();
	//레벨업 판정
	dolevelUp();
	//플레이어가 골에 도달하면
	if (m_position.y > WINSIZEY)
	{
		isMoving = false;
		m_position = { 380,40 };

		//상점을 날리기위한 임시 구문
		for (int i = 0; i < EVENTMANAGER->getEventVector().size(); i++)
		{
			if(EVENTMANAGER->getEventVector()[i]->getEventType() == EVENT_VENDER && EVENTMANAGER->getEventVector()[i]->getIsMapPlaced())
			EVENTMANAGER->getEventVector()[i]->setPosition(PointMake(10000, 10000));
		}
				
		stageCount += 1;
		//물약 초기화
		ITEMMANAGER->getVenderVector()[11]->setIsPurchased(false);
		switch (stageCount)
		{
		case 2:
			MONSTERMANAGER->init(STAGE_TWO, 3);
			EVENTMANAGER->init(MONSTERMANAGER->getStage(), 3);
			TILEMANAGER->stageInit(stageCount);
			break;
		case 3:
			MONSTERMANAGER->init(STAGE_THREE, 5);
			EVENTMANAGER->init(MONSTERMANAGER->getStage(), 3);
			TILEMANAGER->stageInit(stageCount);
			break;
		case 4:
			MONSTERMANAGER->init(STAGE_FOUR, 2);
			EVENTMANAGER->init(MONSTERMANAGER->getStage(), 2);
			TILEMANAGER->stageInit(stageCount);
			break;
		case 5:
			MONSTERMANAGER->init(STAGE_FIVE, 4);
			EVENTMANAGER->init(MONSTERMANAGER->getStage(), 5);
			TILEMANAGER->stageInit(stageCount);
			break;
		case 6:
			MONSTERMANAGER->init(STAGE_SIX, 6);
			EVENTMANAGER->init(MONSTERMANAGER->getStage(), 4);
			TILEMANAGER->stageInit(stageCount);
			break;
		case 7:
			MONSTERMANAGER->init(STAGE_SEVEN, 4);
			EVENTMANAGER->init(MONSTERMANAGER->getStage(), 3);
			TILEMANAGER->stageInit(stageCount);
			break;
		case 8:
			MONSTERMANAGER->init(STAGE_EIGHT, 5);
			EVENTMANAGER->init(MONSTERMANAGER->getStage(), 2);
			TILEMANAGER->stageInit(stageCount);
			break;
		case 9:
			MONSTERMANAGER->init(STAGE_NINE, 6);
			EVENTMANAGER->init(MONSTERMANAGER->getStage(), 3);
			TILEMANAGER->stageInit(stageCount);
			break;
		case 10:
			MONSTERMANAGER->init(STAGE_TEN, 9);
			EVENTMANAGER->init(MONSTERMANAGER->getStage(), 4);
			TILEMANAGER->stageInit(stageCount);
			break;
		default:
			break;
		}
		

	}
	else if (m_position.x == 510 && m_position.y == 530 && stageCount == 10)
	{
		isWin = true;
	}
	animation();
}

void player::render()
{
	m_collider = RectMakeCenter(m_position.x, m_position.y, 32*1.5, 32*1.5);
	m_image->frameRender(getMemDC(), m_collider.left, m_collider.top, m_image->getFrameX(), m_image->getFrameY());
	m_image->frameRender(getMemDC(), 10,30, m_image->getFrameX(), m_image->getFrameY());

	HFONT myFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "휴먼매직체");
	HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
	SetBkMode(getMemDC(), TRANSPARENT);

	sprintf_s(playerHp, "H  P : %0.f / %0.f", m_playerCurHp, m_playerMaxHp);
	TextOut(getMemDC(), 70, 30, playerHp, strlen(playerHp));
	sprintf_s(playerAtk, "ATK : %0.f", m_playerAtk);
	TextOut(getMemDC(), 70, 50, playerAtk, strlen(playerAtk));
	sprintf_s(playerDef, "DEF : %0.f", m_playerDef);
	TextOut(getMemDC(), 70, 70, playerDef, strlen(playerDef));
	sprintf_s(playerExpGold, "EXP : %0.f / %0.f   GOLD : %0.f", m_playerExp,m_playerExpMax, m_playerGold);
	TextOut(getMemDC(), 20, 100, playerExpGold, strlen(playerExpGold));
	wsprintf(playerLevel, "LEVEL : %d", m_playerLevel);
	TextOut(getMemDC(), 70, 5, playerLevel, strlen(playerLevel));

	SelectObject(getMemDC(), oldFont);
	DeleteObject(myFont);
}

void player::animation()
{

	switch (m_direction)
	{
	case PLAYER_IDLE:

			m_playerCount++;
			m_image->setFrameY(0);
			if (m_playerCount % 4 == 0)
			{
				m_playerCount = 0;
				m_image->setFrameX(m_image->getMaxFrameX() - m_playerIndex++);
				if (m_playerIndex > m_image->getMaxFrameX()) m_playerIndex = 0;
			}
		
		break;
	case PLAYER_UP:
		
			m_playerCount++;
			m_image->setFrameY(4);
			if (m_playerCount % 4 == 0)
			{
				m_playerCount = 0;
				m_image->setFrameX(m_image->getMaxFrameX() - m_playerIndex++);
				if (m_playerIndex > m_image->getMaxFrameX()) m_playerIndex = 0;
			}
		
		break;
	case PLAYER_DOWN:
		
			m_playerCount++;
			m_image->setFrameY(3);
			if (m_playerCount % 4 == 0)
			{
				m_playerCount = 0;
				m_image->setFrameX(m_image->getMaxFrameX() - m_playerIndex++);
				if (m_playerIndex > m_image->getMaxFrameX()) m_playerIndex = 0;
			}
		
		break;
	case PLAYER_LEFT:
		
			m_playerCount++;
			m_image->setFrameY(1);
			if (m_playerCount % 4 == 0)
			{
				m_playerCount = 0;
				m_image->setFrameX(m_image->getMaxFrameX() - m_playerIndex++);
				if (m_playerIndex > m_image->getMaxFrameX()) m_playerIndex = 0;
			}
		
		break;
	case PLAYER_RIGHT:
		
			m_playerCount++;
			m_image->setFrameY(2);
			if (m_playerCount % 4 == 0)
			{
				m_playerCount = 0;
				m_image->setFrameX(m_image->getMaxFrameX() - m_playerIndex++);
				if (m_playerIndex > m_image->getMaxFrameX()) m_playerIndex = 0;
			}
		
		break;

	default:
		break;
	}

	
}

void player::move()
{
	switch (stageCount)
	{
	case 1:
		if (m_position.x == m_startPosition.x && m_position.y == m_startPosition.y)
		{
			setDestination(0);
		}
		else if (getDistanceBetween(12) < 1)
		{
			setDestination(14);
		}
		else if (getDistanceBetween(14) < 1)
		{
			destAngle = getAngle(m_position.x, m_position.y, m_position.x, 1050);
		}
		break;
	case 2:
		if (m_position.x == m_startPosition.x && m_position.y == m_startPosition.y)
		{
			setDestination(0);
		}
		else if (getDistanceBetween(0) < 1)
		{
			setDestination(2);
		}
		else if (getDistanceBetween(2) < 1)
		{
			setDestination(5);
		}
		else if (getDistanceBetween(5) < 1)
		{
			setDestination(4);
		}
		else if (getDistanceBetween(4) < 1)
		{
			setDestination(7);
		}
		else if (getDistanceBetween(7) < 1)
		{
			setDestination(8);
		}
		else if (getDistanceBetween(8) < 1)
		{
			destAngle = getAngle(m_position.x, m_position.y, m_position.x, 1050);
		}
		break;
	case 3:
		if (m_position.x == m_startPosition.x && m_position.y == m_startPosition.y)
		{
			setDestination(0);
		}
		else if (getDistanceBetween(6) < 1)
		{
			setDestination(8);
		}
		else if (getDistanceBetween(8) < 1)
		{
			destAngle = getAngle(m_position.x, m_position.y, m_position.x, 1050);
		}
		break;
	case 4:
		if (m_position.x == m_startPosition.x && m_position.y == m_startPosition.y)
		{
			setDestination(0);
		}
		else if (getDistanceBetween(6) < 1)
		{
			setDestination(7);
		}
		else if (getDistanceBetween(7) < 1)
		{
			setDestination(1);
		}
		else if (getDistanceBetween(1) < 1)
		{
			setDestination(2);
		}
		else if (getDistanceBetween(2) < 1)
		{
			destAngle = getAngle(m_position.x, m_position.y, m_position.x, 1050);
		}
		break;
	case 5:
		if (m_position.x == m_startPosition.x && m_position.y == m_startPosition.y)
		{
			setDestination(0);
		}
		else if (getDistanceBetween(0) < 1)
		{
			setDestination(2);
		}
		else if (getDistanceBetween(2) < 1)
		{
			setDestination(8);
		}
		else if (getDistanceBetween(8) < 1)
		{
			setDestination(7);
		}
		else if (getDistanceBetween(7) < 1)
		{
			setDestination(4);
		}
		else if (getDistanceBetween(4) < 1)
		{
			setDestination(3);
		}
		else if (getDistanceBetween(3) < 1)
		{
			setDestination(9);
		}
		else if (getDistanceBetween(9) < 1)
		{
			setDestination(11);
		}
		else if (getDistanceBetween(11) < 1)
		{
			destAngle = getAngle(m_position.x, m_position.y, m_position.x, 1050);
		}
		break;
	case 6:
		if (m_position.x == m_startPosition.x && m_position.y == m_startPosition.y)
		{
			setDestination(0);
		}
		else if (getDistanceBetween(12) < 1)
		{
			setDestination(13);
		}
		else if (getDistanceBetween(13) < 1)
		{
			setDestination(7);
		}
		else if (getDistanceBetween(7) < 1)
		{
			setDestination(8);
		}
		else if (getDistanceBetween(8) < 1)
		{
			destAngle = getAngle(m_position.x, m_position.y, m_position.x, 1050);
		}
		break;
	case 7:
		if (m_position.x == m_startPosition.x && m_position.y == m_startPosition.y)
		{
			setDestination(0);
		}
		else if (getDistanceBetween(0) < 1)
		{
			setDestination(1);
		}
		else if (getDistanceBetween(1) < 1)
		{
			setDestination(4);
		}
		else if (getDistanceBetween(4) < 1)
		{
			setDestination(3);
		}
		else if (getDistanceBetween(3) < 1)
		{
			setDestination(6);
		}
		else if (getDistanceBetween(6) < 1)
		{
			setDestination(8);
		}
		else if (getDistanceBetween(8) < 1)
		{
			destAngle = getAngle(m_position.x, m_position.y, m_position.x, 1050);
		}
		break;
	case 8:
		if (m_position.x == m_startPosition.x && m_position.y == m_startPosition.y)
		{
			setDestination(0);
		}
		else if (getDistanceBetween(0) < 1)
		{
			setDestination(2);
		}
		else if (getDistanceBetween(2) < 1)
		{
			setDestination(5);
		}
		else if (getDistanceBetween(5) < 1)
		{
			setDestination(3);
		}
		else if (getDistanceBetween(3) < 1)
		{
			setDestination(6);
		}
		else if (getDistanceBetween(6) < 1)
		{
			setDestination(7);
		}
		else if (getDistanceBetween(7) < 1)
		{
			setDestination(10);
		}
		else if (getDistanceBetween(10) < 1)
		{
			setDestination(9);
		}
		else if (getDistanceBetween(9) < 1)
		{
			setDestination(12);
		}
		else if (getDistanceBetween(12) < 1)
		{
			setDestination(14);
		}
		else if (getDistanceBetween(14) < 1)
		{
			destAngle = getAngle(m_position.x, m_position.y, m_position.x, 1050);
		}
		break;
	case 9:
		if (m_position.x == m_startPosition.x && m_position.y == m_startPosition.y)
		{
			setDestination(0);
		}
		else if (getDistanceBetween(6) < 1)
		{
			setDestination(7);
		}
		else if (getDistanceBetween(7) < 1)
		{
			setDestination(1);
		}
		else if (getDistanceBetween(1) < 1)
		{
			setDestination(2);
		}
		else if (getDistanceBetween(2) < 1)
		{
			setDestination(11);
		}
		else if (getDistanceBetween(11) < 1)
		{
			setDestination(9);
		}
		else if (getDistanceBetween(9) < 1)
		{
			setDestination(12);
		}
		else if (getDistanceBetween(12) < 1)
		{
			setDestination(14);
		}
		else if (getDistanceBetween(14) < 1)
		{
			destAngle = getAngle(m_position.x, m_position.y, m_position.x, 1050);
		}
		break;
	case 10:
		if (m_position.x == m_startPosition.x && m_position.y == m_startPosition.y)
		{
			setDestination(0);
		}
		else if (getDistanceBetween(0) < 1)
		{
			setDestination(2);
		}
		else if (getDistanceBetween(2) < 1)
		{
			setDestination(14);
		}
		else if (getDistanceBetween(14) < 1)
		{
			setDestination(12);
		}
		else if (getDistanceBetween(12) < 1)
		{
			setDestination(3);
		}
		else if (getDistanceBetween(3) < 1)
		{
			setDestination(4);
		}
		else if (getDistanceBetween(4) < 1)
		{
			setDestination(10);
		}
		else if (getDistanceBetween(10) < 1)
		{
			destAngle = getAngle(m_position.x, m_position.y, m_position.x, 1050);
		}
		break;
	default:
		break;
	}
	

	m_position.x += cosf(destAngle)*m_playerSpeed;
	m_position.y += -sinf(destAngle)*m_playerSpeed;

	if (destAngle >= PI / 4 && destAngle <= 3 * PI / 4)
	{
		m_direction = PLAYER_UP;	
	}
	else if (destAngle >= 3 * PI / 4 && destAngle <= 5 * PI / 4)
	{
		m_direction = PLAYER_LEFT;
	}
	else if (destAngle >= 5 * PI / 4 && destAngle <= 7 * PI / 4)
	{
		m_direction = PLAYER_DOWN;
	}
	else if (destAngle >= 7 * PI / 4 && destAngle <= PI2 || destAngle >= 0 && destAngle <= PI / 4 )
	{
		m_direction = PLAYER_RIGHT;
	}

}
void player::setDestination(int _tilemapindex)
{
	destAngle = getAngle(getPosition().x, getPosition().y,
		TILEMANAGER->getTileMapVector()[_tilemapindex]->getPosition().x, TILEMANAGER->getTileMapVector()[_tilemapindex]->getPosition().y);
	
}

float player::getDistanceBetween(int _tilemapindex)
{
	distance =
		getDistance(m_position.x, m_position.y,
			TILEMANAGER->getTileMapVector()[_tilemapindex]->getPosition().x, TILEMANAGER->getTileMapVector()[_tilemapindex]->getPosition().y);

	return distance;
}

void player::battle()
{
	RECT temp;
	for (int i = 0; i < MONSTERMANAGER->getMonsterVector().size(); i++)
	{
		if (IntersectRect(&temp, &m_collider, &MONSTERMANAGER->getMonsterVector()[i]->getCollider()))
		{
			isBattle = true;
			switch (m_direction)
			{
			case PLAYER_IDLE:
				break;
			case PLAYER_UP:
				MONSTERMANAGER->getMonsterVector()[i]->setDirection(MONSTER_DOWN);
				break;
			case PLAYER_DOWN:
				MONSTERMANAGER->getMonsterVector()[i]->setDirection(MONSTER_UP);
				break;
			case PLAYER_LEFT:
				MONSTERMANAGER->getMonsterVector()[i]->setDirection(MONSTER_RIGHT);
				break;
			case PLAYER_RIGHT:
				MONSTERMANAGER->getMonsterVector()[i]->setDirection(MONSTER_LEFT);
				break;
			default:
				break;
			}
			
						
				MONSTERMANAGER->getMonsterVector()[i]->setMonsterCurHp(MONSTERMANAGER->getMonsterVector()[i]->getMonsterCurHp() -
					(m_playerAtk - MONSTERMANAGER->getMonsterVector()[i]->getMonsterDef()));

				if (MONSTERMANAGER->getMonsterVector()[i]->getMonsterCurHp() > 0)
				{
					if (MONSTERMANAGER->getMonsterVector()[i]->getMonsterAtk() - m_playerDef <= 0)
					{
						m_playerCurHp -= 1;
					}
					else
					{
						m_playerCurHp -= MONSTERMANAGER->getMonsterVector()[i]->getMonsterAtk() - m_playerDef;
					}
					
				}
				else if (MONSTERMANAGER->getMonsterVector()[i]->getMonsterCurHp() <= 0)
				{
					MONSTERMANAGER->getMonsterVector()[i]->setPosition(PointMake(10000, 10000));
					m_playerGold += MONSTERMANAGER->getMonsterVector()[i]->getMonsterGold();
					m_playerExp += MONSTERMANAGER->getMonsterVector()[i]->getMonsterExp();
					//버프 디버프
					m_defShrineCount--;
					if (m_defShrineCount <= 0) m_defShrineCount = 0;
					m_poisonClawCount--;
					if (m_poisonClawCount <= 0) m_poisonClawCount = 0;

					if (m_defShrineCount == 1)
					{
						isDefShrineActive = false;
						m_playerDef -= 4;
					}
					if (m_poisonClawCount == 1)
					{
						m_playerMaxHp += 20;
						m_playerAtk += 2;
						m_playerDef += 2;
					}
					
					isBattle = false;
					startTime = 0;
				}				
		}
	}		
}

void player::eventIncounter()
{
	RECT temp;
	for (int i = 0; i < EVENTMANAGER->getEventVector().size(); i++)
	{
		if (IntersectRect(&temp, &m_collider, &EVENTMANAGER->getEventVector()[i]->getCollider()))
		{
			/*EVENTMANAGER->getEventVector()[i]->setEventCurHp(EVENTMANAGER->getEventVector()[i]->getEventCurHp() -
				(m_playerAtk - EVENTMANAGER->getEventVector()[i]->getEventDef()));*/

			switch (EVENTMANAGER->getEventVector()[i]->getEventType())
			{
			case EVENT_NONE:
				break;
			case EVENT_FIRETRAP:
				m_playerCurHp -= EVENTMANAGER->getEventVector()[i]->getEventAtk();		
				break;
			case EVENT_BITETRAP:
				m_playerCurHp -= EVENTMANAGER->getEventVector()[i]->getEventAtk();
				break;
			case EVENT_CLAWTRAP:
				m_playerCurHp -= EVENTMANAGER->getEventVector()[i]->getEventAtk();
				break;
			case EVENT_POSIONCLAWTRAP:
				m_poisonClawCount = 4;
				m_playerMaxHp -= 20;
				m_playerAtk -= 2;
				m_playerDef -= 2;
				break;
			case EVENT_WINDTRAP:
				m_playerCurHp -= EVENTMANAGER->getEventVector()[i]->getEventAtk();
				break;
			case EVENT_HPSHRINE:
				m_playerCurHp += EVENTMANAGER->getEventVector()[i]->getEventPower();
				break;
			case EVENT_DEFSHRINE:
				isDefShrineActive = true;
				m_defShrineCount = 4;
				m_playerDef += 4;
				break;
			case EVENT_EXPSHRINE:
				m_playerExp += EVENTMANAGER->getEventVector()[i]->getEventPower();
				break;
			case EVENT_GOLDBAG:
				m_playerGold += EVENTMANAGER->getEventVector()[i]->getEventPower();
				break;
			case EVENT_VENDER:
				
				break;
			case EVENT_CLASSCHANGE:
				isClassChange1 = true;				
				if (isClassChange1 && isClassChange2)
				{
					m_image = IMAGEMANAGER->addFrameImage("플레이어업그레이드", "images/Celica_A.bmp", 128 * 1.5, 191 * 1.5, 4, 6, true, RGB(255, 0, 255));
					isMoving = false;
					m_direction = PLAYER_IDLE;
					m_playerAtk += 5;
					m_playerDef += 3;
					m_playerMaxHp += 70;
					m_playerCurHp = m_playerMaxHp;
					/*m_playerSpeed += 0.5;*/

				}
				break;
			default:
				break;
			}		
			if (EVENTMANAGER->getEventVector()[i]->getEventType() != EVENT_VENDER)
			{
				EVENTMANAGER->getEventVector()[i]->setPosition(PointMake(10000, 10000));
			}
		}				
	}
	//상점 접근하면 켜짐
	for (int i = 0; i < EVENTMANAGER->getEventVector().size(); i++)
	{
		if (EVENTMANAGER->getEventVector()[i]->getEventType() == EVENT_VENDER)
		{
			if(getDistance(m_position.x, m_position.y,
				EVENTMANAGER->getEventVector()[i]->getPosition().x, EVENTMANAGER->getEventVector()[i]->getPosition().y) < 40)
			{				
					ITEMMANAGER->setInVender(true);
			}
			else
			{
				ITEMMANAGER->setInVender(false);
			}
		}

	}

	
}

void player::dolevelUp()
{
	if (m_playerExp >= m_playerExpMax)
	{
		m_playerLevel += 1;
		m_playerMaxHp += 5;
		m_playerCurHp += (m_playerMaxHp / 5);
		m_playerAtk += 1;
		if (m_playerLevel % 3 == 0)
		{
			m_playerDef += 1;
		}
		m_playerExpMax = m_playerExpMax += m_playerLevel * 2;
	}
}

void player::buying()
{
	for (int i = 0; i < ITEMMANAGER->getVenderVector().size(); i++)
	{
		if (PtInRect(&ITEMMANAGER->getVenderVector()[i]->getCollider(), m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (ITEMMANAGER->getVenderVector()[i]->getItemPrice() <= m_playerGold && ITEMMANAGER->getVenderVector()[i]->getIsPurchased() == false)
			{
				m_playerGold -= ITEMMANAGER->getVenderVector()[i]->getItemPrice();
				ITEMMANAGER->buyItem(ITEMMANAGER->getVenderVector()[i]->getItemType());
				ITEMMANAGER->getVenderVector()[i]->setIsPurchased(true);

				ITEMTYPE boughtItem = ITEMMANAGER->getVenderVector()[i]->getItemType();

				switch (boughtItem)
				{
				case ITEM_NONE:
					break;
				case ITEM_SWORD1:
					m_playerAtk += ITEMMANAGER->getVenderVector()[i]->getItemAtk();
					break;
				case ITEM_SWORD2:
					m_playerAtk += ITEMMANAGER->getVenderVector()[i]->getItemAtk();
					break;
				case ITEM_SWORD3:
					m_playerAtk += ITEMMANAGER->getVenderVector()[i]->getItemAtk();
					break;
				case ITEM_SWORD4:
					m_playerMaxHp += ITEMMANAGER->getVenderVector()[i]->getItemHp();
					m_playerAtk += ITEMMANAGER->getVenderVector()[i]->getItemAtk();
					m_playerDef += ITEMMANAGER->getVenderVector()[i]->getItemDef();
					break;
				case ITEM_BOW1:
					m_playerMaxHp += ITEMMANAGER->getVenderVector()[i]->getItemHp();
					m_playerAtk += ITEMMANAGER->getVenderVector()[i]->getItemAtk();
					m_playerDef += ITEMMANAGER->getVenderVector()[i]->getItemDef();
					break;
				case ITEM_SHIELD1:
					m_playerMaxHp += ITEMMANAGER->getVenderVector()[i]->getItemHp();
					m_playerDef += ITEMMANAGER->getVenderVector()[i]->getItemDef();
					break;
				case ITEM_SHIELD2:
					m_playerMaxHp += ITEMMANAGER->getVenderVector()[i]->getItemHp();
					m_playerDef += ITEMMANAGER->getVenderVector()[i]->getItemDef();
					break;
				case ITEM_SHIELD3:
					m_playerMaxHp += ITEMMANAGER->getVenderVector()[i]->getItemHp();
					m_playerDef += ITEMMANAGER->getVenderVector()[i]->getItemDef();
					break;				
				case ITEM_HPRING:
					m_playerMaxHp += ITEMMANAGER->getVenderVector()[i]->getItemHp();
					m_playerCurHp += ITEMMANAGER->getVenderVector()[i]->getItemPower();
					break;
				case ITEM_DUALRING1:
					m_playerAtk += ITEMMANAGER->getVenderVector()[i]->getItemAtk();
					m_playerDef += ITEMMANAGER->getVenderVector()[i]->getItemDef();
					break;
				case ITEM_DUALRING2:
					m_playerAtk += ITEMMANAGER->getVenderVector()[i]->getItemAtk();
					m_playerDef += ITEMMANAGER->getVenderVector()[i]->getItemDef();
					break;
				case ITEM_KEY:
					break;
				case ITEM_POTION:
					m_playerCurHp += ITEMMANAGER->getVenderVector()[i]->getItemPower();
					break;
				case ITEM_CLASSBOOK:
					m_playerMaxHp += ITEMMANAGER->getVenderVector()[i]->getItemHp();
					m_playerAtk += ITEMMANAGER->getVenderVector()[i]->getItemAtk();
					m_playerDef += ITEMMANAGER->getVenderVector()[i]->getItemDef();
					m_playerCurHp += ITEMMANAGER->getVenderVector()[i]->getItemPower();
					isClassChange2 = true;
					if (isClassChange1 && isClassChange2)
					{
						m_image = IMAGEMANAGER->addFrameImage("플레이어업그레이드", "images/Celica_A.bmp", 128 * 1.5, 191 * 1.5, 4, 6, true, RGB(255, 0, 255));
						isMoving = false;
						m_direction = PLAYER_IDLE;
						m_playerAtk += 5;
						m_playerDef += 3;
						m_playerMaxHp += 70;
						m_playerCurHp = m_playerMaxHp;
						/*m_playerSpeed += 0.5;*/
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

player::player()
{
}


player::~player()
{
}
