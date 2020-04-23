#pragma once
#include "singletonBase.h"

enum SCENETYPE
{

};

class enemySet;
class soldier;
class malmijal;
class eyeman;
class snake;
class bat;
class worm_boss;
class agahnim_boss;
class enemyManager : public singletonBase<enemyManager>
{
private:
	vector<enemySet*> _enemyVector;
	SCENETYPE _sceneType;

public:
	enemyManager();
	~enemyManager();

	virtual HRESULT init();
	virtual HRESULT init(int sceneNum);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);

	vector<enemySet*> getEnemyList() { return _enemyVector; }
};

