#pragma once
#include "singletonBase.h"

class tile;
class unitSet;
class itemSet;
class UI_command;
enum ASTAR_STATE
{
	ASTAR_IDLE,
	ASTAR_UNITMOVE,
	ASTAR_SHOWUNITMOVETILE,
	ASTAR_SHOWUNITATTACKTILE,
	ASTAR_SEARCHINGTARGET,
	ASTAR_ATTACKSEQUENCE

};
class customAstar : public singletonBase<customAstar>
{
private:

	ASTAR_STATE m_AstarState;
	ASTAR_STATE m_PreAstarState;
	vector<tile*> openList;
	vector<tile*> closeList;
	/*vector<tile*> playerSearchOpenList;
	vector<tile*> playerSearchCloseList;*/
	stack<tile*> moveList;
	vector<tile*> enemySearchList;

	tile* startTile;
	tile* lastTile;
	unitSet* movingUnit;
	unitSet* AI_TargetUnit;
	
	int lastIndex;					//나중에 추가된 클로즈 리스트의 인덱스

	int CL_X;							//마지막으로 추가된 클로즈 리스트의 X
	int CL_Y;							//마지막으로 추가된 클로즈 리스트의 Y
	int CL_G;

	bool isReadyNextMove;
	bool isStartMoving;
	bool isAI_TargetFound;

	int startTime;
	int sequenceCount;				//전투 시퀀스 컨트롤 변수
	bool isAttackSequenceOne;
	bool isAttackSequenceTwo;
	bool isFinalAttackSequence;

public:
	customAstar();
	~customAstar();

	HRESULT init();
	void release();
	void update();

	void tileInitializing(tile* _searchingstarttile);	//배치 후 속성에 맞게 타일 초기화
	void addOpenList();			//오픈리스트 추가
	void calculateH();			//오픈 리스트 내 타일 H값계산
	void calculateF();			//오픈 리스트 내 타일 F값 계산
	void addCloseList();		//가장 작은 F선택후 클로즈 리스트에 추가
	void checkArrive();			//도착했는지 여부 체크
	void storeWay(tile* _tile);	//도착 후 시작지점과 도착지점을 잇는 경로 보여주자.
	void startMove();
	void finishMove();
	void cancelMove();
	void attackCommand();
	void attackSequence();

	void showAvailableTile_addOpenList();
	void showAvailableTile_addCloseList();
	void showAttackTile_addOpenList();
	bool showAttackTile_addCloseList();
	void AI_findTarget_addOpenList();
	bool AI_findTarget_addCloseList();

	void setStartTile(tile* _starttile) { startTile = _starttile; }
	void setLastTile(tile* _lasttile) { lastTile = _lasttile; }
	
	unitSet* getMovingUnit() { return movingUnit; }
	void setMovingUnit(unitSet* _movingunit) { movingUnit = _movingunit; }
	unitSet* getAI_TargetUnit() { return AI_TargetUnit; }
	void setAI_TargetUnit(unitSet* _ai_targetunit) { AI_TargetUnit = _ai_targetunit; }
	void setisAI_TargetFound(bool _isai_targetfound) { isAI_TargetFound = _isai_targetfound; }

	vector<tile*> getCloseList() { return closeList; }
	vector<tile*> getEnemySearchList() { return enemySearchList; }
	void setStartTime(int _starttime) { startTime = _starttime; }
	void setAstarState(ASTAR_STATE _astar_state) { m_AstarState = _astar_state; }

	void setIsAttackSequenceOne(bool _isattacksequenceone) { isAttackSequenceOne = _isattacksequenceone; }
	void setIsAttackSequenceTwo(bool _isattacksequencetwo) { isAttackSequenceTwo = _isattacksequencetwo; }
};

