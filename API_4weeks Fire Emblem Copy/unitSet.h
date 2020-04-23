#pragma once
#include "objects.h"

class itemSet;
class tile;
class customAstar;
class unitSet :
	public objects
{
protected:

	UNITTYPE m_unitType;
	UNITDEF m_unitDef;
	UNIT_DIRECTION m_direction;
	UNIT_DIRECTION moveDirection;
	image* smallMugShot;

	vector<itemSet*> inven;
	itemSet* equiped;

	tile* occupiedTile;
	
	unitSet* enemyTargetUnit;
	tile* enemyTargetTile;

	float unitAngle;

	char unitNameStr[256];
	int maxHp;
	int curHp;
	int strength;
	int skill;
	int speed;
	int luck;
	int defense;
	int resistance;
	int move;


	int atk;
	int hit;
	int crit;
	int def;
	int avoid;
	int rng;
	int enemyAttackRange;

	bool isTargeted;
	bool isSelected;
	bool isTurnEnd;
	bool isFlying;
	bool isMove;
	bool isAttack;
	bool isDead;
	bool isPickTarget;

	//AI
	bool isNPC;
	bool isTakingTurn;
	bool isTargetFound;
	bool isSearchTarget;
	bool commanceAttack;

	int m_unitFrameIndex;
	int m_unitFrameCount;

public:
	unitSet();
	~unitSet();

	static bool cmp(const unitSet* a, const unitSet* b)
	{
		return a->m_position.y < b->m_position.y;
	}

	virtual HRESULT init();
	virtual void update();
	virtual void animation();
	virtual void heading();
	virtual void render();
	virtual void release();
	virtual void setUnit(UNITTYPE _unitType);

	void statusCalc();
	void unitMove();
	void equipItem();
	void unitAI();
	void pickTarget();
	void changeEquipItem();


	tile* getOccupiedTile() { return occupiedTile; }
	void setOccupiedTile(tile* _occupiedtile) { occupiedTile = _occupiedtile; }
	UNITTYPE getUnitType() { return m_unitType; }
	UNITDEF getUnitDef() { return m_unitDef; }
	void setDirection(UNIT_DIRECTION _unitdirection) { m_direction = _unitdirection; }
	itemSet* getEquiped() { return equiped; }
	void setEquiped(itemSet* _equiped) { equiped = _equiped; }
	char* getUnitNameStr() { return unitNameStr; }
	vector<itemSet*> getInven() { return inven; }
	image* getSmallMugShot() { return smallMugShot; }
	

	int getMaxHp() { return maxHp; }
	void setMaxHp(int _maxhp) { maxHp = _maxhp; }
	int getCurHp() { return curHp; }
	void setCurHp(int _curhp) { curHp = _curhp; }
	int getMove() { return move; }
	int getDefense() { return defense; }
	int getResistance() { return resistance; }

	int getStrength() { return strength; }
	int getSkill() { return skill; }
	int getLuck() { return luck; }

	int getAtk() { return atk; }
	int getHit() { return hit; }
	int getCrit() { return crit; }
	int getDef() { return def; }
	int getAvoid() { return avoid; }
	int getRng() { return rng; }
	int getEnemyAttackRange() { return enemyAttackRange; }

	bool getIsTurnEnd() { return isTurnEnd; }
	bool getIsFlying() { return isFlying; }
	void setIsTurnEnd(bool _isturnend) { isTurnEnd = _isturnend; }
	void setIsTargeted(bool _istargeted) { isTargeted = _istargeted; }
	void setIsSelected(bool _isselected) { isSelected = _isselected; }
	
	bool getIsDead() { return isDead; }
	void setIsDead(bool _isdead) { isDead = _isdead; }
	bool getIsPickTarget() { return isPickTarget; }
	void setIsPickTarget(bool _ispicktarget) { isPickTarget = _ispicktarget; }

	void setUnitAngle(float _angle) { unitAngle = _angle; }
	void setIsMove(bool _ismove) { isMove = _ismove; }
	void setIsAttack(bool _isattack) { isAttack = _isattack; }
	bool getIsNPC() { return isNPC; }
	void setIsNPC(bool _isnpc) { isNPC = _isnpc; }
	bool getIsTakingTurn() { return isTakingTurn; }
	void setIsTakingTurn(bool _istakingturn) { isTakingTurn = _istakingturn; }
	bool getIsTargetFound() { return isTargetFound; }
	void setIsTargetFound(bool _istargetfound) { isTargetFound = _istargetfound; }
	bool getIsSearchTarget() { return isSearchTarget; }
	void setIsSearchTarget(bool _issearchtarget) { isSearchTarget = _issearchtarget; }
	bool getCommanceAttack() { return commanceAttack; }
	void setCommanceAttack(bool _iscommanceattack) { commanceAttack = _iscommanceattack; }


};

