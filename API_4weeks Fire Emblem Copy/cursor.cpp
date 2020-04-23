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
	m_image = IMAGEMANAGER->addFrameImage("커서", "images/cursor.bmp", 240, 120, 4, 2, true, RGB(255, 0, 255));
	commandCursor = IMAGEMANAGER->addFrameImage("커맨드커서", "images/UI/fingerCursor2.bmp", 285, 36, 5, 1, true, RGB(255, 0, 255));
	aniDelayTime = 0;

	cursorState = CURSOR_IDLE;
	moveStartTile = nullptr;	//캐릭의 이동이 시작된 타일
	selectedUnit = nullptr;	//선택한 캐릭
	receiveUnit = nullptr;	//타겟으로 선택한 캐릭

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
				
		//커서가 위치한 자리에 유닛이 있으면
		if (curTile->getOnTileUnit() != nullptr)
		{
			//그 유닛이 아군이고
			if (curTile->getOnTileUnit()->getUnitDef() == DEF_ALLY)
			{
				//턴이 끝나지 않았으면 유닛의 모션을 변경
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
	//유닛위에 커서가 올려져있을 시
	if (curTile->getOnTileUnit() != nullptr)
	{
		m_image->setFrameX(4);
	}
	//일반 커서 애니메이션
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
		SCENEMANAGER->changeScene("스테이지1");
	}
	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		SCENEMANAGER->changeScene("스테이지2");
	}
	if (KEYMANAGER->isOnceKeyDown('3'))
	{
		SCENEMANAGER->changeScene("스테이지3");
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
				//켜줬던 온타일유닛의 타겟팅 불값을 꺼서 원래 아이들 모션으로 변경먼저 해주고 그 뒤에,
				if (curTile->getOnTileUnit() != nullptr)
				{
					curTile->getOnTileUnit()->setIsTargeted(false);
				}
				//커서이동
				curTile = l_tileVector[curTileIndex - 1];

				if (m_position.x == 120 && CAMERA->m_cameraPos.x > 0) //커서좌표X가 화면타일 위에서 3번째일때 && //카메라의 X좌표가 맵의 0 보다 크다면
				{
					CAMERA->m_cameraPos.x -= 48;
					if (CAMERA->m_cameraPos.x <= 0) //
					{
						CAMERA->m_cameraPos.x = 0; // 카메라의 X좌표 = 맵의 최대X - 화면의 최대X				
					}
				}
				else
				{
					if (tileMapSizeX * TILEX < WINSIZEX) //맵의 최대X가 화면최대X보다 작을 때는
					{
						CAMERA->m_cameraPos.x = 0; //카메라X좌표는 0으로 고정
						m_position.x -= 48; //커서 포지션만 옮겨준다
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
				//커서이동
				curTile = l_tileVector[curTileIndex + 1];

				if (m_position.x == 600 && CAMERA->m_cameraPos.x + WINSIZEX < tileMapSizeX * TILEX) //커서좌표X가 화면타일 밑에서 3번째일때 && //카메라의 X좌표 + 화면최대X가 맵의 최대X 보다 작다면
				{
					CAMERA->m_cameraPos.x += 48;
					if (CAMERA->m_cameraPos.x >= tileMapSizeX * TILEX - WINSIZEX)//카메라의 시작점이 맵의 최대 카메라 크기를 넘어가면
					{
						CAMERA->m_cameraPos.x = tileMapSizeX * TILEX - WINSIZEX; // 카메라의 X좌표 = 맵의 최대X - 화면의 최대X				
					}
				}
				else
				{
					if (TILEMANAGER->getMapMaxX() * TILEX < WINSIZEX) //맵의 최대X가 화면최대X보다 작을 때는
					{
						CAMERA->m_cameraPos.x = 0; //카메라X좌표는 0으로 고정
						m_position.x += 48; //커서 포지션만 옮겨준다
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
				//커서이동
				curTile = l_tileVector[curTileIndex - tileMapSizeX];

				if (m_position.y == 120 && CAMERA->m_cameraPos.y > 0) //커서좌표Y가 화면타일 위에서 3번째일때 && //카메라의 Y좌표가 맵의 0 보다 크다면
				{
					CAMERA->m_cameraPos.y -= 48;
					if (CAMERA->m_cameraPos.y <= 0) //
					{
						CAMERA->m_cameraPos.y = 0; // 카메라의 Y좌표 = 맵의 최대Y - 화면의 최대Y				
					}
				}
				else
				{
					if (tileMapSizeY * TILEY < WINSIZEY) //맵의 최대Y가 화면최대Y보다 작을 때는
					{
						CAMERA->m_cameraPos.y = 0; //카메라Y좌표는 0으로 고정
						m_position.y -= 48; //커서 포지션만 옮겨준다
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
			if (curTile->getTileIndexY() >= tileMapSizeY - 1) //현재 타일이 마지막 타일이면 키입력 받지 않는다
			{
				return;
			}
			else
			{
				if (curTile->getOnTileUnit() != nullptr)
				{
					curTile->getOnTileUnit()->setIsTargeted(false);
				}
				//커서이동
				curTile = l_tileVector[curTileIndex + tileMapSizeX];

				if (m_position.y == 360 && CAMERA->m_cameraPos.y + WINSIZEY < tileMapSizeY * TILEY) //커서좌표Y가 화면타일 밑에서 3번째일때 && //카메라의 Y좌표 + 화면최대Y가 맵의 최대Y 보다 작다면
				{
					CAMERA->m_cameraPos.y += 48;
					if (CAMERA->m_cameraPos.y >= tileMapSizeY * TILEY - WINSIZEY)//카메라의 시작점이 맵의 최대 카메라 크기를 넘어가면
					{
						CAMERA->m_cameraPos.y = tileMapSizeY * TILEY - WINSIZEY; // 카메라의 Y좌표 = 맵의 최대Y - 화면의 최대Y		
					}
				}
				else
				{
					if (tileMapSizeY * TILEY < WINSIZEY) //맵의 최대Y가 화면최대Y보다 작을 때는
					{
						CAMERA->m_cameraPos.y = 0; //카메라Y좌표는 0으로 고정
						m_position.y += 48; //커서 포지션만 옮겨준다
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
						//그 아군 유닛을 저장
						selectedUnit = tagUnit;
						//유닛이 있는 타일을 저장
						moveStartTile = curTile;
						ASTAR->setLastTile(curTile);
						//유닛이 있는 타일 기준으로 이동거리 표시
						ASTAR->setAstarState(ASTAR_SHOWUNITMOVETILE);
						//이동범위 선택해야함을 표시
						selectMoveTile = true;
						//에이스타에 시작지점을 표시
						ASTAR->setStartTile(curTile);
						//유닛의 선택됨불값을 켜줌
						tagUnit->setIsSelected(true);

						cursorState = CURSOR_MOVETILESELECT;
					}
				}				
			}
			else
			{
				//옵션UI출력
			}
		}
		
		break;

	case CURSOR_ATTACKTILESELECT:

		if (KEYMANAGER->isOnceKeyDown('A'))
		{
			if (curTile->getOnTileUnit() != nullptr && tagUnit->getUnitDef() == DEF_ENEMY)
			{
				//공격범위안에 있다면
				if (curTile->getShowRedTile())
				{
					//공격아이템 선택UI 오픈
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
			//자기 자신이라면
			if (tagUnit == selectedUnit)
			{
				ASTAR->setLastTile(curTile);
				ASTAR->tileInitializing(curTile);
				while (true)
				{
					ASTAR->showAttackTile_addOpenList();
					if (ASTAR->showAttackTile_addCloseList()) break;
				}
				//유닛에게 명령하는 UI오픈
				UIMANAGER->getCommand()->setCommandState(COMMAND_MOVECONFIRM);
				UIMANAGER->getCommand()->adjustCommandWindow();
				UIMANAGER->getCommand()->arrangementCommand();
				isCommandStart = true;
				curCommand = UIMANAGER->getCommand()->getCommandBar();

				cursorState = CURSOR_COMMANDSELECT;
			}

			//이동범위 선택일 경우
			else if (curTile->getShowBlueTile())
			{
				if (tagUnit == nullptr)
				{
					ASTAR->setLastTile(curTile);
					ASTAR->setAstarState(ASTAR_UNITMOVE);
					//이동이 끝날 때까지 컨트롤을 멈춤
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
	//취소키를 누르면 (이 부분 임시 조치로 버그 막아두었음)
	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		if (ASTAR->getMovingUnit()->getUnitDef() == DEF_ALLY || selectedUnit->getUnitDef() == DEF_ALLY)
		{
			if (!ASTAR->getMovingUnit()->getIsTurnEnd())
			{
				//선택된 유닛의 선택됨을 끄고 기타 초기화
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

	//현재 커서가 있는 곳이 어택명령이면 범위를 보여준다
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
		//대기명령
		if (curCommand->commandType == COMMAND_WAIT)
		{
			ASTAR->finishMove();

		}		
		//공격명령
		if (curCommand->commandType == COMMAND_ATK)
		{
			ASTAR->attackCommand();
			selectAttackTile = true;
			isCommandStart = false;
			cursorState = CURSOR_ATTACKTILESELECT;
		}
		//아이템명령
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

		if (selectItem)	//아이템사용창일경우
		{
			if (selectedUnit->getInven()[menuNum]->getItemType() == ITEM_WEAPON)	//무기이면 장착
			{
				selectedUnit->setEquiped(selectedUnit->getInven()[menuNum]);
				ASTAR->finishMove();
				selectItem = false;
			}
			else if (selectedUnit->getInven()[menuNum]->getItemType() == ITEM_CONSUMABLE)	//소모품이면 사용
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
		else   //공격일경우
		{
			if (selectedUnit->getInven()[menuNum]->getItemType() == ITEM_WEAPON)
			{
				//공격을 받는 유닛은 선택한 적군
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
					//선택한 무기를 장착
					selectedUnit->setEquiped(selectedUnit->getInven()[menuNum]);


					//캐릭터의 앵글을 서로 잡아주고
					selectedUnit->setUnitAngle(getAngle(selectedUnit->getPosition().x, selectedUnit->getPosition().y,
						receiveUnit->getPosition().x, receiveUnit->getPosition().y));
					receiveUnit->setUnitAngle(getAngle(receiveUnit->getPosition().x, receiveUnit->getPosition().y, selectedUnit->getPosition().
						x, selectedUnit->getPosition().y));
					//공격을 하기로 한 유닛의 어택을 켜줌(방향전환용)
					selectedUnit->setIsAttack(true);
					//공격을 받기로 한 유닛의 어택을 켜줌(방향전환용)
					receiveUnit->setIsAttack(true);
					//공격 UI 주세요
					isBattleStart = true;
					//공격 시퀀스를 진행하라
					ASTAR->setStartTime(GetTickCount());
					ASTAR->setAstarState(ASTAR_ATTACKSEQUENCE);
					//우리편 공격부터 진행하라
					ASTAR->setIsAttackSequenceOne(true);
					//컨트롤을 멈춰라
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
	//아군 차례일 때
	if (isPlayerTurn)
	{
		//턴이 바뀌는 중이 아니면
		if (!isTurning)
		{			
			for (i; i < l_allyUnitVector.size(); i++)
			{
				//턴이 다 끝나지 않았으면 여기서 탈출
				if (!l_allyUnitVector[i]->getIsTurnEnd()) break;
			}
			//턴이 다 끝났으면 여기로 들어옴
			if (i == l_allyUnitVector.size())
			{				
				//턴이 바뀐것을 알려주고
				UIMANAGER->getTrunAni()->setIsTurnChange(true);
				//플레이어의 조작을 금지시키고
				isStopControl = true;
				//턴에 대한 상태를 다시 돌려줌
				for (int j = 0; j < l_allyUnitVector.size(); j++)
				{
					l_allyUnitVector[j]->setIsTurnEnd(false);
				}
				//터닝 에니메이션 실행
				isTurning = true;				
			}
		}
		//턴이 바뀌는 중이면
		else if (isTurning)
		{
			//턴 체인지 애니메이션이 끝난후에 턴을 바꿔준다.
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

