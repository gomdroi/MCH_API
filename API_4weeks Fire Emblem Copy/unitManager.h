#pragma once
#include "singletonBase.h"
class unitSet;
class tile;
enum STAGE_NUMBER;
class unitManager :
	public singletonBase<unitManager>
{
private:
	vector<unitSet*> allyUnitVector;
	vector<unitSet*> enemyUnitVector;
	vector<unitSet*> zOrderVector;

	

public:
	unitManager();
	~unitManager();



	virtual HRESULT init();
	virtual void init(STAGE_NUMBER _stagenumber);
	virtual void update();
	virtual void animation();
	virtual void render();
	virtual void release();
	virtual void deadCheck();
	virtual void endGameCheck();
	//virtual void deadCheckAllyUnit();
	//virtual void deadCheckEnemyUnit();

	vector<unitSet*> getAllyUnitVector() { return allyUnitVector; }
	vector<unitSet*> getEnemyUnitVector() { return enemyUnitVector; }
};

