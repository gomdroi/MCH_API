#pragma once
#include "singletonBase.h"
#define INVEN_MAX 16
#define MAP_MAX 15
class tile;
class monsters;
class events;
class tileManager : public singletonBase<tileManager>
{
private:
	vector<tile*> m_tileInvenVector;
	/*vector<tile*>::iterator m_tileInvenIter;*/
	vector<tile*> m_tileMapVector;
	vector<tile*>::iterator m_tileMapIter;
	tile* m_selectedTile;

	POINT *m_mousePositionPointer;
	//Ÿ�� �巡�׿�
	POINT mousePositionSave;
	bool isTouch;
	bool isOccupied;
	monsters** m_monsterVector;

public:

	virtual HRESULT init();
	virtual HRESULT stageInit(int _stage);

	void stageRelease();
	virtual void release();
	virtual void update();
	virtual void render();



	//�巡�� ����
	void dragTile();
	//Ÿ�� �κ� ����
	void reArrangeInvenTile();


	vector<tile*> getTileInvenVector() { return m_tileInvenVector; }
	vector<tile*> getTileMapVector() { return m_tileMapVector; }
	/*vector<tile*>::iterator getTileInvenIter() {return m_tileInvenIter; }*/
	
	tileManager();
	~tileManager();

	
};

