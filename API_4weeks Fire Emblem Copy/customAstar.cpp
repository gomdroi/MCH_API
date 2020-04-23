#include "stdafx.h"
#include "customAstar.h"
#include "tile.h"
#include "unitSet.h"
#include "itemSet.h"
#include "UI_command.h"


customAstar::customAstar()
{
}


customAstar::~customAstar()
{
}

HRESULT customAstar::init()
{

	startTile = nullptr;
	movingUnit = nullptr;
	isStartMoving = false;
	isReadyNextMove = false;
	isAI_TargetFound = false;
	startTime = 0;
	isAttackSequenceOne = false;
	isAttackSequenceTwo = false;
	isFinalAttackSequence = false;
	sequenceCount = 0;

	return S_OK;
}

void customAstar::release()
{
}

void customAstar::update()
{
	movingUnit = g_cursor.getSelectedUnit();
	
	if (m_AstarState != m_PreAstarState)
	{
		if (m_AstarState == ASTAR_SHOWUNITATTACKTILE)
		{
			tileInitializing(lastTile);
		}
		else
		{
			tileInitializing(startTile);
		}		
		m_PreAstarState = m_AstarState;
	}
	switch (m_AstarState)
	{
	case ASTAR_IDLE:
		break;
	case ASTAR_UNITMOVE:
		if (!isStartMoving)
		{
			addOpenList();		//���¸���Ʈ�� �߰���G�����
			calculateH();		//H���
			calculateF();		//F���
			addCloseList();		//Ŭ�����Ʈ�� �߰�
			checkArrive();		//�������� üũ
		}	
		else if (isStartMoving)
		{
			startMove();
		}
		break;
	case ASTAR_SHOWUNITMOVETILE:
		showAvailableTile_addOpenList();
		showAvailableTile_addCloseList();
		break;
	case ASTAR_SHOWUNITATTACKTILE:
		showAttackTile_addOpenList();
		showAttackTile_addCloseList();
		break;
	case ASTAR_SEARCHINGTARGET:
		AI_findTarget_addOpenList();
		AI_findTarget_addCloseList();
		break;
	case ASTAR_ATTACKSEQUENCE:
		if (GetTickCount() - startTime > 1000)
		{
			startTime = GetTickCount();
			attackSequence();
		}
		break;
	default:
		break;
	}
}

void customAstar::tileInitializing(tile* _searchingstarttile)
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();
	for (int i = 0; i < l_tileVector.size(); i++)
	{
		l_tileVector[i]->setListOn(false);
		l_tileVector[i]->setF(BIGNUM);
		l_tileVector[i]->setH(0);
		l_tileVector[i]->setG(0);
		l_tileVector[i]->setParent(NULL);
	}
	lastIndex = 0;
	openList.clear();
	closeList.clear();
	enemySearchList.clear();
	closeList.push_back(_searchingstarttile);
}

void customAstar::addOpenList()
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	CL_X = closeList[lastIndex]->getTileIndexX();			//���������� �߰��� Ŭ���� ����Ʈ�� X
	CL_Y = closeList[lastIndex]->getTileIndexY();
	CL_G = closeList[lastIndex]->getG();

	int tileIndex = CL_X + CL_Y * g_cursor.getTileMapSizeX();

	if (CL_Y != 0)	//0��° ���� �ƴ϶�� ��� ���ΰ��
	{
		if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getTileType() == TILE_MOVABLE || movingUnit->getIsFlying())//��� ��� Ÿ���� �������� �ִٸ�
		{
			if (!l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getListOn())//���¸���Ʈ�� ������ �� �Ǿ� �ִ� Ÿ���̶��
			{
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setListOn(true); //���¸���Ʈ�� ���ԵǾ���.
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setG(CL_G + 10);	//Ÿ���� G���� Ŭ���� ����Ʈ�� ���� G+10
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);//Ÿ���� �θ� Ŭ���� ����Ʈ�� ���������� �߰��� Ÿ�Ϸ� ����
				openList.push_back(l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]);//���¸���Ʈ ���� �߰�
			}
			else//���¸���Ʈ�� ������ �Ǿ� �ִ� Ÿ���̶��
			{
				if (CL_G + 10 < l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getG())//���� G������ ���� ����� G���� �۴ٸ�
				{
					l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setG(CL_G + 10);//G�� ���Ӱ� ���
					l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);//Ÿ���� �θ� Ŭ���� ����Ʈ�� ���������� �߰��� Ÿ�Ϸ� �缳��
				}
			}
		}
	}

	if (CL_X != 0)//���� : 0��° ���� �ƴ϶��
	{
		if (l_tileVector[tileIndex - 1]->getTileType() == TILE_MOVABLE || movingUnit->getIsFlying())//���� Ÿ���� �̵� �����ϴٸ�
		{
			if (!l_tileVector[tileIndex - 1]->getListOn())
			{
				l_tileVector[tileIndex - 1]->setListOn(true);
				l_tileVector[tileIndex - 1]->setG(CL_G + 10);
				l_tileVector[tileIndex - 1]->setParent(closeList[lastIndex]);
				openList.push_back(l_tileVector[tileIndex - 1]);
			}
			else
			{
				if (CL_G + 10 < l_tileVector[tileIndex - 1]->getG())//���� G������ ���� ����� G���� �۴ٸ�
				{
					l_tileVector[tileIndex - 1]->setG(CL_G + 10);//G�� ���Ӱ� ���
					l_tileVector[tileIndex - 1]->setParent(closeList[lastIndex]);//Ÿ���� �θ� Ŭ���� ����Ʈ�� ���������� �߰��� Ÿ�Ϸ� �缳��
				}
			}
		}
	}

	if (CL_X != g_cursor.getTileMapSizeX() - 1)//����:���������� �ƴ϶��
	{
		if (l_tileVector[tileIndex + 1]->getTileType() == TILE_MOVABLE || movingUnit->getIsFlying())//���� Ÿ���� �̵������ϴٸ�
		{
			if (!l_tileVector[tileIndex + 1]->getListOn())
			{
				l_tileVector[tileIndex + 1]->setListOn(true);
				l_tileVector[tileIndex + 1]->setG(CL_G + 10);
				l_tileVector[tileIndex + 1]->setParent(closeList[lastIndex]);
				openList.push_back(l_tileVector[tileIndex + 1]);
			}
			else
			{
				if (CL_G + 10 < l_tileVector[tileIndex + 1]->getG())//���� G������ ���� ����� G���� �۴ٸ�
				{
					l_tileVector[tileIndex + 1]->setG(CL_G + 10);//G�� ���Ӱ� ���
					l_tileVector[tileIndex + 1]->setParent(closeList[lastIndex]);//Ÿ���� �θ� Ŭ���� ����Ʈ�� ���������� �߰��� Ÿ�Ϸ� �缳��
				}
			}
		}
	}

	if (CL_Y != g_cursor.getTileMapSizeY() - 1)//������ ���� �ƴ϶�� �ϴ� ���� ���
	{
		if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getTileType() == TILE_MOVABLE || movingUnit->getIsFlying())//�ϴ� ��� Ÿ���� �̵������ϴٸ�
		{
			if (!l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getListOn())
			{
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setListOn(true);
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setG(CL_G + 10);
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);
				openList.push_back(l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]);
			}
			else
			{
				if (CL_G + 10 < l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getG())//���� G������ ���� ����� G���� �۴ٸ�
				{
					l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setG(CL_G + 10);//G�� ���Ӱ� ���
					l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);//Ÿ���� �θ� Ŭ���� ����Ʈ�� ���������� �߰��� Ÿ�Ϸ� �缳��
				}
			}
		}
	}
}

