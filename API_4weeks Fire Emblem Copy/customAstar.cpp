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
			addOpenList();		//오픈리스트에 추가후G값계산
			calculateH();		//H계산
			calculateF();		//F계산
			addCloseList();		//클로즈리스트에 추가
			checkArrive();		//도착여부 체크
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

	CL_X = closeList[lastIndex]->getTileIndexX();			//마지막으로 추가된 클로즈 리스트의 X
	CL_Y = closeList[lastIndex]->getTileIndexY();
	CL_G = closeList[lastIndex]->getG();

	int tileIndex = CL_X + CL_Y * g_cursor.getTileMapSizeX();

	if (CL_Y != 0)	//0번째 줄이 아니라면 상단 라인계산
	{
		if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getTileType() == TILE_MOVABLE || movingUnit->getIsFlying())//상단 가운데 타일이 지나갈수 있다면
		{
			if (!l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getListOn())//오픈리스트에 포함이 안 되어 있는 타일이라면
			{
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setListOn(true); //오픈리스트에 포함되었다.
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setG(CL_G + 10);	//타일의 G값을 클로즈 리스트의 누적 G+10
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);//타일의 부모를 클로즈 리스트의 마지막으로 추가된 타일로 설정
				openList.push_back(l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]);//오픈리스트 벡터 추가
			}
			else//오픈리스트에 포함이 되어 있던 타일이라면
			{
				if (CL_G + 10 < l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getG())//기존 G값보다 새로 계산한 G값이 작다면
				{
					l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setG(CL_G + 10);//G값 새롭게 계산
					l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);//타일의 부모를 클로즈 리스트의 마지막으로 추가된 타일로 재설정
				}
			}
		}
	}

	if (CL_X != 0)//좌측 : 0번째 열이 아니라면
	{
		if (l_tileVector[tileIndex - 1]->getTileType() == TILE_MOVABLE || movingUnit->getIsFlying())//좌측 타일이 이동 가능하다면
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
				if (CL_G + 10 < l_tileVector[tileIndex - 1]->getG())//기존 G값보다 새로 계산한 G값이 작다면
				{
					l_tileVector[tileIndex - 1]->setG(CL_G + 10);//G값 새롭게 계산
					l_tileVector[tileIndex - 1]->setParent(closeList[lastIndex]);//타일의 부모를 클로즈 리스트의 마지막으로 추가된 타일로 재설정
				}
			}
		}
	}

	if (CL_X != g_cursor.getTileMapSizeX() - 1)//우측:마지막열이 아니라면
	{
		if (l_tileVector[tileIndex + 1]->getTileType() == TILE_MOVABLE || movingUnit->getIsFlying())//우측 타일이 이동가능하다면
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
				if (CL_G + 10 < l_tileVector[tileIndex + 1]->getG())//기존 G값보다 새로 계산한 G값이 작다면
				{
					l_tileVector[tileIndex + 1]->setG(CL_G + 10);//G값 새롭게 계산
					l_tileVector[tileIndex + 1]->setParent(closeList[lastIndex]);//타일의 부모를 클로즈 리스트의 마지막으로 추가된 타일로 재설정
				}
			}
		}
	}

	if (CL_Y != g_cursor.getTileMapSizeY() - 1)//마지막 행이 아니라면 하단 라인 계산
	{
		if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getTileType() == TILE_MOVABLE || movingUnit->getIsFlying())//하단 가운데 타일이 이동가능하다면
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
				if (CL_G + 10 < l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getG())//기존 G값보다 새로 계산한 G값이 작다면
				{
					l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setG(CL_G + 10);//G값 새롭게 계산
					l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);//타일의 부모를 클로즈 리스트의 마지막으로 추가된 타일로 재설정
				}
			}
		}
	}
}

void customAstar::calculateH()
{
	for (int i = 0; i < openList.size(); i++)
	{
		int vertical = (lastTile->getTileIndexX() - openList[i]->getTileIndexX()) * 10;//가로H 값
		int horizontal = (lastTile->getTileIndexY() - openList[i]->getTileIndexY()) * 10;//세로 H값


		if (vertical < 0)vertical *= -1;	//방향이 반대이면 음수부여
		if (horizontal < 0)horizontal *= -1;

		openList[i]->setH(vertical + horizontal);	//총 h값 :  가로+세로 H
	}
}

void customAstar::calculateF()
{
	for (int i = 0; i < openList.size(); i++)
	{
		openList[i]->setF(openList[i]->getG() + openList[i]->getH()); //F값계산
	}
}

