#include "stdafx.h"
#include "unitSet.h"
#include "itemSet.h"
#include "tile.h"
#include "customAstar.h"

unitSet::unitSet()
{
}


unitSet::~unitSet()
{
}

HRESULT unitSet::init()
{
	m_direction = UNIT_IDLE;

	maxHp = 0;
	curHp = 0;
	strength = 0;
	skill = 0;
	speed = 0;
	luck = 0;
	defense = 0;
	resistance = 0;
	move = 0;

	atk = 0;
	hit = 0;
	crit = 0;
	avoid = 0;
	rng = 0;
	enemyAttackRange = 0;

	isTargeted = false;
	isSelected = false;
	isForwards = true;
	isTurnEnd = false;
	isMove = false;
	isAttack = false;
	isDead = false;
	isNPC = false;
	isTakingTurn = false;
	isTargetFound = false;
	isSearchTarget = true;
	isPickTarget = false;
	isFlying = false;

	occupiedTile = nullptr;

	inven.clear();

	IMAGEMANAGER->addImage("������彺���ӱ׼�", "images/Unit/eliwood_smallmug.bmp", 96, 96, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�������ӱ׼�", "images/Unit/lyn_smallmug.bmp", 96, 96, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("���ͽ����ӱ׼�", "images/Unit/hector_smallmug.bmp", 96, 96, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("����ī�����ӱ׼�", "images/Unit/rebecca_smallmug.bmp", 96, 96, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("���󽺸��ӱ׼�", "images/Unit/serra_smallmug.bmp", 96, 96, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��Ʈ�����ӱ׼�", "images/Unit/kent_smallmug.bmp", 96, 96, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�÷θ��������ӱ׼�", "images/Unit/florina_smallmug.bmp", 96, 96, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�������ӱ׼�", "images/Unit/enemy_smallmug.bmp", 96, 96, true, RGB(255, 0, 255));
	return S_OK;
}

void unitSet::update()
{
	m_rc = RectMakeCenter(m_position.x, m_position.y, 24, 24);
	animation();
	heading();
	statusCalc();
	if (isMove)
	{
		unitMove();		
	}
	if (isEnemyTurn)
	{
		if (!isTurning)
		{
			if (isNPC)
			{
				unitAI();
			}
		}
	}
	
}