void customAstar::calculateH()
{
	for (int i = 0; i < openList.size(); i++)
	{
		int vertical = (lastTile->getTileIndexX() - openList[i]->getTileIndexX()) * 10;//����H ��
		int horizontal = (lastTile->getTileIndexY() - openList[i]->getTileIndexY()) * 10;//���� H��


		if (vertical < 0)vertical *= -1;	//������ �ݴ��̸� �����ο�
		if (horizontal < 0)horizontal *= -1;

		openList[i]->setH(vertical + horizontal);	//�� h�� :  ����+���� H
	}
}

void customAstar::calculateF()
{
	for (int i = 0; i < openList.size(); i++)
	{
		openList[i]->setF(openList[i]->getG() + openList[i]->getH()); //F�����
	}
}

void customAstar::addCloseList()
{
	if (openList.size() == 0)//�˻��ߴµ���openList�� ����� 0�̶�� ���̻� ã������ ����
	{
		return;								//���� �۾� ����
	}
	int index = 0;				//���¸���Ʈ �� ���� F�� ���� Ÿ���� �ε���
	int lowest = BIGNUM;		//���¸���Ʈ �� ���� ���� F��

	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i]->getF() < lowest)	//���¸���Ʈ�� F�� ���������ź��� �� �۴ٸ�
		{
			lowest = openList[i]->getF();	//lowest�� �� F��
			index = i;					//index�� �� index��	
		}
	}

	closeList.push_back(openList[index]);		//Ŭ���� ����Ʈ�� �߰�
	openList.erase(openList.begin() + index);	//Ŭ�����Ʈ�� �߰��� Ÿ���� ���¸���Ʈ���� ����


	lastIndex++;	//���� ���߿� �߰��� Ŭ������ �ε���
}

void customAstar::checkArrive()
{
	if (closeList[lastIndex]->getTileIndexY() == lastTile->getTileIndexY() &&
		closeList[lastIndex]->getTileIndexX() == lastTile->getTileIndexX())//Ŭ���� ����Ʈ�� x,y�� ���������� ���ٸ�
	{
		storeWay(closeList[lastIndex]);			//��θ� ���ÿ� ����
		lastTile = closeList[lastIndex];
	}
}

void customAstar::storeWay(tile * _tile)
{	
	moveList.push(_tile);   //���ÿ� �������	
	
	_tile = _tile->getParent();	//Ÿ���� �θ� �����ؼ� storeWay()�Լ����ٽ� �ִ´�.

	if (_tile == startTile) //���������� ������ �׸�
	{		
		moveList.push(_tile);
		//���������� �ִ� ������ �̵��� ���ֺ����� ����
		movingUnit = moveList.top()->getOnTileUnit();
		//�̵��� ������ �̵��� ����
		movingUnit->setIsMove(true);
		//Ŀ���ʿ� ���̽�Ÿ �̵����� �Լ��� ������ �˸�
		isStartMoving = true;
		isReadyNextMove = true;
		return;
	}
	else
	{
		storeWay(_tile);		//�ٽ� ȣ��
	}		
}

void customAstar::startMove()
{ 
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	//���� �̵��� Ÿ�������� ����
	if (isReadyNextMove)
	{
		//�̵�ó���� ���� Ÿ���� ���ÿ��� ����
		moveList.pop();	
		//�̵�ó���� Ÿ���� ���̻� ���ٸ�
		if (moveList.empty())
		{
			//������ �̵��� ���ְ�
			movingUnit->setIsMove(false);
			//���������� �� ���̹Ƿ� ���������� �߾����� ������ ��ġ
			movingUnit->setPosition(PointFloatMake(lastTile->getPosition().x, lastTile->getPosition().y));
			//�̵������� ������ ���������� �Ѱ���
			lastTile->setOnTileUnit(movingUnit);
			//�̵������� ������ ������ ������
			closeList[0]->setOnTileUnit(nullptr);
			//�̵� ���� �˸�
			isStartMoving = false;
			m_AstarState = ASTAR_IDLE;

			//�����ϸ� �̵�ǥ�� Ÿ���� ���ش�
			for (int i = 0; i < l_tileVector.size(); i++)
			{
				l_tileVector[i]->setShowBlueTile(false);
			}

			//�츮���ϰ�� ��Ʈ�� UI�� ����
			if (movingUnit->getUnitDef() == DEF_ALLY)
			{				
				//���� ��ó�� �ִ��� �˻�
				tileInitializing(lastTile);
				while (true)
				{
					showAttackTile_addOpenList();
					if(showAttackTile_addCloseList()) break;								
				}
				//��Ʈ�� Ȱ��ȭ
				g_cursor.setIsStopControl(false);
				//���ֿ��� ����ϴ� UI����		
				UIMANAGER->getCommand()->setCommandState(COMMAND_MOVECONFIRM);
				UIMANAGER->getCommand()->adjustCommandWindow();
				UIMANAGER->getCommand()->arrangementCommand();
				isCommandStart = true;
				g_cursor.setCurCommand(UIMANAGER->getCommand()->getCommandBar());
				g_cursor.setCursorState(CURSOR_COMMANDSELECT);
			}
			//���� ��� ����ϰų� ����
			else if (movingUnit->getUnitDef() == DEF_ENEMY)
			{
				//���ϰ�� �̵����� ��ġ���� ��Ž���� ���ؼ� ĳ������ ����Ÿ���� �ٲ���
				movingUnit->setOccupiedTile(lastTile);

				if (movingUnit->getIsTargetFound())
				{
					tileInitializing(lastTile);
					while (true)
					{
						showAttackTile_addOpenList();
						if (showAttackTile_addCloseList()) break;
					}
				}
				if (!movingUnit->getIsTargetFound())
				{
					finishMove();
				}
			}
			
			return;
		}

		movingUnit->setUnitAngle(getAngle(movingUnit->getPosition().x, movingUnit->getPosition().y,
			moveList.top()->getPosition().x, moveList.top()->getPosition().y));
		isReadyNextMove = false;			
	}
	else if (!isReadyNextMove)
	{
		if (getDistance(movingUnit->getPosition().x, movingUnit->getPosition().y,
			moveList.top()->getPosition().x, moveList.top()->getPosition().y) < 3)
		{
			movingUnit->setPosition(PointFloatMake(moveList.top()->getPosition().x, moveList.top()->getPosition().y));
			isReadyNextMove = true;
		}
	}			
}

