#include "stdafx.h"
#include "cursor.h"
#include "unitSet.h"
#include "itemSet.h"
#include "tile.h"
#include "UI_command.h"
#include "UI_turn.h"

cursor::cursor()
{
}


cursor::~cursor()
{
}

HRESULT cursor::init()
{
	m_image = IMAGEMANAGER->addFrameImage("Ŀ��", "images/cursor.bmp", 240, 120, 4, 2, true, RGB(255, 0, 255));
	commandCursor = IMAGEMANAGER->addFrameImage("Ŀ�ǵ�Ŀ��", "images/UI/fingerCursor2.bmp", 285, 36, 5, 1, true, RGB(255, 0, 255));
	aniDelayTime = 0;

	cursorState = CURSOR_IDLE;
	moveStartTile = nullptr;	//ĳ���� �̵��� ���۵� Ÿ��
	selectedUnit = nullptr;	//������ ĳ��
	receiveUnit = nullptr;	//Ÿ������ ������ ĳ��

	curCommand = nullptr;

	isForwards = true;
	commandCursorIsForward = false;
	isOnUnit = false;
	selectMoveTile = false;
	selectAttackTile = false;
	selectItem = false;
	
	isStopControl = false;
	menuNum = 0;

	return S_OK;
}

void cursor::update()
{		
	UNITMANAGER->endGameCheck();
	turnOverCheck();
	animation();

	if (!isStopControl)
	{
		
		control();
				
		//Ŀ���� ��ġ�� �ڸ��� ������ ������
		if (curTile->getOnTileUnit() != nullptr)
		{
			//�� ������ �Ʊ��̰�
			if (curTile->getOnTileUnit()->getUnitDef() == DEF_ALLY)
			{
				//���� ������ �ʾ����� ������ ����� ����
				if (!curTile->getOnTileUnit()->getIsTurnEnd())
				{
					curTile->getOnTileUnit()->setIsTargeted(true);
				}
			}			
		}
	}

	if (isPlayerTurn)
	{
		CAMERA->m_preCameraPos = CAMERA->m_cameraPos;
	}
	if (isEnemyTurn)
	{
		if (selectedUnit->getPosition().x - WINSIZEX / 2 < 0)
		{
			CAMERA->m_cameraPos.x = 0;
		}
		else if (selectedUnit->getPosition().x + WINSIZEX / 2 > tileMapSizeX * TILEX)
		{
			CAMERA->m_cameraPos.x = tileMapSizeX * TILEX - WINSIZEX;
		}
		else
		{
			CAMERA->m_cameraPos.x = selectedUnit->getPosition().x - WINSIZEX / 2;
		}

		
		if (selectedUnit->getPosition().y - WINSIZEY / 2 < 0)
		{
			CAMERA->m_cameraPos.y = 0;
		}
		else if (selectedUnit->getPosition().y + WINSIZEY / 2 > tileMapSizeY * TILEY)
		{
			CAMERA->m_cameraPos.y = tileMapSizeY * TILEY - WINSIZEY;
		}
		else
		{
			CAMERA->m_cameraPos.y = selectedUnit->getPosition().y - WINSIZEY / 2;
		}
	}
}

void cursor::animation()
{
	//�������� Ŀ���� �÷������� ��
	if (curTile->getOnTileUnit() != nullptr)
	{
		m_image->setFrameX(4);
	}
	//�Ϲ� Ŀ�� �ִϸ��̼�
	else
	{
		m_cursorCount++;
		m_image->setFrameY(0);
		if (m_cursorCount % 4 == 0)
		{
			if (isForwards)
			{

				m_cursorCount = 0;
				m_image->setFrameX(m_cursorIndex++);
				if (m_cursorIndex >= 3) m_cursorIndex = 3;
				if (m_cursorIndex == 2) aniDelayTime = GetTickCount();
				if (m_cursorIndex == 3)
				{
					if (GetTickCount() - aniDelayTime >= 300)
					{
						isForwards = false;
					}
				}
			}
			else if (!isForwards)
			{
				m_cursorCount = 0;
				m_image->setFrameX(m_cursorIndex--);
				if (m_cursorIndex <= 0) m_cursorIndex = 0;
				if (m_cursorIndex == 1) aniDelayTime = GetTickCount();
				if (m_cursorIndex == 0)
				{
					if (GetTickCount() - aniDelayTime >= 300)
					{
						isForwards = true;
					}
				}
			}
		}
	}

	if (isCommandStart)
	{
		m_commandCursorCount++;
		commandCursor->setFrameY(0);
		if (m_commandCursorCount % 4 == 0)
		{
			if (commandCursorIsForward)
			{
				m_commandCursorCount = 0;
				commandCursor->setFrameX(m_commandCursorIndex++);
				if (m_commandCursorIndex >= 4) m_commandCursorIndex = 4;
				if (m_commandCursorIndex == 3) aniDelayTime = GetTickCount();
				if (m_commandCursorIndex == 4)
				{
					if (GetTickCount() - aniDelayTime >= 200)
					{
						commandCursorIsForward = false;
					}
				}
			}
			else if (!commandCursorIsForward)
			{
				m_commandCursorCount = 0;
				commandCursor->setFrameX(m_commandCursorIndex--);
				if (m_commandCursorIndex <= 0) m_commandCursorIndex = 0;
				if (m_commandCursorIndex == 1) aniDelayTime = GetTickCount();
				if (m_commandCursorIndex == 0)
				{
					if (GetTickCount() - aniDelayTime >= 200)
					{
						commandCursorIsForward = true;
					}
				}
			}
		}
	}

}