void unitSet::animation()
{
	switch (m_direction)
	{
	case UNIT_IDLE:
			m_unitFrameCount++;
			if (isTurnEnd)
			{
				m_image->setFrameY(6);
			}
			else
			{
				m_image->setFrameY(0);
			}

			if (m_unitFrameCount % 4 == 0)
			{
				if (isForwards)
				{

					m_unitFrameCount = 0;
					m_image->setFrameX(m_unitFrameIndex++);
					if (m_unitFrameIndex >= 2) m_unitFrameIndex = 2;
					if (m_unitFrameIndex == 1) aniDelayTime = GetTickCount();
					if (m_unitFrameIndex == 2)
					{
						if (GetTickCount() - aniDelayTime >= 300)
						{
							isForwards = false;
						}
					}
				}
				else if (!isForwards)
				{
					m_unitFrameCount = 0;
					m_image->setFrameX(m_unitFrameIndex--);
					if (m_unitFrameIndex <= 0) m_unitFrameIndex = 0;
					if (m_unitFrameIndex == 1) aniDelayTime = GetTickCount();
					if (m_unitFrameIndex == 0)
					{
						if (GetTickCount() - aniDelayTime >= 300)
						{
							isForwards = true;
						}
					}
				}
			}		
			break;
	case UNIT_LEFT:	
		m_unitFrameCount++;
		m_image->setFrameY(1);
		if (m_unitFrameCount % 4 == 0)
		{			
			m_unitFrameCount = 0;
			m_image->setFrameX(m_unitFrameIndex++);
			if (m_unitFrameIndex >= m_image->getMaxFrameX()) 
			{
				m_unitFrameIndex = 0;
			}					
		}
		break;
	case UNIT_RIGHT:
		m_unitFrameCount++;
		m_image->setFrameY(2);
		if (m_unitFrameCount % 4 == 0)
		{
			m_unitFrameCount = 0;
			m_image->setFrameX(m_unitFrameIndex++);
			if (m_unitFrameIndex >= m_image->getMaxFrameX())
			{
				m_unitFrameIndex = 0;
			}
		}
		break;
	case UNIT_DOWN:
		m_unitFrameCount++;
		m_image->setFrameY(3);
		if (m_unitFrameCount % 4 == 0)
		{
			m_unitFrameCount = 0;
			m_image->setFrameX(m_unitFrameIndex++);
			if (m_unitFrameIndex >= m_image->getMaxFrameX())
			{
				m_unitFrameIndex = 0;
			}
		}
		break;
	case UNIT_UP:
		m_unitFrameCount++;
		m_image->setFrameY(4);
		if (m_unitFrameCount % 4 == 0)
		{
			m_unitFrameCount = 0;
			m_image->setFrameX(m_unitFrameIndex++);
			if (m_unitFrameIndex >= m_image->getMaxFrameX())
			{
				m_unitFrameIndex = 0;
			}
		}
		break;
	case UNIT_TARGETED:
		m_unitFrameCount++;
		m_image->setFrameY(5);
		if (m_unitFrameCount % 4 == 0)
		{
			if (isForwards)
			{

				m_unitFrameCount = 0;
				m_image->setFrameX(m_unitFrameIndex++);
				if (m_unitFrameIndex >= 2) m_unitFrameIndex = 2;
				if (m_unitFrameIndex == 1) aniDelayTime = GetTickCount();
				if (m_unitFrameIndex == 2)
				{
					if (GetTickCount() - aniDelayTime >= 300)
					{
						isForwards = false;
					}
				}
			}
			else if (!isForwards)
			{
				m_unitFrameCount = 0;
				m_image->setFrameX(m_unitFrameIndex--);
				if (m_unitFrameIndex <= 0) m_unitFrameIndex = 0;
				if (m_unitFrameIndex == 1) aniDelayTime = GetTickCount();
				if (m_unitFrameIndex == 0)
				{
					if (GetTickCount() - aniDelayTime >= 300)
					{
						isForwards = true;
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

void unitSet::heading()
{
	if (isMove || isAttack)
	{
		if (unitAngle >= PI / 4 && unitAngle <= 3 * PI / 4)
		{
			if (moveDirection != UNIT_UP)
			{
				m_unitFrameIndex = 0;
				moveDirection = UNIT_UP;
			}
			m_direction = UNIT_UP;
		}
		else if (unitAngle >= 3 * PI / 4 && unitAngle <= 5 * PI / 4)
		{
			if (moveDirection != UNIT_LEFT)
			{
				m_unitFrameIndex = 0;
				moveDirection = UNIT_LEFT;
			}
			m_direction = UNIT_LEFT;
		}
		else if (unitAngle >= 5 * PI / 4 && unitAngle <= 7 * PI / 4)
		{
			if (moveDirection != UNIT_DOWN)
			{
				m_unitFrameIndex = 0;
				moveDirection = UNIT_DOWN;
			}
			m_direction = UNIT_DOWN;
		}
		else if (unitAngle >= 7 * PI / 4 && unitAngle <= PI2 || unitAngle >= 0 && unitAngle <= PI / 4)
		{
			if (moveDirection != UNIT_RIGHT)
			{
				m_unitFrameIndex = 0;
				moveDirection = UNIT_RIGHT;
			}
			m_direction = UNIT_RIGHT;
		}
	}
	else
	{
		if (isTargeted)
		{
			if (moveDirection != UNIT_TARGETED)
			{
				m_unitFrameIndex = 0;
				moveDirection = UNIT_TARGETED;
			}
			m_direction = UNIT_TARGETED;
		}
		else
		{
			if (moveDirection != UNIT_IDLE)
			{
				m_unitFrameIndex = 0;
				moveDirection = UNIT_IDLE;
			}
			m_direction = UNIT_IDLE;
		}
	}
	
}

void unitSet::render()
{
	m_image->frameRender(getMemDC(), m_position.x - m_image->getFrameWidth() / 2 - CAMERA->m_cameraPos.x, m_position.y - m_image->getFrameHeight() / 2 - CAMERA->m_cameraPos.y, m_image->getFrameX(), m_image->getFrameY());
	
	
	if (isDebug)
	{
		RectangleMake(getMemDC(), m_rc);
	}
}

void unitSet::release()
{
}

void unitSet::setUnit(UNITTYPE _unitType)
{
	switch (_unitType)
	{
	case TYPE_NONE:
		break;
	case TYPE_ELIWOOD:
		m_image = new image;
		m_image->init("images/Unit/eliwood3.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("������彺���ӱ׼�");

		m_unitType = TYPE_ELIWOOD;
		m_unitDef = DEF_ALLY;
		sprintf_s(unitNameStr, "Eliwood");
		maxHp = 20;
		curHp = maxHp;
		strength = 5;
		skill = 7;
		speed = 4;
		luck = 3;
		defense = 2;
		resistance = 2;
		move = 4;

		inven.push_back(ITEMMANAGER->getShopVector()[0]);
		if (g_cursor.getStageNum() == 2 || g_cursor.getStageNum() == 3)
		{
			inven.push_back(ITEMMANAGER->getShopVector()[11]);
		}
		inven.push_back(ITEMMANAGER->getShopVector()[8]);	
		equiped = inven[0];
		break;
	case TYPE_LYN:
		m_image = new image;
		m_image->init("images/Unit/lyn3.bmp", 480, 840, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("�������ӱ׼�");

		m_unitType = TYPE_LYN;
		m_unitDef = DEF_ALLY;
		sprintf_s(unitNameStr, "Lyn");
		maxHp = 15;
		curHp = 15;
		strength = 4;
		skill = 9;
		speed = 8;
		luck = 10;
		defense = 2;
		resistance = 3;
		move = 4;

		inven.push_back(ITEMMANAGER->getShopVector()[0]);
		inven.push_back(ITEMMANAGER->getShopVector()[1]);		
		equiped = inven[0];
		break;
	case TYPE_HECTOR:
		m_image = new image;
		m_image->init("images/Unit/hector3.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("���ͽ����ӱ׼�");

		m_unitType = TYPE_HECTOR;
		m_unitDef = DEF_ALLY;
		sprintf_s(unitNameStr, "Hector");
		maxHp = 25;
		curHp = maxHp;
		strength = 8;
		skill = 3;
		speed = 2;
		luck = 3;
		defense = 3;
		resistance = 3;
		move = 3;

		inven.push_back(ITEMMANAGER->getShopVector()[2]);
		inven.push_back(ITEMMANAGER->getShopVector()[3]);
		inven.push_back(ITEMMANAGER->getShopVector()[8]);
		equiped = inven[0];
		break;
	case TYPE_GUY:
		break;
	case TYPE_REBECCA:
		m_image = new image;
		m_image->init("images/Unit/rebecca3.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("����ī�����ӱ׼�");

		m_unitType = TYPE_REBECCA;
		m_unitDef = DEF_ALLY;
		sprintf_s(unitNameStr, "Rebecca");
		maxHp = 15;
		curHp = maxHp;
		strength = 4;
		skill = 10;
		speed = 10;
		luck = 6;
		defense = 1;
		resistance = 1;
		move = 4;

		inven.push_back(ITEMMANAGER->getShopVector()[6]);
		inven.push_back(ITEMMANAGER->getShopVector()[7]);
		equiped = inven[0];
		break;
	case TYPE_SERRA:
		m_image = new image;
		m_image->init("images/Unit/serra3.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("���󽺸��ӱ׼�");

		m_unitType = TYPE_SERRA;
		m_unitDef = DEF_ALLY;
		sprintf_s(unitNameStr, "Serra");
		maxHp = 12;
		curHp = maxHp;
		strength = 2;
		skill = 4;
		speed = 6;
		luck = 10;
		defense = 2;
		resistance = 4;
		move = 3;

		equiped = inven[0];
		break;
	case TYPE_FLORINA:
		m_image = new image;
		m_image->init("images/Unit/pegasus3.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("�÷θ��������ӱ׼�");

		m_unitType = TYPE_FLORINA;
		m_unitDef = DEF_ALLY;
		sprintf_s(unitNameStr, "Florina");
		maxHp = 18;
		curHp = maxHp;
		strength = 4;
		skill = 8;
		speed = 8;
		luck = 4;
		defense = 3;
		resistance = 3;
		move = 5;
		isFlying = true;

		inven.push_back(ITEMMANAGER->getShopVector()[4]);
		inven.push_back(ITEMMANAGER->getShopVector()[5]);
		inven.push_back(ITEMMANAGER->getShopVector()[10]);
		equiped = inven[0];
		break;
	case TYPE_KENT:
		m_image = new image;
		m_image->init("images/Unit/cavalier3.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("��Ʈ�����ӱ׼�");

		m_unitType = TYPE_KENT;
		m_unitDef = DEF_ALLY;
		sprintf_s(unitNameStr, "Kent");
		maxHp = 23;
		curHp = maxHp;
		strength = 6;
		skill = 5;
		speed = 6;
		luck = 3;
		defense = 4;
		resistance = 3;
		move = 5;

		inven.push_back(ITEMMANAGER->getShopVector()[4]);
		inven.push_back(ITEMMANAGER->getShopVector()[10]);
		equiped = inven[0];
		break;
	case TYPE_BRIGAND:
		m_image = new image;
		m_image->init("images/Unit/brigand3.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("�������ӱ׼�");

		m_unitType = TYPE_BRIGAND;
		m_unitDef = DEF_ENEMY;
		sprintf_s(unitNameStr, "Brigand");
		isNPC = true;
		maxHp = 14;
		curHp = 14;
		strength = 4;
		skill = 6;
		speed = 2;
		luck = 1;
		defense = 1;
		resistance = 2;
		move = 3;
	
		inven.push_back(ITEMMANAGER->getShopVector()[2]);
		equiped = inven[0];
		break;
	case TYPE_SOLDIER:
		m_image = new image;
		m_image->init("images/Unit/soldier3.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("�������ӱ׼�");

		m_unitType = TYPE_SOLDIER;
		m_unitDef = DEF_ENEMY;
		isNPC = true;
		sprintf_s(unitNameStr, "Soldier");
		maxHp = 16;
		curHp = maxHp;
		strength = 5;
		skill = 10;
		speed = 4;
		luck = 3;
		defense = 3;
		resistance = 2;
		move = 2;

		inven.push_back(ITEMMANAGER->getShopVector()[5]);
		equiped = inven[0];
		break;
	case TYPE_CAVALIER:
		m_image = new image;
		m_image->init("images/Unit/cavalier3_enemy.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("�������ӱ׼�");

		m_unitType = TYPE_CAVALIER;
		m_unitDef = DEF_ENEMY;
		isNPC = true;
		sprintf_s(unitNameStr, "Cavalier");
		maxHp = 20;
		curHp = maxHp;
		strength = 7;
		skill = 2;
		speed = 2;
		luck = 1;
		defense = 1;
		resistance = 1;
		move = 5;

		inven.push_back(ITEMMANAGER->getShopVector()[4]);
		equiped = inven[0];
		break;
	case TYPE_ARCHER:
		m_image = new image;
		m_image->init("images/Unit/archer3_enemy.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("�������ӱ׼�");

		m_unitType = TYPE_ARCHER;
		m_unitDef = DEF_ENEMY;
		isNPC = true;
		sprintf_s(unitNameStr, "Archer");
		maxHp = 9;
		curHp = maxHp;
		strength = 3;
		skill = 10;
		speed = 8;
		luck = 8;
		defense = 1;
		resistance = 1;
		move = 4;

		inven.push_back(ITEMMANAGER->getShopVector()[7]);
		equiped = inven[0];
		break;
	case TYPE_FIGHTER:
		break;
	case TYPE_KNIGHT:
		m_image = new image;
		m_image->init("images/Unit/knight3_enemy.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("�������ӱ׼�");

		m_unitType = TYPE_KNIGHT;
		m_unitDef = DEF_ENEMY;
		isNPC = true;
		sprintf_s(unitNameStr, "Knight");
		maxHp = 30;
		curHp = maxHp;
		strength = 4;
		skill = 2;
		speed = 2;
		luck = 2;
		defense = 4;
		resistance = 0;
		move = 3;

		inven.push_back(ITEMMANAGER->getShopVector()[4]);
		inven.push_back(ITEMMANAGER->getShopVector()[10]);
		equiped = inven[0];
		break;
	case TYPE_PEGASUSKNIGHT:
		break;
	case TYPE_WYVERN:
		m_image = new image;
		m_image->init("images/Unit/wyvern3.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("�������ӱ׼�");

		m_unitType = TYPE_WYVERN;
		m_unitDef = DEF_ENEMY;
		isNPC = true;
		sprintf_s(unitNameStr, "wyvern");
		maxHp = 12;
		curHp = maxHp;
		strength = 5;
		skill = 10;
		speed = 5;
		luck = 5;
		defense = 2;
		resistance = 2;
		move = 5;
		isFlying = true;

		inven.push_back(ITEMMANAGER->getShopVector()[4]);
		equiped = inven[0];
		break;
	case TYPE_WARRIOR:
		m_image = new image;
		m_image->init("images/Unit/warrior3_enemy.bmp", 576, 1008, 4, 7, true, RGB(255, 0, 255));
		smallMugShot = IMAGEMANAGER->findImage("�������ӱ׼�");

		m_unitType = TYPE_WARRIOR;
		m_unitDef = DEF_ENEMY;
		isNPC = true;
		sprintf_s(unitNameStr, "Warrior");
		maxHp = 40;
		curHp = maxHp;
		strength = 7;
		skill = 1;
		speed = 3;
		luck = 5;
		defense = 3;
		resistance = 3;
		move = 4;

		inven.push_back(ITEMMANAGER->getShopVector()[3]);
		inven.push_back(ITEMMANAGER->getShopVector()[9]);
		equiped = inven[0];
		break;
	default:
		break;
	}
}

void unitSet::statusCalc()
{
	
	
	
	
	
	

	atk = strength + equiped->getWeaponDmg(); //���ݷ� = �� + ���� ���� ���ݷ�
	hit = skill + equiped->getWeaponHit(); //���߷� = ������� + �ڽ��Ǹ��� + ��
	crit = luck + equiped->getWeaponCrit(); //ġ��Ÿ = ����ġ��Ÿ + ��
	avoid = luck + speed + occupiedTile->getTileAvoid(); //ȸ�� = ���ǵ� + �� + Ÿ���� ȸ�Ƿ�
	def = defense + occupiedTile->getTileDef(); //���� = �ڽ��� ���� + Ÿ���� ����

	for (int i = 0; i < inven.size(); i++)
	{
		if (inven[i]->getWeaponRng() == 2)
		{
			rng = 2;
			return;
		}
		else
		{
			rng = 1;
		}
	} //�����Ÿ� = �κ��丮������ ���� ���� ��Ÿ��� ���� ������ ��Ÿ�
}

void unitSet::unitMove()
{
	m_position.x += cosf(unitAngle)*2;
	m_position.y += -sinf(unitAngle)*2;
}

void unitSet::equipItem()
{
	//���ǵ� = ���⹫�԰� ������ ũ�� �׸�ŭ ���ش�
	int spdPenalty = strength - equiped->getWeaponWeight();
	if (spdPenalty >= 1)
	{
		speed += speed - spdPenalty;
	}
}

void unitSet::unitAI()
{
	if (!isTurnEnd)
	{
		if (isTakingTurn)
		{			
			if (!isBattleStart) //������ ���۵��� �ʾ��� ���
			{
				if (isSearchTarget) //���ڸ����� ���ݰ����� ���� ã�ƺ�
				{
					//���� ��ó�� �ִ��� �˻�
					ASTAR->tileInitializing(occupiedTile);
					while (true)
					{
						ASTAR->showAttackTile_addOpenList();
						if (ASTAR->showAttackTile_addCloseList()) break;
					}
					isTargetFound = true;
				}
				else if (!isSearchTarget) //���ݰ����� ���� �� ã�Ҵٸ�
				{					
					if (m_unitType == TYPE_KNIGHT)
					{
						isTurnEnd = true;
					}
					else
					{
						//���� ����� ���� Ž��
						if (!isPickTarget)
						{
							ASTAR->tileInitializing(occupiedTile);
							while (true)
							{
								ASTAR->AI_findTarget_addOpenList();
								if (ASTAR->AI_findTarget_addCloseList()) break;
							}
							ASTAR->setisAI_TargetFound(false);

							pickTarget();
						}
						//�˻��ߴ� Ÿ���߿��� ���� ����� ���� ���� ����� Ÿ���� ��ǥ�� ����

						ASTAR->setAstarState(ASTAR_UNITMOVE);
					}				
				}
			}	
			if (commanceAttack)
			{
				//������ �޴� ������ ������ ����
				g_cursor.setReceiveUnit(ASTAR->getAI_TargetUnit());
				//ĳ������ �ޱ��� ���� ����ְ�
				g_cursor.getSelectedUnit()->setUnitAngle(getAngle(g_cursor.getSelectedUnit()->getPosition().x, g_cursor.getSelectedUnit()->getPosition().y,
					g_cursor.getReceiveUnit()->getPosition().x, g_cursor.getReceiveUnit()->getPosition().y));
				g_cursor.getReceiveUnit()->setUnitAngle(getAngle(g_cursor.getReceiveUnit()->getPosition().x, g_cursor.getReceiveUnit()->getPosition().y,
					g_cursor.getSelectedUnit()->getPosition().x, g_cursor.getSelectedUnit()->getPosition().y));
				//������ �ϱ�� �� ������ ������ ����(������ȯ��)
				g_cursor.getSelectedUnit()->setIsAttack(true);
				//������ �ޱ�� �� ������ ������ ����(������ȯ��)
				g_cursor.getReceiveUnit()->setIsAttack(true);
				//���� UI �ּ���
				isBattleStart = true;
				//���� �������� �����϶�
				ASTAR->setStartTime(GetTickCount());
				ASTAR->setAstarState(ASTAR_ATTACKSEQUENCE);
				//AI ���ݺ��� �����϶�
				ASTAR->setIsAttackSequenceOne(true);
				commanceAttack = false;
			}
		}
	}
}

void unitSet::pickTarget()
{
	vector<unitSet*> l_allyUnitVector = UNITMANAGER->getAllyUnitVector();
	vector<tile*> l_astarCloseVector = ASTAR->getEnemySearchList();
	float matchTempDistance = 5000;
	float curTempDistance = 0;
	int index = 0;
	bool changeDistance = false;

	for (int i = 0; i < l_allyUnitVector.size(); i++)
	{
		if (!changeDistance)
		{
			curTempDistance = getDistance(this->m_position.x, this->m_position.y,
				l_allyUnitVector[i]->getPosition().x, l_allyUnitVector[i]->getPosition().y);
			changeDistance = true;
			enemyTargetUnit = l_allyUnitVector[i];
		}
		else if (changeDistance)
		{
			matchTempDistance = getDistance(this->m_position.x, this->m_position.y,
				l_allyUnitVector[i]->getPosition().x, l_allyUnitVector[i]->getPosition().y);
		}

		if (matchTempDistance < curTempDistance)
		{
			curTempDistance = matchTempDistance;
			enemyTargetUnit = l_allyUnitVector[i];
		}		
	}

	for (int i = 0; i < l_astarCloseVector.size(); i++)
	{
		if (l_astarCloseVector[i]->getOnTileUnit() != nullptr) continue;
		if (changeDistance)
		{
			curTempDistance = getDistance(enemyTargetUnit->getPosition().x, enemyTargetUnit->getPosition().y,
				l_astarCloseVector[i]->getPosition().x, l_astarCloseVector[i]->getPosition().y);
			changeDistance = false;
			enemyTargetTile = l_astarCloseVector[i];
		}
		else if (!changeDistance)
		{
			matchTempDistance = getDistance(enemyTargetUnit->getPosition().x, enemyTargetUnit->getPosition().y,
				l_astarCloseVector[i]->getPosition().x, l_astarCloseVector[i]->getPosition().y);
		}

		if (matchTempDistance < curTempDistance)
		{
			curTempDistance = matchTempDistance;
			enemyTargetTile = l_astarCloseVector[i];
		}
	}

	ASTAR->setLastTile(enemyTargetTile);
	isPickTarget = true;
}

void unitSet::changeEquipItem()
{
	if (getDistance(m_position.x, m_position.y,
		ASTAR->getAI_TargetUnit()->getPosition().x, ASTAR->getAI_TargetUnit()->getPosition().y) <= 48)
	{
		for (int i = 0; i < inven.size(); i++)
		{
			if (inven[i]->getWeaponRng() == 1)
			{
				equiped = inven[i];
			}
		}
	}
	else if (getDistance(m_position.x, m_position.y,
		ASTAR->getAI_TargetUnit()->getPosition().x, ASTAR->getAI_TargetUnit()->getPosition().y) > 48)
	{
		for (int i = 0; i < inven.size(); i++)
		{
			if (inven[i]->getWeaponRng() == 2)
			{
				equiped = inven[i];
			}
		}
	}
}