void customAstar::finishMove()
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	if (movingUnit != nullptr)
	{
		//������ ���� ���ְ�
		movingUnit->setIsTurnEnd(true);
		movingUnit->setIsTakingTurn(false);
		movingUnit->setIsPickTarget(false);
		movingUnit->setIsTargeted(false);
		//���� ������ ����ְ�
		movingUnit = nullptr;
	}	
	//���̽�Ÿ �̵��Լ��� ���� Ŀ���ʿ� �˸�
	g_cursor.setSelectMoveTile(false);
	//�̵��� ����ߴ� Ÿ���� �̵� ���� �ʱ�ȭ
	for (int i = 0; i < l_tileVector.size(); i++)
	{
		l_tileVector[i]->setShowRedTile(false);
		l_tileVector[i]->setShowBlueTile(false);
	}
	m_AstarState = ASTAR_IDLE;

	isReadyNextMove = false;
	isCommandStart = false;
	isAI_TargetFound = false;
	g_cursor.setMenuNum(0);

	//������â ũ�� ���� �ʱ�ȭ
	UIMANAGER->getCommand()->setMoveConfirmFrontNum(2);
	UIMANAGER->getCommand()->setIsAtkPossible(false);
	g_cursor.setCursorState(CURSOR_IDLE);
}

void customAstar::cancelMove()
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	if (movingUnit != nullptr)
	{
		//�̵������� ���������� ������
		movingUnit->setPosition(PointFloatMake(startTile->getPosition().x, startTile->getPosition().y));
		//�̵������� ������ ���������� �Ѱ���
		startTile->setOnTileUnit(movingUnit);
	}

	//���̽�Ÿ �̵��Լ��� ���� Ŀ���ʿ� �˸�
	g_cursor.setSelectMoveTile(false);
	
	
	if (startTile != lastTile)
	{
		//�̵������� ������ ������ ������
		lastTile->setOnTileUnit(nullptr);
	}
	
	
	g_cursor.setSelectAttackTile(false);

	//�̵��� ����ߴ� Ÿ���� �̵� ���� �ʱ�ȭ
	for (int i = 0; i < l_tileVector.size(); i++)
	{
		l_tileVector[i]->setShowBlueTile(false);
		l_tileVector[i]->setShowRedTile(false);
	}
	m_AstarState = ASTAR_IDLE;

	isReadyNextMove = false;
	isCommandStart = false;

	g_cursor.setSelectItem(false);
	g_cursor.setMenuNum(0);

	//������â ũ�� ���� �ʱ�ȭ
	UIMANAGER->getCommand()->setMoveConfirmFrontNum(2);
	UIMANAGER->getCommand()->setIsAtkPossible(false);
	g_cursor.setCursorState(CURSOR_IDLE);
}

void customAstar::attackCommand()
{
	g_cursor.setSelectMoveTile(false);
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	for (int i = 0; i < l_tileVector.size(); i++)
	{
		l_tileVector[i]->setShowBlueTile(false);
	}
	tileInitializing(lastTile);

	isReadyNextMove = false;
	isCommandStart = false;


}