void cursor::render()
{
	if (isTurning) return;
	if (!isBattleStart && !isEnemyTurn)
	{
		if (!isCommandStart)
		{
			m_image->frameRender(getMemDC(), curTile->getPosition().x - m_image->getFrameWidth() / 2 - CAMERA->m_cameraPos.x, curTile->getPosition().y - m_image->getFrameHeight() / 2 - CAMERA->m_cameraPos.y, m_image->getFrameX(), m_image->getFrameY());
		}
	}
	
	if (isCommandStart && !isBattleStart)
	{
		commandCursor->frameRender(getMemDC(), curCommand->position.x - 55, curCommand->position.y, commandCursor->getFrameX(), commandCursor->getFrameY());
	}
}

void cursor::release()
{
}

void cursor::control()
{	
	vector<unitSet*> l_allyUnitVector = UNITMANAGER->getAllyUnitVector();
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();
	int curTileIndex = curTile->getTileIndexX() + curTile->getTileIndexY() * tileMapSizeX;
	unitSet* tagUnit = curTile->getOnTileUnit();

	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		isDebug = !isDebug;
	}
	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		SCENEMANAGER->changeScene("��������1");
	}
	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		SCENEMANAGER->changeScene("��������2");
	}
	if (KEYMANAGER->isOnceKeyDown('3'))
	{
		SCENEMANAGER->changeScene("��������3");
	}
	if (cursorState == CURSOR_IDLE || cursorState == CURSOR_MOVETILESELECT || cursorState == CURSOR_ATTACKTILESELECT)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			if (curTile->getTileIndexX() <= 0)
			{
				return;
			}
			else
			{
				//����� ��Ÿ�������� Ÿ���� �Ұ��� ���� ���� ���̵� ������� ������� ���ְ� �� �ڿ�,
				if (curTile->getOnTileUnit() != nullptr)
				{
					curTile->getOnTileUnit()->setIsTargeted(false);
				}
				//Ŀ���̵�
				curTile = l_tileVector[curTileIndex - 1];

				if (m_position.x == 120 && CAMERA->m_cameraPos.x > 0) //Ŀ����ǥX�� ȭ��Ÿ�� ������ 3��°�϶� && //ī�޶��� X��ǥ�� ���� 0 ���� ũ�ٸ�
				{
					CAMERA->m_cameraPos.x -= 48;
					if (CAMERA->m_cameraPos.x <= 0) //
					{
						CAMERA->m_cameraPos.x = 0; // ī�޶��� X��ǥ = ���� �ִ�X - ȭ���� �ִ�X				
					}
				}
				else
				{
					if (tileMapSizeX * TILEX < WINSIZEX) //���� �ִ�X�� ȭ���ִ�X���� ���� ����
					{
						CAMERA->m_cameraPos.x = 0; //ī�޶�X��ǥ�� 0���� ����
						m_position.x -= 48; //Ŀ�� �����Ǹ� �Ű��ش�
					}

					else
					{
						m_position.x -= 48;
					}
				}
			}
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			if (curTile->getTileIndexX() >= tileMapSizeX - 1)
			{
				return;
			}
			else
			{
				if (curTile->getOnTileUnit() != nullptr)
				{
					curTile->getOnTileUnit()->setIsTargeted(false);
				}
				//Ŀ���̵�
				curTile = l_tileVector[curTileIndex + 1];

				if (m_position.x == 600 && CAMERA->m_cameraPos.x + WINSIZEX < tileMapSizeX * TILEX) //Ŀ����ǥX�� ȭ��Ÿ�� �ؿ��� 3��°�϶� && //ī�޶��� X��ǥ + ȭ���ִ�X�� ���� �ִ�X ���� �۴ٸ�
				{
					CAMERA->m_cameraPos.x += 48;
					if (CAMERA->m_cameraPos.x >= tileMapSizeX * TILEX - WINSIZEX)//ī�޶��� �������� ���� �ִ� ī�޶� ũ�⸦ �Ѿ��
					{
						CAMERA->m_cameraPos.x = tileMapSizeX * TILEX - WINSIZEX; // ī�޶��� X��ǥ = ���� �ִ�X - ȭ���� �ִ�X				
					}
				}
				else
				{
					if (TILEMANAGER->getMapMaxX() * TILEX < WINSIZEX) //���� �ִ�X�� ȭ���ִ�X���� ���� ����
					{
						CAMERA->m_cameraPos.x = 0; //ī�޶�X��ǥ�� 0���� ����
						m_position.x += 48; //Ŀ�� �����Ǹ� �Ű��ش�
					}

					else
					{
						m_position.x += 48;
					}
				}
			}


		}
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			if (curTile->getTileIndexY() <= 0)
			{
				return;
			}
			else
			{
				if (curTile->getOnTileUnit() != nullptr)
				{
					curTile->getOnTileUnit()->setIsTargeted(false);
				}
				//Ŀ���̵�
				curTile = l_tileVector[curTileIndex - tileMapSizeX];

				if (m_position.y == 120 && CAMERA->m_cameraPos.y > 0) //Ŀ����ǥY�� ȭ��Ÿ�� ������ 3��°�϶� && //ī�޶��� Y��ǥ�� ���� 0 ���� ũ�ٸ�
				{
					CAMERA->m_cameraPos.y -= 48;
					if (CAMERA->m_cameraPos.y <= 0) //
					{
						CAMERA->m_cameraPos.y = 0; // ī�޶��� Y��ǥ = ���� �ִ�Y - ȭ���� �ִ�Y				
					}
				}
				else
				{
					if (tileMapSizeY * TILEY < WINSIZEY) //���� �ִ�Y�� ȭ���ִ�Y���� ���� ����
					{
						CAMERA->m_cameraPos.y = 0; //ī�޶�Y��ǥ�� 0���� ����
						m_position.y -= 48; //Ŀ�� �����Ǹ� �Ű��ش�
					}

					else
					{
						m_position.y -= 48;
					}
				}

			}

		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			if (curTile->getTileIndexY() >= tileMapSizeY - 1) //���� Ÿ���� ������ Ÿ���̸� Ű�Է� ���� �ʴ´�
			{
				return;
			}
			else
			{
				if (curTile->getOnTileUnit() != nullptr)
				{
					curTile->getOnTileUnit()->setIsTargeted(false);
				}
				//Ŀ���̵�
				curTile = l_tileVector[curTileIndex + tileMapSizeX];

				if (m_position.y == 360 && CAMERA->m_cameraPos.y + WINSIZEY < tileMapSizeY * TILEY) //Ŀ����ǥY�� ȭ��Ÿ�� �ؿ��� 3��°�϶� && //ī�޶��� Y��ǥ + ȭ���ִ�Y�� ���� �ִ�Y ���� �۴ٸ�
				{
					CAMERA->m_cameraPos.y += 48;
					if (CAMERA->m_cameraPos.y >= tileMapSizeY * TILEY - WINSIZEY)//ī�޶��� �������� ���� �ִ� ī�޶� ũ�⸦ �Ѿ��
					{
						CAMERA->m_cameraPos.y = tileMapSizeY * TILEY - WINSIZEY; // ī�޶��� Y��ǥ = ���� �ִ�Y - ȭ���� �ִ�Y		
					}
				}
				else
				{
					if (tileMapSizeY * TILEY < WINSIZEY) //���� �ִ�Y�� ȭ���ִ�Y���� ���� ����
					{
						CAMERA->m_cameraPos.y = 0; //ī�޶�Y��ǥ�� 0���� ����
						m_position.y += 48; //Ŀ�� �����Ǹ� �Ű��ش�
					}

					else
					{
						m_position.y += 48;
					}
				}
			}
		}
	}
	
	switch (cursorState)
	{
	case CURSOR_IDLE:
		
		if (KEYMANAGER->isOnceKeyDown('A'))
		{
			if (tagUnit != nullptr)
			{
				if (tagUnit->getUnitDef() == DEF_ALLY)
				{
					if (!tagUnit->getIsTurnEnd())
					{
						//�� �Ʊ� ������ ����
						selectedUnit = tagUnit;
						//������ �ִ� Ÿ���� ����
						moveStartTile = curTile;
						ASTAR->setLastTile(curTile);
						//������ �ִ� Ÿ�� �������� �̵��Ÿ� ǥ��
						ASTAR->setAstarState(ASTAR_SHOWUNITMOVETILE);
						//�̵����� �����ؾ����� ǥ��
						selectMoveTile = true;
						//���̽�Ÿ�� ���������� ǥ��
						ASTAR->setStartTile(curTile);
						//������ ���õʺҰ��� ����
						tagUnit->setIsSelected(true);

						cursorState = CURSOR_MOVETILESELECT;
					}
				}				
			}
			else
			{
				//�ɼ�UI���
			}
		}
		
		break;

	case CURSOR_ATTACKTILESELECT:

		if (KEYMANAGER->isOnceKeyDown('A'))
		{
			if (curTile->getOnTileUnit() != nullptr && tagUnit->getUnitDef() == DEF_ENEMY)
			{
				//���ݹ����ȿ� �ִٸ�
				if (curTile->getShowRedTile())
				{
					//���ݾ����� ����UI ����
					UIMANAGER->getCommand()->setCommandState(COMMAND_ATKITEMCONFIRM);
					UIMANAGER->getCommand()->adjustCommandWindow();
					UIMANAGER->getCommand()->arrangementCommand();
					isCommandStart = true;
					curCommand = UIMANAGER->getCommand()->getItemBar();

					cursorState = CURSOR_COMMANDSELECT;
				}
			}
		}
		break;

	case CURSOR_MOVETILESELECT:

		if (KEYMANAGER->isOnceKeyDown('A'))
		{
			//�ڱ� �ڽ��̶��
			if (tagUnit == selectedUnit)
			{
				ASTAR->setLastTile(curTile);
				ASTAR->tileInitializing(curTile);
				while (true)
				{
					ASTAR->showAttackTile_addOpenList();
					if (ASTAR->showAttackTile_addCloseList()) break;
				}
				//���ֿ��� ����ϴ� UI����
				UIMANAGER->getCommand()->setCommandState(COMMAND_MOVECONFIRM);
				UIMANAGER->getCommand()->adjustCommandWindow();
				UIMANAGER->getCommand()->arrangementCommand();
				isCommandStart = true;
				curCommand = UIMANAGER->getCommand()->getCommandBar();

				cursorState = CURSOR_COMMANDSELECT;
			}

			//�̵����� ������ ���
			else if (curTile->getShowBlueTile())
			{
				if (tagUnit == nullptr)
				{
					ASTAR->setLastTile(curTile);
					ASTAR->setAstarState(ASTAR_UNITMOVE);
					//�̵��� ���� ������ ��Ʈ���� ����
					isStopControl = true;
				}	
				else
				{
				}
			}			
		}	
		break;

	case CURSOR_ITEMSELECT:
		break;

	case CURSOR_COMMANDSELECT:

		if (UIMANAGER->getCommand()->getCommandState() == COMMAND_MOVECONFIRM)
		{
			unitControl();
		}
		else if (UIMANAGER->getCommand()->getCommandState() == COMMAND_ATKITEMCONFIRM)
		{
			unitAtkControl();
		}
		break;

	default:
		break;

	}
	//���Ű�� ������ (�� �κ� �ӽ� ��ġ�� ���� ���Ƶξ���)
	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		if (ASTAR->getMovingUnit()->getUnitDef() == DEF_ALLY || selectedUnit->getUnitDef() == DEF_ALLY)
		{
			if (!ASTAR->getMovingUnit()->getIsTurnEnd())
			{
				//���õ� ������ ���õ��� ���� ��Ÿ �ʱ�ȭ
				if (selectedUnit != nullptr)
				{
					selectedUnit->setIsSelected(false);
				}
				selectMoveTile = false;
				moveStartTile = nullptr;
				for (int i = 0; i < l_tileVector.size(); i++)
				{
					l_tileVector[i]->setShowBlueTile(false);
					l_tileVector[i]->setShowRedTile(false);
				}
				ASTAR->cancelMove();
			}			
		}	
	}
	
}

