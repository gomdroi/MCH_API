#pragma once
#include "objects.h"
enum MONSTERTYPE
{
	TYPE_NONE,
	TYPE_ENTOMBED,
	TYPE_MOGALL,
	TYPE_DEIMOS,
	TYPE_REVENANT,
	TYPE_GUARDIAN,
	TYPE_BARON,
	TYPE_BERKUT,
	TYPE_TITAN,
	TYPE_VESTAL,
	TYPE_DRAGON,
	TYPE_JEDAH,
	TYPE_DEATHGOYLE,
	TYPE_NUIBABA,
	TYPE_TOMBLORD,
	TYPE_FELLGOD
};
enum MONSTERLEVEL
{
	LEVEL_0,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	LEVEL_4,
	LEVEL_5,
	LEVEL_6,
	LEVEL_7
};
enum MONSTER_DIRECTION
{
	MONSTER_IDLE,
	MONSTER_UP,
	MONSTER_DOWN,
	MONSTER_LEFT,
	MONSTER_RIGHT
};
class monsters : public objects
{
private:
	char monsterName[128];

	float m_monsterMaxHp;
	float m_monsterCurHp;
	float m_monsterAtk;
	float m_monsterDef;
	float m_monsterExp;
	float m_monsterGold;

	char monsterHp[128];
	char monsterAtk[128];
	char monsterDef[128];
	char monsterExpGold[128];

	/*POINT* m_stickedTilePosition;*/
	int m_monsterNumber;
	MONSTERTYPE monsterType;
	MONSTERLEVEL monsterLevel;
	MONSTER_DIRECTION m_direction;
	int m_monsterIndex;
	int m_monsterCount;

	bool isInvenPlaced;
	bool isMapPlaced;
	
public:

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void animation();
	virtual void setMonster(MONSTERTYPE _monsterType);

	/*POINT* getStickedTilePosition() { return m_stickedTilePosition; }
	void setStickedTilePosition(POINT _stp) { m_stickedTilePosition = &_stp; }*/
	bool getIsInvenPlaced() { return isInvenPlaced; }
	void setIsInvenPlaced(bool _isplaced) { isInvenPlaced = _isplaced; }
	bool getIsMapPlaced() { return isMapPlaced; }
	void setIsMapPlaced(bool _isplaced) { isMapPlaced = _isplaced; }
	void setDirection(MONSTER_DIRECTION _direction) { m_direction = _direction; }
	int getMonsterNumber() { return m_monsterNumber; }
	void setMonsterNumber(int _monsternumber) { m_monsterNumber = _monsternumber; }

	float getMonsterMaxHp() { return m_monsterMaxHp; }
	void setMonsterMaxHp(float _monstermaxhp) { m_monsterMaxHp = _monstermaxhp; }
	float getMonsterCurHp() { return m_monsterCurHp; }
	void setMonsterCurHp(float _monstercurhp) { m_monsterCurHp = _monstercurhp; }
	float getMonsterAtk() { return m_monsterAtk; }
	void setMonsterAtk(float _monsteratk) { m_monsterAtk = _monsteratk; }
	float getMonsterDef() { return m_monsterDef; }
	void setMonsterDef(float _monsterdef) { m_monsterDef = _monsterdef; }
	float getMonsterExp() { return m_monsterExp; }
	void setMonsterExp(float _monsterexp) { m_monsterExp = _monsterexp; }
	float getMonsterGold() { return m_monsterGold; }
	void setMonsterGold(float _monstergold) { m_monsterGold = _monstergold; }

	monsters();
	~monsters();
};

