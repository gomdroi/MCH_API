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
	
	int lastIndex;					//���߿� �߰��� Ŭ���� ����Ʈ�� �ε���

	int CL_X;							//���������� �߰��� Ŭ���� ����Ʈ�� X
	int CL_Y;							//���������� �߰��� Ŭ���� ����Ʈ�� Y
	int CL_G;

	bool isReadyNextMove;
	bool isStartMoving;
	bool isAI_TargetFound;

	int startTime;
	int sequenceCount;				//���� ������ ��Ʈ�� ����
	bool isAttackSequenceOne;
	bool isAttackSequenceTwo;
	bool isFinalAttackSequence;

public:
	customAstar();
	~customAstar();

	HRESULT init();
	void release();
	void update();

	void tileInitializing(tile* _searchingstarttile);	//��ġ �� �Ӽ��� �°� Ÿ�� �ʱ�ȭ
	void addOpenList();			//���¸���Ʈ �߰�
	void calculateH();			//���� ����Ʈ �� Ÿ�� H�����
	void calculateF();			//���� ����Ʈ �� Ÿ�� F�� ���
	void addCloseList();		//���� ���� F������ Ŭ���� ����Ʈ�� �߰�
	void checkArrive();			//�����ߴ��� ���� üũ
	void storeWay(tile* _tile);	//���� �� ���������� ���������� �մ� ��� ��������.
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