void cursor::unitControl()
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		if (curCommand == UIMANAGER->getCommand()->getCommandBar())
		{
			curCommand = curCommand + (UIMANAGER->getCommand()->getMoveConfirmFrontNum() - 1);
		}
		else
		{
			curCommand = curCommand - 1;
		}		
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		if (curCommand->commandType == COMMAND_WAIT)
		{
			curCommand = UIMANAGER->getCommand()->getCommandBar();
		}
		else
		{
			curCommand = curCommand + 1;
		}	
	}

	//���� Ŀ���� �ִ� ���� ���ø���̸� ������ �����ش�
	if (curCommand->commandType == COMMAND_ATK)
	{
		ASTAR->setAstarState(ASTAR_SHOWUNITATTACKTILE);
	}
	else
	{
		for (int i = 0; i < l_tileVector.size(); i++)
		{
			l_tileVector[i]->setShowRedTile(false);
		}
	}

	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		//�����
		if (curCommand->commandType == COMMAND_WAIT)
		{
			ASTAR->finishMove();

		}		
		//���ݸ��
		if (curCommand->commandType == COMMAND_ATK)
		{
			ASTAR->attackCommand();
			selectAttackTile = true;
			isCommandStart = false;
			cursorState = CURSOR_ATTACKTILESELECT;
		}
		//�����۸��
		if (curCommand->commandType == COMMAND_ITEM)
		{
			selectItem = true;
			UIMANAGER->getCommand()->setCommandState(COMMAND_ATKITEMCONFIRM);
			UIMANAGER->getCommand()->adjustCommandWindow();
			UIMANAGER->getCommand()->arrangementCommand();
			isCommandStart = true;
			curCommand = UIMANAGER->getCommand()->getItemBar();
		}
	}
	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		curTile = moveStartTile;
		m_position = curTile->getPosition();
		ASTAR->cancelMove();
	}
}