void customAstar::attackSequence()
{
	sequenceCount += 1;
	//�ʿ��� ��� ����
	int SU_HitChance = (g_cursor.getSelectedUnit()->getHit() - g_cursor.getReceiveUnit()->getAvoid());
	int RU_HitChance = (g_cursor.getReceiveUnit()->getHit() - g_cursor.getSelectedUnit()->getAvoid());
	int SU_Damage = (g_cursor.getSelectedUnit()->getAtk() - g_cursor.getReceiveUnit()->getDef());
	int RU_Damage = (g_cursor.getReceiveUnit()->getAtk() - g_cursor.getSelectedUnit()->getDef());
	int hitDice = RND->getInt(100);
	int critChance = RND->getInt(100);
	if (isAttackSequenceOne)
	{
		//���� ���
		if (hitDice > SU_HitChance) //������ ���
		{
			EFFECTMANAGER->play("������", g_cursor.getReceiveUnit()->getPosition().x - CAMERA->m_cameraPos.x, g_cursor.getReceiveUnit()->getPosition().y - CAMERA->m_cameraPos.y);
		}
		else if (hitDice <= SU_HitChance) //������ ���
		{
			if (critChance < g_cursor.getSelectedUnit()->getCrit())
			{
				g_cursor.getReceiveUnit()->setCurHp(g_cursor.getReceiveUnit()->getCurHp() - (SU_Damage + 5));
				EFFECTMANAGER->play("����ũ��", g_cursor.getReceiveUnit()->getPosition().x - CAMERA->m_cameraPos.x, g_cursor.getReceiveUnit()->getPosition().y - CAMERA->m_cameraPos.y);
			}
			else
			{
				g_cursor.getReceiveUnit()->setCurHp(g_cursor.getReceiveUnit()->getCurHp() - (SU_Damage));
				EFFECTMANAGER->play("������", g_cursor.getReceiveUnit()->getPosition().x - CAMERA->m_cameraPos.x, g_cursor.getReceiveUnit()->getPosition().y - CAMERA->m_cameraPos.y);
			}
		}
		else
		{
			EFFECTMANAGER->play("������", 500, 500);
		}
		//�׾����� üũ�ϰ� �׾����� �ݰ��� ����
		if (g_cursor.getReceiveUnit()->getCurHp() <= 0)
		{
			g_cursor.getReceiveUnit()->setIsDead(true);
			g_cursor.getReceiveUnit()->getOccupiedTile()->setOnTileUnit(nullptr);
			sequenceCount = 2;
		}
		//����� ����ִ� ������ �����Ÿ��� üũ���� �� �ڽź��� ����(&&) �ٷ� ���� �ƴ϶�� �ݰ��� ����
		if (g_cursor.getSelectedUnit()->getEquiped()->getWeaponRng() > g_cursor.getReceiveUnit()->getEquiped()->getWeaponRng() &&
			getDistance(g_cursor.getSelectedUnit()->getPosition().x, g_cursor.getSelectedUnit()->getPosition().y,
				g_cursor.getReceiveUnit()->getPosition().x, g_cursor.getReceiveUnit()->getPosition().y) > TILEX)
		{
			sequenceCount = 2;
		}
		//����� Ÿ���� �Ǻ������� �ü��̰�(&&) �ٷ� ������ �����ߴٸ� �ݰ��� ����
		if (g_cursor.getReceiveUnit()->getUnitType() == TYPE_ARCHER || g_cursor.getReceiveUnit()->getUnitType() == TYPE_REBECCA)
		{
			if (getDistance(g_cursor.getSelectedUnit()->getPosition().x, g_cursor.getSelectedUnit()->getPosition().y,
				g_cursor.getReceiveUnit()->getPosition().x, g_cursor.getReceiveUnit()->getPosition().y) <= TILEX)
			{
				sequenceCount = 2;
			}
		}
		if (sequenceCount == 2)
		{
			isFinalAttackSequence = true;
			isAttackSequenceOne = false;
			isAttackSequenceTwo = false;
		}
		else
		{
			isAttackSequenceOne = false;
			isAttackSequenceTwo = true;
		}
	}
	else if (isAttackSequenceTwo)
	{
		//���� ���
		if (hitDice > RU_HitChance) //������ ���
		{
			EFFECTMANAGER->play("������", g_cursor.getSelectedUnit()->getPosition().x - CAMERA->m_cameraPos.x, g_cursor.getSelectedUnit()->getPosition().y - CAMERA->m_cameraPos.y);
		}
		else if (hitDice <= RU_HitChance) //������ ���
		{
			if (critChance < g_cursor.getReceiveUnit()->getCrit())
			{
				g_cursor.getSelectedUnit()->setCurHp(g_cursor.getSelectedUnit()->getCurHp() - (RU_Damage + 5));
				EFFECTMANAGER->play("����ũ��", g_cursor.getSelectedUnit()->getPosition().x - CAMERA->m_cameraPos.x, g_cursor.getSelectedUnit()->getPosition().y - CAMERA->m_cameraPos.y);
			}
			else
			{
				g_cursor.getSelectedUnit()->setCurHp(g_cursor.getSelectedUnit()->getCurHp() - RU_Damage);
				EFFECTMANAGER->play("������", g_cursor.getSelectedUnit()->getPosition().x - CAMERA->m_cameraPos.x, g_cursor.getSelectedUnit()->getPosition().y - CAMERA->m_cameraPos.y);
			}
		}

		if (g_cursor.getSelectedUnit()->getCurHp() <= 0)
		{
			g_cursor.getSelectedUnit()->setIsDead(true);
			g_cursor.getSelectedUnit()->getOccupiedTile()->setOnTileUnit(nullptr);
			sequenceCount = 2;
		}

		if (sequenceCount == 2)
		{
			isFinalAttackSequence = true;
			isAttackSequenceOne = false;
			isAttackSequenceTwo = false;
		}
		else
		{
			isAttackSequenceOne = true;
			isAttackSequenceTwo = false;
		}

	}
	else if (isFinalAttackSequence)
	{
		sequenceCount = 0;
		isBattleStart = false;
		m_AstarState = ASTAR_IDLE;
		g_cursor.setSelectAttackTile(false);
		//������ �ϱ�� �� ������ ������ ����(������ȯ��)
		g_cursor.getSelectedUnit()->setIsAttack(false);
		//������ �ޱ�� �� ������ ������ ����(������ȯ��)
		g_cursor.getReceiveUnit()->setIsAttack(false);
		if (isEnemyTurn)
		{
			g_cursor.getSelectedUnit()->setCommanceAttack(false);
			g_cursor.getSelectedUnit()->setIsTargetFound(false);
			g_cursor.getSelectedUnit()->setIsTakingTurn(false);
			ASTAR->setAI_TargetUnit(nullptr);
		}
		else
		{
			//��Ʈ���� �ٽ� Ȱ��ȭ
			g_cursor.setIsStopControl(false);
		}
		ASTAR->finishMove();
		//������ ���� üũ
		UNITMANAGER->deadCheck();
	}

}

