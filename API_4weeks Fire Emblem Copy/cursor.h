#pragma once
#include "objects.h"

class unitSet;
class itemSet;
class tile;
class UI_command;
class UI_turn;
struct commandMenu;
enum CURSOR_STATE
{
	CURSOR_IDLE,
	CURSOR_ATTACKTILESELECT,
	CURSOR_MOVETILESELECT,
	CURSOR_ITEMSELECT,
	CURSOR_COMMANDSELECT
};
class cursor : public objects
{
private:
	CURSOR_STATE cursorState;

	tile* curTile;	//커서가 있는 현재 타일
	tile* moveStartTile;	//캐릭의 이동이 시작된 타일
	unitSet* selectedUnit;	//선택한 캐릭
	unitSet* receiveUnit;	//타겟으로 선택한 캐릭

	image* commandCursor;	//명령메뉴 커서 아이콘
	commandMenu* curCommand;	//커서가 가리키는 현재 명령
	bool commandCursorIsForward;	//커서의 애니메이션 컨트롤

	int menuNum;
	
	int m_cursorIndex;
	int m_cursorCount;
	int m_commandCursorIndex;
	int m_commandCursorCount;

	int tileMapSizeX;	//현재 맵의 최대 사이즈 X
	int tileMapSizeY;	//현재 맵의 최대 사이즈 Y

	bool isOnUnit;
	bool selectMoveTile;
	bool selectAttackTile;
	bool isStopControl;
	bool selectItem;

	int stageNum;


public:
	cursor();
	~cursor();

	virtual HRESULT init();
	virtual void update();
	virtual void animation();
	virtual void render();
	virtual void release();

	virtual void control();
	void unitControl();
	void unitAtkControl();

	virtual void turnOverCheck();

	void setCursorState(CURSOR_STATE _cursorstate) { cursorState = _cursorstate; }

	void setTileMapSizeX(int _mapsizex) { tileMapSizeX = _mapsizex; }
	void setTileMapSizeY(int _mapsizey) { tileMapSizeY = _mapsizey; }
	tile* getCurTile() { return curTile; }
	void setCurTile(tile* _curtile) { curTile = _curtile; }
	unitSet* getSelectedUnit() { return selectedUnit; }
	void setSelectedUnit(unitSet* _selectedunit) { selectedUnit = _selectedunit; }
	unitSet* getReceiveUnit() { return receiveUnit; }
	void setReceiveUnit(unitSet* _receiveunit) { receiveUnit = _receiveunit; }

	int getMenuNum() { return menuNum; }
	void setMenuNum(int _menunum) { menuNum = _menunum; }

	bool getSelectAttackTile() { return selectAttackTile; }
	void setSelectAttackTile(bool _selectattacktile) { selectAttackTile = _selectattacktile; }
	void setSelectMoveTile(bool _selectmovetile) { selectMoveTile = _selectmovetile; }
	void setIsStopControl(bool _isstopcontrol) { isStopControl = _isstopcontrol; }
	void setSelectItem(bool _selectitem) { selectItem = _selectitem; }

	int getTileMapSizeX() { return tileMapSizeX; }
	int getTileMapSizeY() { return tileMapSizeY; }

	void setCurCommand(commandMenu* _curcommand) { curCommand = _curcommand; }

	int getStageNum() { return stageNum; }
	void setStageNum(int _stagenum) { stageNum = _stagenum; }
};

