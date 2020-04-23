#pragma once
#include "singletonBase.h"
enum STAGE
{
	STAGE_NONE,
	STAGE_ONE,
	STAGE_TWO,
	STAGE_THREE,
	STAGE_FOUR,
	STAGE_FIVE,
	STAGE_SIX,
	STAGE_SEVEN,
	STAGE_EIGHT,
	STAGE_NINE,
	STAGE_TEN
};
class monsters;
class monsterManager : public singletonBase<monsterManager>
{
private:
	vector<monsters*> m_MonsterVector;
	STAGE m_stage;
	int m_totalMonster;
public:

	virtual HRESULT init();
	virtual HRESULT init(STAGE _stage,int _monsterplus);
	virtual void release();
	virtual void update();
	virtual void render();

	vector<monsters*> getMonsterVector() { return m_MonsterVector; }

	STAGE getStage() { return m_stage; }
	monsterManager();
	~monsterManager();
};