void customAstar::showAvailableTile_addOpenList()
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	CL_X = closeList[lastIndex]->getTileIndexX();			//���������� �߰��� Ŭ���� ����Ʈ�� X
	CL_Y = closeList[lastIndex]->getTileIndexY();
	CL_G = closeList[lastIndex]->getG();

	int tileIndex = CL_X + CL_Y * g_cursor.getTileMapSizeX();
	bool MovableCheck = false;

	if (CL_Y != 0)	//0��° ���� �ƴ϶�� ��� ���ΰ��
	{
		if (movingUnit->getIsFlying())
		{
			MovableCheck = true;
		}
		else if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getTileType() == TILE_MOVABLE)//��� ��� Ÿ���� �������� �ְ�
		{
			if (isPlayerTurn)
			{
				if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getOnTileUnitType() != DEF_ENEMY)
				{
					MovableCheck = true;
				}				
			}
			else if (isEnemyTurn)
			{
				if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getOnTileUnitType() != DEF_ALLY)
				{
					MovableCheck = true;
				}
			}				
		}

		if (MovableCheck)
		{
			if (!l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getListOn())//���¸���Ʈ�� ������ �� �Ǿ� �ִ� Ÿ���̶��
			{
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setListOn(true); //���¸���Ʈ�� ���ԵǾ���.
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setShowBlueTile(true); //�̵�����Ÿ�� ǥ��
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setG(CL_G + 10);	//Ÿ���� G���� Ŭ���� ����Ʈ�� ���� G+10
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);//Ÿ���� �θ� Ŭ���� ����Ʈ�� ���������� �߰��� Ÿ�Ϸ� ����
				openList.push_back(l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]);//���¸���Ʈ ���� �߰�
			}
			else//���¸���Ʈ�� ������ �Ǿ� �ִ� Ÿ���̶��
			{
			}
			MovableCheck = false;
		}
	}

	if (CL_X != 0)//���� : 0��° ���� �ƴ϶��
	{
		if (movingUnit->getIsFlying())
		{
			MovableCheck = true;
		}
		else if (l_tileVector[tileIndex - 1]->getTileType() == TILE_MOVABLE)//���� Ÿ���� �̵� �����ϴٸ�
		{
			if (isPlayerTurn)
			{
				if (l_tileVector[tileIndex - 1]->getOnTileUnitType() != DEF_ENEMY)
				{
					MovableCheck = true;
				}
			}
			else if (isEnemyTurn)
			{
				if (l_tileVector[tileIndex - 1]->getOnTileUnitType() != DEF_ALLY)
				{
					MovableCheck = true;
				}
			}			
		}

		if (MovableCheck)
		{
			if (!l_tileVector[tileIndex - 1]->getListOn())
			{
				l_tileVector[tileIndex - 1]->setListOn(true);
				l_tileVector[tileIndex - 1]->setShowBlueTile(true);
				l_tileVector[tileIndex - 1]->setG(CL_G + 10);
				l_tileVector[tileIndex - 1]->setParent(closeList[lastIndex]);
				openList.push_back(l_tileVector[tileIndex - 1]);
			}
			else
			{
			}
			MovableCheck = false;
		}
	}

	if (CL_X != g_cursor.getTileMapSizeX() - 1)//����:���������� �ƴ϶��
	{
		if (movingUnit->getIsFlying())
		{
			MovableCheck = true;
		}
		else if (l_tileVector[tileIndex + 1]->getTileType() == TILE_MOVABLE)//���� Ÿ���� �̵������ϴٸ�
		{
			if (isPlayerTurn)
			{
				if (l_tileVector[tileIndex + 1]->getOnTileUnitType() != DEF_ENEMY)
				{
					MovableCheck = true;
				}
			}
			else if (isEnemyTurn)
			{
				if (l_tileVector[tileIndex + 1]->getOnTileUnitType() != DEF_ALLY)
				{
					MovableCheck = true;
				}
			}			
		}

		if (MovableCheck)
		{
			if (!l_tileVector[tileIndex + 1]->getListOn())
			{
				l_tileVector[tileIndex + 1]->setListOn(true);
				l_tileVector[tileIndex + 1]->setShowBlueTile(true);
				l_tileVector[tileIndex + 1]->setG(CL_G + 10);
				l_tileVector[tileIndex + 1]->setParent(closeList[lastIndex]);
				openList.push_back(l_tileVector[tileIndex + 1]);
			}
			else
			{
			}
			MovableCheck = false;
		}
	}

	if (CL_Y != g_cursor.getTileMapSizeY() - 1)//������ ���� �ƴ϶�� �ϴ� ���� ���
	{
		if (movingUnit->getIsFlying())
		{
			MovableCheck = true;
		}
		else if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getTileType() == TILE_MOVABLE)//�ϴ� ��� Ÿ���� �̵������ϴٸ�
		{
			if (isPlayerTurn)
			{
				if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getOnTileUnitType() != DEF_ENEMY)
				{
					MovableCheck = true;
				}
			}
			else if (isEnemyTurn)
			{
				if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getOnTileUnitType() != DEF_ALLY)
				{
					MovableCheck = true;
				}
			}					
		}

		if (MovableCheck)
		{
			if (!l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getListOn())
			{
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setListOn(true);
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setShowBlueTile(true);
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setG(CL_G + 10);
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);
				openList.push_back(l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]);
			}
			else
			{
			}
			MovableCheck = false;
		}
	}
}
void customAstar::showAvailableTile_addCloseList()
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();
	if (openList.size() == 0)//�˻��ߴµ���openList�� ����� 0�̶�� ���̻� ã������ ����
	{
		m_AstarState = ASTAR_IDLE;	//�̵����� üũ�ϴ� ���� ��ž
		return;									//���� �۾� ����
	}
	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i]->getG() < g_cursor.getSelectedUnit()->getMove() * 10)	//���¸���Ʈ�� G�� �̵���*10���� �۴ٸ�
		{
			closeList.push_back(openList[i]);
			openList.erase(openList.begin() + i);
			lastIndex++;			//���� ���߿� �߰��� Ŭ������ �ε���	

			break;
		}
		if (openList[i]->getG() >= g_cursor.getSelectedUnit()->getMove() * 10)
		{
			openList.erase(openList.begin() + i);
		}
	}
}