void cursor::unitAtkControl()
{
	
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		if (curCommand == UIMANAGER->getCommand()->getItemBar())
		{
			curCommand = curCommand + (selectedUnit->getInven().size() - 1);
			menuNum = selectedUnit->getInven().size() - 1;
		}
		else
		{
			curCommand = curCommand - 1;
			menuNum -= 1;
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		if (curCommand == UIMANAGER->getCommand()->getItemBar() + selectedUnit->getInven().size() - 1)
		{
			curCommand = UIMANAGER->getCommand()->getItemBar();
			menuNum = 0;
		}
		else
		{
			curCommand = curCommand + 1;
			menuNum += 1;
		}
	}
	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		unitSet* tagUnit = curTile->getOnTileUnit();
		bool attackCheck = false;

		if (selectItem)	//�����ۻ��â�ϰ��
		{
			if (selectedUnit->getInven()[menuNum]->getItemType() == ITEM_WEAPON)	//�����̸� ����
			{
				selectedUnit->setEquiped(selectedUnit->getInven()[menuNum]);
				ASTAR->finishMove();
				selectItem = false;
			}
			else if (selectedUnit->getInven()[menuNum]->getItemType() == ITEM_CONSUMABLE)	//�Ҹ�ǰ�̸� ���
			{
				selectedUnit->setCurHp(selectedUnit->getCurHp() + selectedUnit->getInven()[menuNum]->getItemPower());
				if (selectedUnit->getCurHp() > selectedUnit->getMaxHp())
				{
					selectedUnit->setCurHp(selectedUnit->getMaxHp());
				}
				ASTAR->finishMove();
				selectItem = false;
			}
		}
		else   //�����ϰ��
		{
			if (selectedUnit->getInven()[menuNum]->getItemType() == ITEM_WEAPON)
			{
				//������ �޴� ������ ������ ����
				receiveUnit = tagUnit;
				
				
				if (getDistance(selectedUnit->getPosition().x, selectedUnit->getPosition().y,
					receiveUnit->getPosition().x, receiveUnit->getPosition().y) > 48)
				{
					if (selectedUnit->getInven()[menuNum]->getWeaponRng() >= 2)
					{
						attackCheck = true;
					}
				}
				else if(getDistance(selectedUnit->getPosition().x, selectedUnit->getPosition().y,
					receiveUnit->getPosition().x, receiveUnit->getPosition().y) <= 48)
				{					
						attackCheck = true;
				}

				if (attackCheck)
				{
					//������ ���⸦ ����
					selectedUnit->setEquiped(selectedUnit->getInven()[menuNum]);


					//ĳ������ �ޱ��� ���� ����ְ�
					selectedUnit->setUnitAngle(getAngle(selectedUnit->getPosition().x, selectedUnit->getPosition().y,
						receiveUnit->getPosition().x, receiveUnit->getPosition().y));
					receiveUnit->setUnitAngle(getAngle(receiveUnit->getPosition().x, receiveUnit->getPosition().y, selectedUnit->getPosition().
						x, selectedUnit->getPosition().y));
					//������ �ϱ�� �� ������ ������ ����(������ȯ��)
					selectedUnit->setIsAttack(true);
					//������ �ޱ�� �� ������ ������ ����(������ȯ��)
					receiveUnit->setIsAttack(true);
					//���� UI �ּ���
					isBattleStart = true;
					//���� �������� �����϶�
					ASTAR->setStartTime(GetTickCount());
					ASTAR->setAstarState(ASTAR_ATTACKSEQUENCE);
					//�츮�� ���ݺ��� �����϶�
					ASTAR->setIsAttackSequenceOne(true);
					//��Ʈ���� �����
					isStopControl = true;
					attackCheck = false;
				}
				
			}
		}
		
	}
	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		curTile = moveStartTile;
		ASTAR->cancelMove();
	}
}