void customAstar::addCloseList()
{
	if (openList.size() == 0)//검색했는데도openList의 사이즈가 0이라면 더이상 찾을것이 없음
	{
		return;								//이하 작업 무시
	}
	int index = 0;				//오픈리스트 중 가장 F가 작은 타일의 인덱스
	int lowest = BIGNUM;		//오픈리스트 중 가장 작은 F값

	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i]->getF() < lowest)	//오픈리스트의 F가 가장작은거보다 더 작다면
		{
			lowest = openList[i]->getF();	//lowest를 그 F로
			index = i;					//index를 그 index로	
		}
	}

	closeList.push_back(openList[index]);		//클로즈 리스트에 추가
	openList.erase(openList.begin() + index);	//클로즈리스트에 추가된 타일은 오픈리스트에서 제외


	lastIndex++;	//가장 나중에 추가된 클로즈의 인덱스
}

void customAstar::checkArrive()
{
	if (closeList[lastIndex]->getTileIndexY() == lastTile->getTileIndexY() &&
		closeList[lastIndex]->getTileIndexX() == lastTile->getTileIndexX())//클로즈 리스트의 x,y가 도착지점과 같다면
	{
		storeWay(closeList[lastIndex]);			//경로를 스택에 저장
		lastTile = closeList[lastIndex];
	}
}

void customAstar::storeWay(tile * _tile)
{	
	moveList.push(_tile);   //스택에 경로저장	
	
	_tile = _tile->getParent();	//타일의 부모를 참조해서 storeWay()함수에다시 넣는다.

	if (_tile == startTile) //시작점까지 왔으면 그만
	{		
		moveList.push(_tile);
		//시작지점에 있는 유닛을 이동할 유닛변수에 저장
		movingUnit = moveList.top()->getOnTileUnit();
		//이동할 유닛의 이동을 켜줌
		movingUnit->setIsMove(true);
		//커서쪽에 에이스타 이동시작 함수의 시작을 알림
		isStartMoving = true;
		isReadyNextMove = true;
		return;
	}
	else
	{
		storeWay(_tile);		//다시 호출
	}		
}

