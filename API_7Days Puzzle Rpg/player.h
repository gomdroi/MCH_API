#pragma once
#include "objects.h"

enum PLAYER_DIRECTION
{
	PLAYER_IDLE,
	PLAYER_UP,
	PLAYER_DOWN,
	PLAYER_LEFT,
	PLAYER_RIGHT
};
class tile;
class monster;
class item;
class player : public objects
{
private:
	float m_playerMaxHp;
	float m_playerCurHp;
	float m_playerAtk;
	float m_playerDef;
	float m_playerSpeed;
	float m_playerExp;
	float m_playerGold;
	int m_playerLevel;
	float m_playerExpMax;
	bool isBattle;

	//변신
	bool isClassChange1;
	bool isClassChange2;

	int m_poisonClawCount;
	int m_defShrineCount;
	bool isDefShrineActive;
	POINT m_startPosition;

	int m_playerIndex;
	int m_playerCount;
	PLAYER_DIRECTION m_direction;
	float destAngle;
	float distance;
	bool isMoving;
	float moveLength;

	bool isWin;

	//시간 관련
	float startTime;
	float curTime;

	//상태표시
	char playerHp[128];
	char playerAtk[128];
	char playerDef[128];
	char playerExpGold[128];
	char playerLevel[128];
public:
	
	int stageCount;
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void animation();
	virtual void move();	

	

	float getMaxHp() { return m_playerMaxHp; }
	void setMaxHp(float _maxhp) { m_playerMaxHp = _maxhp; }
	float getCurHp() { return m_playerCurHp; }
	void setCurHp(float _curhp) { m_playerCurHp = _curhp; }
	float getAtk() { return m_playerAtk; }
	void setAtk(float _atk) { m_playerAtk = _atk; }
	float getDef() { return m_playerDef; }
	void setDef(float _def) { m_playerDef = _def; }
	float getExp() { return m_playerExp; }
	void setExp(float _exp) { m_playerExp = _exp; }
	float getGold() { return m_playerGold; }
	void setGold(float _gold) { m_playerGold = _gold; }
	int getLevel() { return m_playerLevel; }
	void setLevel(int _level) { m_playerLevel = _level; }

	bool getIsWin() { return isWin; }

	//목적지 설정
	void setDestination(int _tilemapindex);
	float getDistanceBetween(int _tilemapindex);
	
	void battle();
	void eventIncounter();
	void dolevelUp();
	void buying();
	player();
	~player();
};

