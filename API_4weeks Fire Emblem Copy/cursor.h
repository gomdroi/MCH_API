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

	tile* curTile;	//Ŀ���� �ִ� ���� Ÿ��
	tile* moveStartTile;	//ĳ���� �̵��� ���۵� Ÿ��
	unitSet* selectedUnit;	//������ ĳ��
	unitSet* receiveUnit;	//Ÿ������ ������ ĳ��

	image* commandCursor;	//��ɸ޴� Ŀ�� ������
	commandMenu* curCommand;	//Ŀ���� ����Ű�� ���� ���
	bool commandCursorIsForward;	//Ŀ���� �ִϸ��̼� ��Ʈ��

	int menuNum;
	
	int m_cursorIndex;
	int m_cursorCount;
	int m_commandCursorIndex;
	int m_commandCursorCount;

	int tileMapSizeX;	//���� ���� �ִ� ������ X
	int tileMapSizeY;	//���� ���� �ִ� ������ Y

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