void customAstar::startMove()
{ 
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	//다음 이동할 타일정보를 구함
	if (isReadyNextMove)
	{
		//이동처리가 끝난 타일을 스택에서 제거
		moveList.pop();	
		//이동처리할 타일이 더이상 없다면
		if (moveList.empty())
		{
			//유닛의 이동을 꺼주고
			movingUnit->setIsMove(false);
			//도착지점에 온 것이므로 도착지점의 중앙으로 유닛을 배치
			movingUnit->setPosition(PointFloatMake(lastTile->getPosition().x, lastTile->getPosition().y));
			//이동유닛의 정보를 도착지점에 넘겨줌
			lastTile->setOnTileUnit(movingUnit);
			//이동유닛의 시작점 정보를 지워줌
			closeList[0]->setOnTileUnit(nullptr);
			//이동 끝을 알림
			isStartMoving = false;
			m_AstarState = ASTAR_IDLE;

			//도착하면 이동표시 타일은 꺼준다
			for (int i = 0; i < l_tileVector.size(); i++)
			{
				l_tileVector[i]->setShowBlueTile(false);
			}

			//우리편일경우 컨트롤 UI를 오픈
			if (movingUnit->getUnitDef() == DEF_ALLY)
			{				
				//적이 근처에 있는지 검사
				tileInitializing(lastTile);
				while (true)
				{
					showAttackTile_addOpenList();
					if(showAttackTile_addCloseList()) break;								
				}
				//컨트롤 활성화
				g_cursor.setIsStopControl(false);
				//유닛에게 명령하는 UI오픈		
				UIMANAGER->getCommand()->setCommandState(COMMAND_MOVECONFIRM);
				UIMANAGER->getCommand()->adjustCommandWindow();
				UIMANAGER->getCommand()->arrangementCommand();
				isCommandStart = true;
				g_cursor.setCurCommand(UIMANAGER->getCommand()->getCommandBar());
				g_cursor.setCursorState(CURSOR_COMMANDSELECT);
			}
			//적일 경우 대기하거나 공격
			else if (movingUnit->getUnitDef() == DEF_ENEMY)
			{
				//적일경우 이동후의 위치에서 적탐색을 위해서 캐릭터의 소유타일을 바꿔줌
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
		//유닛의 턴을 꺼주고
		movingUnit->setIsTurnEnd(true);
		movingUnit->setIsTakingTurn(false);
		movingUnit->setIsPickTarget(false);
		movingUnit->setIsTargeted(false);
		//무빙 유닛을 비워주고
		movingUnit = nullptr;
	}	
	//에이스타 이동함수의 끝을 커서쪽에 알림
	g_cursor.setSelectMoveTile(false);
	//이동에 사용했던 타일의 이동 정보 초기화
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

	//유아이창 크기 조절 초기화
	UIMANAGER->getCommand()->setMoveConfirmFrontNum(2);
	UIMANAGER->getCommand()->setIsAtkPossible(false);
	g_cursor.setCursorState(CURSOR_IDLE);
}

void customAstar::cancelMove()
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	if (movingUnit != nullptr)
	{
		//이동유닛을 시작점으로 돌려놈
		movingUnit->setPosition(PointFloatMake(startTile->getPosition().x, startTile->getPosition().y));
		//이동유닛의 정보를 시작지점에 넘겨줌
		startTile->setOnTileUnit(movingUnit);
	}

	//에이스타 이동함수의 끝을 커서쪽에 알림
	g_cursor.setSelectMoveTile(false);
	
	
	if (startTile != lastTile)
	{
		//이동유닛의 도착점 정보를 지워줌
		lastTile->setOnTileUnit(nullptr);
	}
	
	
	g_cursor.setSelectAttackTile(false);

	//이동에 사용했던 타일의 이동 정보 초기화
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

	//유아이창 크기 조절 초기화
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
	//필요한 계산 정리
	int SU_HitChance = (g_cursor.getSelectedUnit()->getHit() - g_cursor.getReceiveUnit()->getAvoid());
	int RU_HitChance = (g_cursor.getReceiveUnit()->getHit() - g_cursor.getSelectedUnit()->getAvoid());
	int SU_Damage = (g_cursor.getSelectedUnit()->getAtk() - g_cursor.getReceiveUnit()->getDef());
	int RU_Damage = (g_cursor.getReceiveUnit()->getAtk() - g_cursor.getSelectedUnit()->getDef());
	int hitDice = RND->getInt(100);
	int critChance = RND->getInt(100);
	if (isAttackSequenceOne)
	{
		//명중 계산
		if (hitDice > SU_HitChance) //빗나간 경우
		{
			EFFECTMANAGER->play("빗나감", g_cursor.getReceiveUnit()->getPosition().x - CAMERA->m_cameraPos.x, g_cursor.getReceiveUnit()->getPosition().y - CAMERA->m_cameraPos.y);
		}
		else if (hitDice <= SU_HitChance) //명중한 경우
		{
			if (critChance < g_cursor.getSelectedUnit()->getCrit())
			{
				g_cursor.getReceiveUnit()->setCurHp(g_cursor.getReceiveUnit()->getCurHp() - (SU_Damage + 5));
				EFFECTMANAGER->play("명중크리", g_cursor.getReceiveUnit()->getPosition().x - CAMERA->m_cameraPos.x, g_cursor.getReceiveUnit()->getPosition().y - CAMERA->m_cameraPos.y);
			}
			else
			{
				g_cursor.getReceiveUnit()->setCurHp(g_cursor.getReceiveUnit()->getCurHp() - (SU_Damage));
				EFFECTMANAGER->play("명중함", g_cursor.getReceiveUnit()->getPosition().x - CAMERA->m_cameraPos.x, g_cursor.getReceiveUnit()->getPosition().y - CAMERA->m_cameraPos.y);
			}
		}
		else
		{
			EFFECTMANAGER->play("빗나감", 500, 500);
		}
		//죽었는지 체크하고 죽었으면 반격은 없음
		if (g_cursor.getReceiveUnit()->getCurHp() <= 0)
		{
			g_cursor.getReceiveUnit()->setIsDead(true);
			g_cursor.getReceiveUnit()->getOccupiedTile()->setOnTileUnit(nullptr);
			sequenceCount = 2;
		}
		//대상이 들고있는 무기의 사정거리를 체크했을 때 자신보다 낮고(&&) 바로 옆이 아니라면 반격은 없음
		if (g_cursor.getSelectedUnit()->getEquiped()->getWeaponRng() > g_cursor.getReceiveUnit()->getEquiped()->getWeaponRng() &&
			getDistance(g_cursor.getSelectedUnit()->getPosition().x, g_cursor.getSelectedUnit()->getPosition().y,
				g_cursor.getReceiveUnit()->getPosition().x, g_cursor.getReceiveUnit()->getPosition().y) > TILEX)
		{
			sequenceCount = 2;
		}
		//대상의 타입을 판별했을때 궁수이고(&&) 바로 옆에서 공격했다면 반격은 없음
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
		//명중 계산
		if (hitDice > RU_HitChance) //빗나간 경우
		{
			EFFECTMANAGER->play("빗나감", g_cursor.getSelectedUnit()->getPosition().x - CAMERA->m_cameraPos.x, g_cursor.getSelectedUnit()->getPosition().y - CAMERA->m_cameraPos.y);
		}
		else if (hitDice <= RU_HitChance) //명중한 경우
		{
			if (critChance < g_cursor.getReceiveUnit()->getCrit())
			{
				g_cursor.getSelectedUnit()->setCurHp(g_cursor.getSelectedUnit()->getCurHp() - (RU_Damage + 5));
				EFFECTMANAGER->play("명중크리", g_cursor.getSelectedUnit()->getPosition().x - CAMERA->m_cameraPos.x, g_cursor.getSelectedUnit()->getPosition().y - CAMERA->m_cameraPos.y);
			}
			else
			{
				g_cursor.getSelectedUnit()->setCurHp(g_cursor.getSelectedUnit()->getCurHp() - RU_Damage);
				EFFECTMANAGER->play("명중함", g_cursor.getSelectedUnit()->getPosition().x - CAMERA->m_cameraPos.x, g_cursor.getSelectedUnit()->getPosition().y - CAMERA->m_cameraPos.y);
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
		//공격을 하기로 한 유닛의 어택을 꺼줌(방향전환용)
		g_cursor.getSelectedUnit()->setIsAttack(false);
		//공격을 받기로 한 유닛의 어택을 꺼줌(방향전환용)
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
			//컨트롤을 다시 활성화
			g_cursor.setIsStopControl(false);
		}
		ASTAR->finishMove();
		//누군가 죽음 체크
		UNITMANAGER->deadCheck();
	}

}

void customAstar::showAvailableTile_addOpenList()
{
	vector<tile*>l_tileVector = TILEMANAGER->getTileVector();

	CL_X = closeList[lastIndex]->getTileIndexX();			//마지막으로 추가된 클로즈 리스트의 X
	CL_Y = closeList[lastIndex]->getTileIndexY();
	CL_G = closeList[lastIndex]->getG();

	int tileIndex = CL_X + CL_Y * g_cursor.getTileMapSizeX();
	bool MovableCheck = false;

	if (CL_Y != 0)	//0번째 줄이 아니라면 상단 라인계산
	{
		if (movingUnit->getIsFlying())
		{
			MovableCheck = true;
		}
		else if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getTileType() == TILE_MOVABLE)//상단 가운데 타일이 지나갈수 있고
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
			if (!l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getListOn())//오픈리스트에 포함이 안 되어 있는 타일이라면
			{
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setListOn(true); //오픈리스트에 포함되었다.
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setShowBlueTile(true); //이동가능타일 표시
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setG(CL_G + 10);	//타일의 G값을 클로즈 리스트의 누적 G+10
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);//타일의 부모를 클로즈 리스트의 마지막으로 추가된 타일로 설정
				openList.push_back(l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]);//오픈리스트 벡터 추가
			}
			else//오픈리스트에 포함이 되어 있던 타일이라면
			{
			}
			MovableCheck = false;
		}
	}

	if (CL_X != 0)//좌측 : 0번째 열이 아니라면
	{
		if (movingUnit->getIsFlying())
		{
			MovableCheck = true;
		}
		else if (l_tileVector[tileIndex - 1]->getTileType() == TILE_MOVABLE)//좌측 타일이 이동 가능하다면
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

	if (CL_X != g_cursor.getTileMapSizeX() - 1)//우측:마지막열이 아니라면
	{
		if (movingUnit->getIsFlying())
		{
			MovableCheck = true;
		}
		else if (l_tileVector[tileIndex + 1]->getTileType() == TILE_MOVABLE)//우측 타일이 이동가능하다면
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

	if (CL_Y != g_cursor.getTileMapSizeY() - 1)//마지막 행이 아니라면 하단 라인 계산
	{
		if (movingUnit->getIsFlying())
		{
			MovableCheck = true;
		}
		else if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getTileType() == TILE_MOVABLE)//하단 가운데 타일이 이동가능하다면
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
	if (openList.size() == 0)//검색했는데도openList의 사이즈가 0이라면 더이상 찾을것이 없음
	{
		m_AstarState = ASTAR_IDLE;	//이동범위 체크하는 로직 스탑
		return;									//이하 작업 무시
	}
	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i]->getG() < g_cursor.getSelectedUnit()->getMove() * 10)	//오픈리스트의 G가 이동력*10보다 작다면
		{
			closeList.push_back(openList[i]);
			openList.erase(openList.begin() + i);
			lastIndex++;			//가장 나중에 추가된 클로즈의 인덱스	

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

	CL_X = closeList[lastIndex]->getTileIndexX();			//마지막으로 추가된 클로즈 리스트의 X
	CL_Y = closeList[lastIndex]->getTileIndexY();
	CL_G = closeList[lastIndex]->getG();

	int tileIndex = CL_X + CL_Y * g_cursor.getTileMapSizeX();

	if (CL_Y != 0)	//0번째 줄이 아니라면 상단 라인계산
	{	
		if (!l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getListOn())//오픈리스트에 포함이 안 되어 있는 타일이라면
		{
			l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setListOn(true); //오픈리스트에 포함시킨다.

			if (m_AstarState != ASTAR_SHOWUNITATTACKTILE) //적을 검사하는 로직으로 사용중이라면
			{
				if (movingUnit->getUnitDef() == DEF_ALLY) //아군이 사용중이라면
				{
					if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getOnTileUnitType() == DEF_ENEMY)
					{
						UIMANAGER->getCommand()->setIsAtkPossible(true);
					}
				}			
				else if (movingUnit->getUnitDef() == DEF_ENEMY) //적군이 사용중이라면
				{
					if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getOnTileUnitType() == DEF_ALLY) //검색지점에 적이 있고
					{
						if (l_tileVector[tileIndex] == movingUnit->getOccupiedTile() && movingUnit->getUnitType() == TYPE_ARCHER) 
						{
							//검색지점이 자기자신이고(&&) 자신이 궁수라면				
							//검색지점이 자신의 위치라면 바로옆에 적이 있다는 의미이므로 인식하지않음												
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
			else if(m_AstarState == ASTAR_SHOWUNITATTACKTILE)//공격범위 표시중이라면
			{
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setShowRedTile(true); //공격가능타일 표시
			}			

			l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setG(CL_G + 10);	//타일의 G값을 클로즈 리스트의 누적 G+10
			l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);//타일의 부모를 클로즈 리스트의 마지막으로 추가된 타일로 설정
			openList.push_back(l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]);//오픈리스트 벡터 추가
		}
		else//오픈리스트에 포함이 되어 있던 타일이라면
		{
		}		
	}

	if (CL_X != 0)//좌측 : 0번째 열이 아니라면
	{		
		if (!l_tileVector[tileIndex - 1]->getListOn())
		{
			l_tileVector[tileIndex - 1]->setListOn(true);

			if (m_AstarState != ASTAR_SHOWUNITATTACKTILE) 
			{
				if (movingUnit->getUnitDef() == DEF_ALLY) //아군이 사용중이라면
				{
					if (l_tileVector[tileIndex - 1]->getOnTileUnitType() == DEF_ENEMY)
					{
						UIMANAGER->getCommand()->setIsAtkPossible(true);
					}
				}
				else if (movingUnit->getUnitDef() == DEF_ENEMY) //적군이 사용중이라면
				{
					if (l_tileVector[tileIndex - 1]->getOnTileUnitType() == DEF_ALLY)
					{
						if (l_tileVector[tileIndex] == movingUnit->getOccupiedTile() && movingUnit->getUnitType() == TYPE_ARCHER)
						{
							//검색지점이 자기자신이고(&&) 자신이 궁수라면				
							//검색지점이 자신의 위치라면 바로옆에 적이 있다는 의미이므로 인식하지않음												
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
			
			else if (m_AstarState == ASTAR_SHOWUNITATTACKTILE)//공격범위 표시중이라면
			{
				l_tileVector[tileIndex - 1]->setShowRedTile(true); //공격가능타일 표시
			}

			l_tileVector[tileIndex - 1]->setG(CL_G + 10);
			l_tileVector[tileIndex - 1]->setParent(closeList[lastIndex]);
			openList.push_back(l_tileVector[tileIndex - 1]);
		}
		else
		{
		}	
	}

	if (CL_X != g_cursor.getTileMapSizeX() - 1)//우측:마지막열이 아니라면
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
				else if (movingUnit->getUnitDef() == DEF_ENEMY) //적군이 사용중이라면
				{
					if (l_tileVector[tileIndex + 1]->getOnTileUnitType() == DEF_ALLY)
					{
						if (l_tileVector[tileIndex] == movingUnit->getOccupiedTile() && movingUnit->getUnitType() == TYPE_ARCHER)
						{
							//검색지점이 자기자신이고(&&) 자신이 궁수라면				
							//검색지점이 자신의 위치라면 바로옆에 적이 있다는 의미이므로 인식하지않음												
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
			else if (m_AstarState == ASTAR_SHOWUNITATTACKTILE)//공격범위 표시중이라면
			{
				l_tileVector[tileIndex + 1]->setShowRedTile(true); //공격가능타일 표시
			}

			l_tileVector[tileIndex + 1]->setG(CL_G + 10);
			l_tileVector[tileIndex + 1]->setParent(closeList[lastIndex]);
			openList.push_back(l_tileVector[tileIndex + 1]);
		}
		else
		{
		}		
	}

	if (CL_Y != g_cursor.getTileMapSizeY() - 1)//마지막 행이 아니라면 하단 라인 계산
	{			
		if (!l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getListOn())
		{
			l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setListOn(true);

			if (m_AstarState != ASTAR_SHOWUNITATTACKTILE) //적을 검사하는 과정중이었으면
			{
				if (movingUnit->getUnitDef() == DEF_ALLY)
				{
					if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getOnTileUnitType() == DEF_ENEMY)
					{
						UIMANAGER->getCommand()->setIsAtkPossible(true);
					}
				}	
				else if (movingUnit->getUnitDef() == DEF_ENEMY) //적군이 사용중이라면
				{
					if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getOnTileUnitType() == DEF_ALLY)
					{
						if (l_tileVector[tileIndex] == movingUnit->getOccupiedTile() && movingUnit->getUnitType() == TYPE_ARCHER)
						{
							//검색지점이 자기자신이고(&&) 자신이 궁수라면				
							//검색지점이 자신의 위치라면 바로옆에 적이 있다는 의미이므로 인식하지않음												
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
			else if (m_AstarState == ASTAR_SHOWUNITATTACKTILE)//공격범위 표시중이라면
			{
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setShowRedTile(true); //공격가능타일 표시
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
	if (openList.size() == 0)//검색했는데도openList의 사이즈가 0이라면 더이상 찾을것이 없음
	{
		if (m_AstarState == ASTAR_SHOWUNITATTACKTILE)
		{
			m_AstarState = ASTAR_IDLE;	//공격범위 체크하는 로직 스탑
			return true;									//이하 작업 무시
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
		if (openList[i]->getG() < g_cursor.getSelectedUnit()->getRng() * 10)	//오픈리스트의 G가 사정거리*10보다 작다면
		{
			closeList.push_back(openList[i]);
			openList.erase(openList.begin() + i);
			lastIndex++;			//가장 나중에 추가된 클로즈의 인덱스	

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

	CL_X = closeList[lastIndex]->getTileIndexX();			//마지막으로 추가된 클로즈 리스트의 X
	CL_Y = closeList[lastIndex]->getTileIndexY();
	CL_G = closeList[lastIndex]->getG();

	int tileIndex = CL_X + CL_Y * g_cursor.getTileMapSizeX();

	if (CL_Y != 0)	//0번째 줄이 아니라면 상단 라인계산
	{			
		if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getTileType() == TILE_MOVABLE
			|| movingUnit->getIsFlying())//상단 가운데 타일이 지나갈수 있거나 날아다니는 유닛이라면
		{
			if (!l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getListOn())//오픈리스트에 포함이 안 되어 있는 타일이라면
			{
 				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setListOn(true); //오픈리스트에 포함되었다.
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setG(CL_G + 10);	//타일의 G값을 클로즈 리스트의 누적 G+10
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);//타일의 부모를 클로즈 리스트의 마지막으로 추가된 타일로 설정
				openList.push_back(l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]);//오픈리스트 벡터 추가								
				l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->setShowBlueTile(true);
			}
			if (movingUnit->getUnitType() != TYPE_ARCHER) //궁수가아니라면 그 자리의 적을 검사
			{
				//그곳에 유닛이 있고 
				if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getOnTileUnit() != nullptr)
				{
					//그게 적이면
					if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX()]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
					{
						//검색위치가 비어있을 경우
						if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
						{
							isAI_TargetFound = true;
							lastTile = l_tileVector[tileIndex];
						}
					}
				}
			}
			else if (movingUnit->getUnitType() == TYPE_ARCHER) //궁수라면 그 자리를 검사시작점에서 한칸 떨어진곳의 적을 검사
			{
				if (CL_Y != 1) //1번째 줄이 아니라면 검사
				{
					if (l_tileVector[tileIndex - (g_cursor.getTileMapSizeX() * 2)]->getOnTileUnit() != nullptr)
					{
						//그게 적이면
						if (l_tileVector[tileIndex - (g_cursor.getTileMapSizeX() * 2)]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
						{
							//검색위치가 비어있을 경우
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
		if (movingUnit->getUnitType() == TYPE_ARCHER) //궁수라면 좌상우상도 검사
		{
			if (CL_X != 0)//좌상단 :  0번째 열이 아니라면
			{
				//그곳에 유닛이 있고 
				if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX() - 1]->getOnTileUnit() != nullptr)
				{
					//그게 적이면
					if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX() - 1]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
					{
						//검색위치가 비어있을 경우
						if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
						{
							isAI_TargetFound = true;
							lastTile = l_tileVector[tileIndex];
						}
					}
				}
			}
			if (CL_X != g_cursor.getTileMapSizeX() - 1)//우상단 :  마지막 열이 아니라면
			{
				//그곳에 유닛이 있고 
				if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX() + 1]->getOnTileUnit() != nullptr)
				{
					//그게 적이면
					if (l_tileVector[tileIndex - g_cursor.getTileMapSizeX() + 1]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
					{
						//검색위치가 비어있을 경우
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

	if (CL_X != 0)//좌측 : 0번째 열이 아니라면
	{			
		if (l_tileVector[tileIndex - 1]->getTileType() == TILE_MOVABLE || movingUnit->getIsFlying())//좌측 타일이 이동 가능하다면
		{
			if (!l_tileVector[tileIndex - 1]->getListOn())
			{
				l_tileVector[tileIndex - 1]->setListOn(true);
				l_tileVector[tileIndex - 1]->setG(CL_G + 10);
				l_tileVector[tileIndex - 1]->setParent(closeList[lastIndex]);
				openList.push_back(l_tileVector[tileIndex - 1]);
				l_tileVector[tileIndex - 1]->setShowBlueTile(true);
			}
			if (movingUnit->getUnitType() != TYPE_ARCHER) //궁수가아니라면 그 자리의 적을 검사
			{
				//그곳에 유닛이 있고 
				if (l_tileVector[tileIndex - 1]->getOnTileUnit() != nullptr)
				{
					//그곳에 적이 있고 
					if (l_tileVector[tileIndex - 1]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
					{
						//검색위치가 비어있을 경우
						if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
						{
							isAI_TargetFound = true;
							lastTile = l_tileVector[tileIndex];
						}
					}
				}
			}
			else if (movingUnit->getUnitType() == TYPE_ARCHER) //궁수라면 그 자리를 검사시작점에서 한칸 떨어진곳의 적을 검사
			{
				if (CL_X != 1) //1번째 줄이 아니라면 검사
				{
					if (l_tileVector[tileIndex - 2]->getOnTileUnit() != nullptr)
					{
						//그게 적이면
						if (l_tileVector[tileIndex - 2]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
						{
							//검색위치가 비어있을 경우
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

	if (CL_X != g_cursor.getTileMapSizeX() - 1)//우측:마지막열이 아니라면
	{			
		if (l_tileVector[tileIndex + 1]->getTileType() == TILE_MOVABLE || movingUnit->getIsFlying())//우측 타일이 이동가능하다면
		{
			if (!l_tileVector[tileIndex + 1]->getListOn())
			{
				l_tileVector[tileIndex + 1]->setListOn(true);
				l_tileVector[tileIndex + 1]->setG(CL_G + 10);
				l_tileVector[tileIndex + 1]->setParent(closeList[lastIndex]);
				openList.push_back(l_tileVector[tileIndex + 1]);
				l_tileVector[tileIndex + 1]->setShowBlueTile(true);
			}
			if (movingUnit->getUnitType() != TYPE_ARCHER) //궁수가아니라면 그 자리의 적을 검사
			{
				//그곳에 유닛이 있고 
				if (l_tileVector[tileIndex + 1]->getOnTileUnit() != nullptr)
				{
					//그곳에 적이 있고 
					if (l_tileVector[tileIndex + 1]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
					{
						//검색위치가 비어있을 경우
						if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
						{
							isAI_TargetFound = true;
							lastTile = l_tileVector[tileIndex];
						}
					}
				}
			}
			else if (movingUnit->getUnitType() == TYPE_ARCHER) //궁수라면 그 자리를 검사시작점에서 한칸 떨어진곳의 적을 검사
			{
				if (CL_X != g_cursor.getTileMapSizeX() - 2) //우측 마지막 이전 줄이 아니라면 검사
				{
					if (l_tileVector[tileIndex + 2]->getOnTileUnit() != nullptr)
					{
						//그게 적이면
						if (l_tileVector[tileIndex + 2]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
						{
							//검색위치가 비어있을 경우
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

	if (CL_Y != g_cursor.getTileMapSizeY() - 1)//마지막 행이 아니라면 하단 라인 계산
	{				
		if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getTileType() == TILE_MOVABLE
			|| movingUnit->getIsFlying())//하단 가운데 타일이 이동가능하다면
		{
			if (!l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getListOn())
			{
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setListOn(true);
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setG(CL_G + 10);
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setParent(closeList[lastIndex]);
				openList.push_back(l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]);
				l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->setShowBlueTile(true);
			}
			if (movingUnit->getUnitType() != TYPE_ARCHER) //궁수가아니라면 그 자리의 적을 검사
			{
				if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getOnTileUnit() != nullptr)
				{
					//그곳에 적이 있고 
					if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX()]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
					{
						//검색위치가 비어있을 경우
						if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
						{
							isAI_TargetFound = true;
							lastTile = l_tileVector[tileIndex];
						}
					}
				}
			}
			else if (movingUnit->getUnitType() == TYPE_ARCHER) //궁수라면 그 자리를 검사시작점에서 한칸 떨어진곳의 적을 검사
			{
				if (CL_Y != g_cursor.getTileMapSizeY() - 2) //하단 마지막 이전 줄이 아니라면 검사
				{
					if (l_tileVector[tileIndex + (g_cursor.getTileMapSizeX() * 2)]->getOnTileUnit() != nullptr)
					{
						//그게 적이면
						if (l_tileVector[tileIndex + (g_cursor.getTileMapSizeX() * 2)]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
						{
							//검색위치가 비어있을 경우
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
		if (CL_X != 0)	//좌하단  : 0번째 열이 아니라면
		{
			if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX() - 1]->getOnTileUnit() != nullptr)
			{
				//그게 적이면
				if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX() - 1]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
				{
					//검색위치가 비어있을 경우
					if (l_tileVector[tileIndex]->getOnTileUnit() == nullptr)
					{
						isAI_TargetFound = true;
						lastTile = l_tileVector[tileIndex];
					}
				}
			}
		}
		if (CL_X != g_cursor.getTileMapSizeX() - 1)//우하단 :마지막 열이 아니라면
		{
			if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX() + 1]->getOnTileUnit() != nullptr)
			{
				//그게 적이면
				if (l_tileVector[tileIndex + g_cursor.getTileMapSizeX() + 1]->getOnTileUnit()->getUnitDef() == DEF_ALLY)
				{
					//검색위치가 비어있을 경우
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
		return true;									//이하 작업 무시
	}
	if (openList.size() == 0)//검색했는데도openList의 사이즈가 0이라면 더이상 찾을것이 없음
	{
		return true;									//이하 작업 무시
	}
	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i]->getG() <= g_cursor.getSelectedUnit()->getMove() * 10) //픽타겟을 위한 검색
		{
			enemySearchList.push_back(openList[i]); //적을 공격하지 못했을 때 이동 위치 검색리스트 벡터 추가
		}
		if (openList[i]->getG() < g_cursor.getSelectedUnit()->getMove() * 10) //정리
		{
			closeList.push_back(openList[i]);
			openList.erase(openList.begin() + i);
			lastIndex++;			//가장 나중에 추가된 클로즈의 인덱스	

			break;
		}
		if (openList[i]->getG() >= g_cursor.getSelectedUnit()->getMove() * 10)
		{
			openList.erase(openList.begin() + i);
		}
	}
	return false;
}



