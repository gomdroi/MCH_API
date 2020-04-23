#pragma once
#include "UI_Set.h"
class UI_turn :
	public UI_Set
{
private:
	image* playerTurnFront;
	image* enemyTurnFront;

	RECT rc_TurnFront;

	POINTFLOAT turnImagePos;

	bool isTurnChange;
	bool isTurnAnimation;
	int startTime;
	bool checkAniTime;

public:
	UI_turn();
	~UI_turn();

	virtual HRESULT init();
	virtual void update();
	virtual void animation();
	virtual void render();
	virtual void release();

	bool getIsTurnChange() { return isTurnChange; }
	void setIsTurnChange(bool _isturnchange) { isTurnChange = _isturnchange; }
	bool getIsTurnAnimation() { return isTurnAnimation; }

};

