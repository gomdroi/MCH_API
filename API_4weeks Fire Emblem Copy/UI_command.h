#pragma once
#include "UI_Set.h"
class unitSet;
class itemSet;
class UI_command :
	public UI_Set
{
private:
	COMMAND_STATE commandState;
	//이동종료시 커맨드창
	RECT rc_moveConfirmFront;
	image* moveConfirmFront;

	commandMenu commandBar[5];
	int moveConfirmFrontNum;

	bool isAtkPossible;
	bool isTradePossible;
	bool isOnVender;

	////////////////////
	// 공격아이템 선택 //
	///////////////////
	RECT rc_atkItemSelectFront;
	image* atkItemSelectFront;
	
	RECT rc_itemName[5];

	RECT rc_atkStatusFront;
	image* atkStatusFront;

	RECT rc_atkStatusStr;
	char afterAtkStatusStr[256];
	RECT rc_critStatusStr;
	char afterCritStatusStr[256];
	RECT rc_hitStatusStr;
	char afterHitStatusStr[256];
	RECT rc_avoidStatusStr;
	char afterAvoidStatusStr[256];

	commandMenu itemBar[5];

	int atkItemSelectFrontNum;
public:
	UI_command();
	~UI_command();

	virtual HRESULT init();
	virtual void update();
	virtual void animation();
	virtual void render();
	virtual void release();

	virtual void adjustCommandWindow();
	virtual void arrangementCommand();

	int getMoveConfirmFrontNum() { return moveConfirmFrontNum; }
	void setMoveConfirmFrontNum(int _moveconfirmnum) { moveConfirmFrontNum = _moveconfirmnum; }
	commandMenu* getCommandBar() { return commandBar; }
	commandMenu* getItemBar() { return itemBar; }

	bool getIsAtkPossible() { return isAtkPossible; }
	void setIsAtkPossible(bool _isatkpossible) { isAtkPossible = _isatkpossible; }
	bool getIsTradePossible() { return isTradePossible; }
	void setIsTradePossible(bool _istradepossible) { isTradePossible = _istradepossible; }

	COMMAND_STATE getCommandState() { return commandState; }
	void setCommandState(COMMAND_STATE _commandstate) { commandState = _commandstate; }

};