void cursor::turnOverCheck()
{
	vector<unitSet*> l_allyUnitVector = UNITMANAGER->getAllyUnitVector();
	vector<unitSet*> l_enemyUnitVector = UNITMANAGER->getEnemyUnitVector();
	int i = 0;
	//�Ʊ� ������ ��
	if (isPlayerTurn)
	{
		//���� �ٲ�� ���� �ƴϸ�
		if (!isTurning)
		{			
			for (i; i < l_allyUnitVector.size(); i++)
			{
				//���� �� ������ �ʾ����� ���⼭ Ż��
				if (!l_allyUnitVector[i]->getIsTurnEnd()) break;
			}
			//���� �� �������� ����� ����
			if (i == l_allyUnitVector.size())
			{				
				//���� �ٲ���� �˷��ְ�
				UIMANAGER->getTrunAni()->setIsTurnChange(true);
				//�÷��̾��� ������ ������Ű��
				isStopControl = true;
				//�Ͽ� ���� ���¸� �ٽ� ������
				for (int j = 0; j < l_allyUnitVector.size(); j++)
				{
					l_allyUnitVector[j]->setIsTurnEnd(false);
				}
				//�ʹ� ���ϸ��̼� ����
				isTurning = true;				
			}
		}
		//���� �ٲ�� ���̸�
		else if (isTurning)
		{
			//�� ü���� �ִϸ��̼��� �����Ŀ� ���� �ٲ��ش�.
			if (!UIMANAGER->getTrunAni()->getIsTurnAnimation())
			{
				isEnemyTurn = true;
				isPlayerTurn = false;
				isTurning = false;
				ASTAR->setMovingUnit(nullptr);
			}
		}
	}
	else if (isEnemyTurn)
	{
		if (!isTurning)
		{
			for (i; i < l_enemyUnitVector.size(); i++)
			{
				if (!l_enemyUnitVector[i]->getIsTurnEnd()) break;
			}

			if (i == l_enemyUnitVector.size())
			{
				UIMANAGER->getTrunAni()->setIsTurnChange(true);				
				for (int j = 0; j < l_enemyUnitVector.size(); j++)
				{
					l_enemyUnitVector[j]->setIsTurnEnd(false);
					l_enemyUnitVector[j]->setIsTakingTurn(false);					
					l_enemyUnitVector[j]->setIsTargetFound(true);
					l_enemyUnitVector[j]->setIsSearchTarget(true);
				}				
				isTurning = true;
			}			
		}
		else if (isTurning)
		{
			if (!UIMANAGER->getTrunAni()->getIsTurnAnimation())
			{
				isEnemyTurn = false;
				CAMERA->m_cameraPos = CAMERA->m_preCameraPos;
				isPlayerTurn = true;
				isStopControl = false;
				isTurning = false;
				ASTAR->setMovingUnit(nullptr);
			}
		}	
	}
}