void customAstar::showAttackTile_addOpenList()
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	CL_X = closeList[lastIndex]->getTileIndexX();			//���������� �߰��� Ŭ���� ����Ʈ�� X
	CL_Y = closeList[lastIndex]->getTileIndexY();
	CL_G = closeList[lastIndex]->getG();

	int tileIndex = CL_X + CL_Y * g_cursor.getTileMapSizeX();

	if (CL_Y != 0)	//0��° ���� �ƴ϶�� ��� ���ΰ��
	{	
		if (!l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getListOn())//���¸���Ʈ�� ������ �� �Ǿ� �ִ� Ÿ���̶��
		{
			l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setListOn(true); //���¸���Ʈ�� ���Խ�Ų��.

			if (m_AstarState != ASTAR_SHOWUNITATTACKTILE) //���� �˻��ϴ� �������� ������̶��
			{
				if (movingUnit->getUnitDef() == DEF_ALLY) //�Ʊ��� ������̶��
				{
					if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getOnTileUnitType() == DEF_ENEMY)
					{
						UIMANAGER->getCommand()->setIsAtkPossible(true);
					}
				}			
				else if (movingUnit->getUnitDef() == DEF_ENEMY) //������ ������̶��
				{
					if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getOnTileUnitType() == DEF_ALLY) //�˻������� ���� �ְ�
					{
						if (l_tileVector[tileIndex] == movingUnit->getOccupiedTile() && movingUnit->getUnitType() == TYPE_ARCHER) 
						{
							//�˻������� �ڱ��ڽ��̰�(&&) �ڽ��� �ü����				
							//�˻������� �ڽ��� ��ġ��� �ٷο��� ���� �ִٴ� �ǹ��̹Ƿ� �ν���������												
						}						
						else
						{
							AI_TargetUnit = l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getOnTileUnit();
							movingUnit->changeEquipItem();
							movingUnit->setCommanceAttack(true);
						}						
					}
				}
				
			}
			else if(m_AstarState == ASTAR_SHOWUNITATTACKTILE)//���ݹ��� ǥ�����̶��
			{
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setShowRedTile(true); //���ݰ���Ÿ�� ǥ��
			}			

			l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setG(CL_G + 10);	//Ÿ���� G���� Ŭ���� ����Ʈ�� ���� G+10
			l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);//Ÿ���� �θ� Ŭ���� ����Ʈ�� ���������� �߰��� Ÿ�Ϸ� ����
			openList.push_back(l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]);//���¸���Ʈ ���� �߰�
		}
		else//���¸���Ʈ�� ������ �Ǿ� �ִ� Ÿ���̶��
		{
		}		
	}

	if (CL_X != 0)//���� : 0��° ���� �ƴ϶��
	{		
		if (!l_tileVector[tileIndex - 1]->getListOn())
		{
			l_tileVector[tileIndex - 1]->setListOn(true);

			if (m_AstarState != ASTAR_SHOWUNITATTACKTILE) 
			{
				if (movingUnit->getUnitDef() == DEF_ALLY) //�Ʊ��� ������̶��
				{
					if (l_tileVector[tileIndex - 1]->getOnTileUnitType() == DEF_ENEMY)
					{
						UIMANAGER->getCommand()->setIsAtkPossible(true);
					}
				}
				else if (movingUnit->getUnitDef() == DEF_ENEMY) //������ ������̶��
				{
					if (l_tileVector[tileIndex - 1]->getOnTileUnitType() == DEF_ALLY)
					{
						if (l_tileVector[tileIndex] == movingUnit->getOccupiedTile() && movingUnit->getUnitType() == TYPE_ARCHER)
						{
							//�˻������� �ڱ��ڽ��̰�(&&) �ڽ��� �ü����				
							//�˻������� �ڽ��� ��ġ��� �ٷο��� ���� �ִٴ� �ǹ��̹Ƿ� �ν���������												
						}
						else
						{
							AI_TargetUnit = l_tileVector[tileIndex - 1]->getOnTileUnit();
							movingUnit->changeEquipItem();
							movingUnit->setCommanceAttack(true);
						}
					}
				}
			}
			
			else if (m_AstarState == ASTAR_SHOWUNITATTACKTILE)//���ݹ��� ǥ�����̶��
			{
				l_tileVector[tileIndex - 1]->setShowRedTile(true); //���ݰ���Ÿ�� ǥ��
			}

			l_tileVector[tileIndex - 1]->setG(CL_G + 10);
			l_tileVector[tileIndex - 1]->setParent(closeList[lastIndex]);
			openList.push_back(l_tileVector[tileIndex - 1]);
		}
		else
		{
		}	
	}

	if (CL_X != g_cursor.getTileMapSizeX() - 1)//����:���������� �ƴ϶��
	{		
		if (!l_tileVector[tileIndex + 1]->getListOn())
		{
			l_tileVector[tileIndex + 1]->setListOn(true);

			if (m_AstarState != ASTAR_SHOWUNITATTACKTILE)
			{
				if (movingUnit->getUnitDef() == DEF_ALLY)
				{
					if (l_tileVector[tileIndex + 1]->getOnTileUnitType() == DEF_ENEMY)
					{
						UIMANAGER->getCommand()->setIsAtkPossible(true);
					}
				}
				else if (movingUnit->getUnitDef() == DEF_ENEMY) //������ ������̶��
				{
					if (l_tileVector[tileIndex + 1]->getOnTileUnitType() == DEF_ALLY)
					{
						if (l_tileVector[tileIndex] == movingUnit->getOccupiedTile() && movingUnit->getUnitType() == TYPE_ARCHER)
						{
							//�˻������� �ڱ��ڽ��̰�(&&) �ڽ��� �ü����				
							//�˻������� �ڽ��� ��ġ��� �ٷο��� ���� �ִٴ� �ǹ��̹Ƿ� �ν���������												
						}
						else
						{
							AI_TargetUnit = l_tileVector[tileIndex + 1]->getOnTileUnit();
							movingUnit->changeEquipItem();
							movingUnit->setCommanceAttack(true);
						}
					}
				}
			}
			else if (m_AstarState == ASTAR_SHOWUNITATTACKTILE)//���ݹ��� ǥ�����̶��
			{
				l_tileVector[tileIndex + 1]->setShowRedTile(true); //���ݰ���Ÿ�� ǥ��
			}

			l_tileVector[tileIndex + 1]->setG(CL_G + 10);
			l_tileVector[tileIndex + 1]->setParent(closeList[lastIndex]);
			openList.push_back(l_tileVector[tileIndex + 1]);
		}
		else
		{
		}		
	}

	if (CL_Y != g_cursor.getTileMapSizeY() - 1)//������ ���� �ƴ϶�� �ϴ� ���� ���
	{			
		if (!l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getListOn())
		{
			l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setListOn(true);

			if (m_AstarState != ASTAR_SHOWUNITATTACKTILE) //���� �˻��ϴ� �������̾�����
			{
				if (movingUnit->getUnitDef() == DEF_ALLY)
				{
					if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getOnTileUnitType() == DEF_ENEMY)
					{
						UIMANAGER->getCommand()->setIsAtkPossible(true);
					}
				}	
				else if (movingUnit->getUnitDef() == DEF_ENEMY) //������ ������̶��
				{
					if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getOnTileUnitType() == DEF_ALLY)
					{
						if (l_tileVector[tileIndex] == movingUnit->getOccupiedTile() && movingUnit->getUnitType() == TYPE_ARCHER)
						{
							//�˻������� �ڱ��ڽ��̰�(&&) �ڽ��� �ü����				
							//�˻������� �ڽ��� ��ġ��� �ٷο��� ���� �ִٴ� �ǹ��̹Ƿ� �ν���������												
						}
						else
						{
							AI_TargetUnit = l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getOnTileUnit();
							movingUnit->changeEquipItem();
							movingUnit->setCommanceAttack(true);
						}
					}
				}
			}
			else if (m_AstarState == ASTAR_SHOWUNITATTACKTILE)//���ݹ��� ǥ�����̶��
			{
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setShowRedTile(true); //���ݰ���Ÿ�� ǥ��
			}

			l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setG(CL_G + 10);
			l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);
			openList.push_back(l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]);
		}
		else
		{
		}		
	}
}
bool customAstar::showAttackTile_addCloseList()
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();
	if (openList.size() == 0)//�˻��ߴµ���openList�� ����� 0�̶�� ���̻� ã������ ����
	{
		if (m_AstarState == ASTAR_SHOWUNITATTACKTILE)
		{
			m_AstarState = ASTAR_IDLE;	//���ݹ��� üũ�ϴ� ���� ��ž
			return true;									//���� �۾� ����
		}
		else if (isEnemyTurn)
		{
			movingUnit->setIsSearchTarget(false);
			movingUnit->setIsTargetFound(false);
			return true;
		}
		else
		{			
			return true;
		}
	}

	if (movingUnit->getCommanceAttack())
	{
		return true;
	}

	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i]->getG() < g_cursor.getSelectedUnit()->getRng() * 10)	//���¸���Ʈ�� G�� �����Ÿ�*10���� �۴ٸ�
		{
			closeList.push_back(openList[i]);
			openList.erase(openList.begin() + i);
			lastIndex++;			//���� ���߿� �߰��� Ŭ������ �ε���	

			break;
		}
		if (openList[i]->getG() >= g_cursor.getSelectedUnit()->getRng() * 10)
		{
			openList.erase(openList.begin() + i);
		}
	}
	return false;
}

void customAstar::AI_findTarget_addOpenList()
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	CL_X = closeList[lastIndex]->getTileIndexX();			//���������� �߰��� Ŭ���� ����Ʈ�� X
	CL_Y = closeList[lastIndex]->getTileIndexY();
	CL_G = closeList[lastIndex]->getG();

	int tileIndex = CL_X + CL_Y * g_cursor.getTileMapSizeX();

	if (CL_Y != 0)	//0��° ���� �ƴ϶�� ��� ���ΰ��
	{			
		if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getTileType() == TILE_MOVABLE
			|| movingUnit->getIsFlying())//��� ��� Ÿ���� �������� �ְų� ���ƴٴϴ� �����̶��
		{
			if (!l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getListOn())//���¸���Ʈ�� ������ �� �Ǿ� �ִ� Ÿ���̶��
			{
 				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setListOn(true); //���¸���Ʈ�� ���ԵǾ���.
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setG(CL_G + 10);	//Ÿ���� G���� Ŭ���� ����Ʈ�� ���� G+10
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);//Ÿ���� �θ� Ŭ���� ����Ʈ�� ���������� �߰��� Ÿ�Ϸ� ����
				openList.push_back(l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]);//���¸���Ʈ ���� �߰�								
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setShowBlueTile(true);
			}
			if (movingUnit->getUnitType() != TYPE_ARCHER) //�ü����ƴ϶�� �� �ڸ��� ���� �˻�
			{
				//�װ��� ������ �ְ� 
				if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getOnTileUnit() != nullptr)
				{
					//�װ� ���̸�
					if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
					{
						//�˻���ġ�� ������� ���
						if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
						{
							isAI_TargetFound = true;
							lastTile = l_tileVector[tileIndex];
						}
					}
				}
			}
			else if (movingUnit->getUnitType() == TYPE_ARCHER) //�ü���� �� �ڸ��� �˻���������� ��ĭ ���������� ���� �˻�
			{
				if (CL_Y != 1) //1��° ���� �ƴ϶�� �˻�
				{
					if (l_tileVector[tileIndex - (g_cursor.getTileMapSizeX() * 2)]->getOnTileUnit() != nullptr)
					{
						//�װ� ���̸�
						if (l_tileVector[tileIndex - (g_cursor.getTileMapSizeX() * 2)]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
						{
							//�˻���ġ�� ������� ���
							if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
							{
								isAI_TargetFound = true;
								lastTile = l_tileVector[tileIndex];
							}
						}
					}
				}			
			}
		}
		if (movingUnit->getUnitType() == TYPE_ARCHER) //�ü���� �»��� �˻�
		{
			if (CL_X != 0)//�»�� :  0��° ���� �ƴ϶��
			{
				//�װ��� ������ �ְ� 
				if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX() - 1]->getOnTileUnit() != nullptr)
				{
					//�װ� ���̸�
					if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX() - 1]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
					{
						//�˻���ġ�� ������� ���
						if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
						{
							isAI_TargetFound = true;
							lastTile = l_tileVector[tileIndex];
						}
					}
				}
			}
			if (CL_X != g_cursor.getTileMapSizeX() - 1)//���� :  ������ ���� �ƴ϶��
			{
				//�װ��� ������ �ְ� 
				if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX() + 1]->getOnTileUnit() != nullptr)
				{
					//�װ� ���̸�
					if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX() + 1]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
					{
						//�˻���ġ�� ������� ���
						if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
						{
							isAI_TargetFound = true;
							lastTile = l_tileVector[tileIndex];
						}
					}
				}
			}
		}
		
	}

	if (CL_X != 0)//���� : 0��° ���� �ƴ϶��
	{			
		if (l_tileVector[tileIndex - 1]->getTileType() == TILE_MOVABLE || movingUnit->getIsFlying())//���� Ÿ���� �̵� �����ϴٸ�
		{
			if (!l_tileVector[tileIndex - 1]->getListOn())
			{
				l_tileVector[tileIndex - 1]->setListOn(true);
				l_tileVector[tileIndex - 1]->setG(CL_G + 10);
				l_tileVector[tileIndex - 1]->setParent(closeList[lastIndex]);
				openList.push_back(l_tileVector[tileIndex - 1]);
				l_tileVector[tileIndex - 1]->setShowBlueTile(true);
			}
			if (movingUnit->getUnitType() != TYPE_ARCHER) //�ü����ƴ϶�� �� �ڸ��� ���� �˻�
			{
				//�װ��� ������ �ְ� 
				if (l_tileVector[tileIndex - 1]->getOnTileUnit() != nullptr)
				{
					//�װ��� ���� �ְ� 
					if (l_tileVector[tileIndex - 1]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
					{
						//�˻���ġ�� ������� ���
						if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
						{
							isAI_TargetFound = true;
							lastTile = l_tileVector[tileIndex];
						}
					}
				}
			}
			else if (movingUnit->getUnitType() == TYPE_ARCHER) //�ü���� �� �ڸ��� �˻���������� ��ĭ ���������� ���� �˻�
			{
				if (CL_X != 1) //1��° ���� �ƴ϶�� �˻�
				{
					if (l_tileVector[tileIndex - 2]->getOnTileUnit() != nullptr)
					{
						//�װ� ���̸�
						if (l_tileVector[tileIndex - 2]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
						{
							//�˻���ġ�� ������� ���
							if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
							{
								isAI_TargetFound = true;
								lastTile = l_tileVector[tileIndex];
							}
						}
					}
				}
			}
		}
	}

	if (CL_X != g_cursor.getTileMapSizeX() - 1)//����:���������� �ƴ϶��
	{			
		if (l_tileVector[tileIndex + 1]->getTileType() == TILE_MOVABLE || movingUnit->getIsFlying())//���� Ÿ���� �̵������ϴٸ�
		{
			if (!l_tileVector[tileIndex + 1]->getListOn())
			{
				l_tileVector[tileIndex + 1]->setListOn(true);
				l_tileVector[tileIndex + 1]->setG(CL_G + 10);
				l_tileVector[tileIndex + 1]->setParent(closeList[lastIndex]);
				openList.push_back(l_tileVector[tileIndex + 1]);
				l_tileVector[tileIndex + 1]->setShowBlueTile(true);
			}
			if (movingUnit->getUnitType() != TYPE_ARCHER) //�ü����ƴ϶�� �� �ڸ��� ���� �˻�
			{
				//�װ��� ������ �ְ� 
				if (l_tileVector[tileIndex + 1]->getOnTileUnit() != nullptr)
				{
					//�װ��� ���� �ְ� 
					if (l_tileVector[tileIndex + 1]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
					{
						//�˻���ġ�� ������� ���
						if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
						{
							isAI_TargetFound = true;
							lastTile = l_tileVector[tileIndex];
						}
					}
				}
			}
			else if (movingUnit->getUnitType() == TYPE_ARCHER) //�ü���� �� �ڸ��� �˻���������� ��ĭ ���������� ���� �˻�
			{
				if (CL_X != g_cursor.getTileMapSizeX() - 2) //���� ������ ���� ���� �ƴ϶�� �˻�
				{
					if (l_tileVector[tileIndex + 2]->getOnTileUnit() != nullptr)
					{
						//�װ� ���̸�
						if (l_tileVector[tileIndex + 2]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
						{
							//�˻���ġ�� ������� ���
							if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
							{
								isAI_TargetFound = true;
								lastTile = l_tileVector[tileIndex];
							}
						}
					}
				}
			}
		}
	}

	if (CL_Y != g_cursor.getTileMapSizeY() - 1)//������ ���� �ƴ϶�� �ϴ� ���� ���
	{				
		if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getTileType() == TILE_MOVABLE
			|| movingUnit->getIsFlying())//�ϴ� ��� Ÿ���� �̵������ϴٸ�
		{
			if (!l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getListOn())
			{
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setListOn(true);
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setG(CL_G + 10);
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);
				openList.push_back(l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]);
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setShowBlueTile(true);
			}
			if (movingUnit->getUnitType() != TYPE_ARCHER) //�ü����ƴ϶�� �� �ڸ��� ���� �˻�
			{
				if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getOnTileUnit() != nullptr)
				{
					//�װ��� ���� �ְ� 
					if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
					{
						//�˻���ġ�� ������� ���
						if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
						{
							isAI_TargetFound = true;
							lastTile = l_tileVector[tileIndex];
						}
					}
				}
			}
			else if (movingUnit->getUnitType() == TYPE_ARCHER) //�ü���� �� �ڸ��� �˻���������� ��ĭ ���������� ���� �˻�
			{
				if (CL_Y != g_cursor.getTileMapSizeY() - 2) //�ϴ� ������ ���� ���� �ƴ϶�� �˻�
				{
					if (l_tileVector[tileIndex + (g_cursor.getTileMapSizeX() * 2)]->getOnTileUnit() != nullptr)
					{
						//�װ� ���̸�
						if (l_tileVector[tileIndex + (g_cursor.getTileMapSizeX() * 2)]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
						{
							//�˻���ġ�� ������� ���
							if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
							{
								isAI_TargetFound = true;
								lastTile = l_tileVector[tileIndex];
							}
						}
					}
				}
			}
		}
		if (CL_X != 0)	//���ϴ�  : 0��° ���� �ƴ϶��
		{
			if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX() - 1]->getOnTileUnit() != nullptr)
			{
				//�װ� ���̸�
				if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX() - 1]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
				{
					//�˻���ġ�� ������� ���
					if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
					{
						isAI_TargetFound = true;
						lastTile = l_tileVector[tileIndex];
					}
				}
			}
		}
		if (CL_X != g_cursor.getTileMapSizeX() - 1)//���ϴ� :������ ���� �ƴ϶��
		{
			if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX() + 1]->getOnTileUnit() != nullptr)
			{
				//�װ� ���̸�
				if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX() + 1]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
				{
					//�˻���ġ�� ������� ���
					if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
					{
						isAI_TargetFound = true;
						lastTile = l_tileVector[tileIndex];
					}
				}
			}
		}
	}
}

bool customAstar::AI_findTarget_addCloseList()
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	if (isAI_TargetFound)
	{
		return true;									//���� �۾� ����
	}
	if (openList.size() == 0)//�˻��ߴµ���openList�� ����� 0�̶�� ���̻� ã������ ����
	{
		return true;									//���� �۾� ����
	}
	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i]->getG() <= g_cursor.getSelectedUnit()->getMove() * 10) //��Ÿ���� ���� �˻�
		{
			enemySearchList.push_back(openList[i]); //���� �������� ������ �� �̵� ��ġ �˻�����Ʈ ���� �߰�
		}
		if (openList[i]->getG() < g_cursor.getSelectedUnit()->getMove() * 10) //����
		{
			closeList.push_back(openList[i]);
			openList.erase(openList.begin() + i);
			lastIndex++;			//���� ���߿� �߰��� Ŭ������ �ε���	

			break;
		}
		if (openList[i]->getG() >= g_cursor.getSelectedUnit()->getMove() * 10)
		{
			openList.erase(openList.begin() + i);
		}
	}
	return false;
}



