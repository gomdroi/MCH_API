#pragma once
#include "objects.h"

class itemSet :
	public objects
{
protected:
	ITEM_TYPE itemType;
	char itemNameStr[256];
	CONSUMABLE_KIND consumableKind;

	int itemPower;

	WEAPON_KIND weaponKind;
	WEAPON_TYPE weaponType;
	WEAPON_GRADE weaponGrade;
		
	int weaponDmg;
	int weaponWeight;
	int weaponRng;
	int weaponHit;
	int weaponCrit;
	int weaponPrice;

public:
	itemSet();
	~itemSet();

	virtual HRESULT init();
	virtual void update();
	virtual void animation();
	virtual void render();
	virtual void release();
	virtual void setWeaponItem(WEAPON_KIND _weaponkind);
	virtual void setConsumableItem(CONSUMABLE_KIND _consumablekind);

	ITEM_TYPE getItemType() { return itemType; }
	char* getItemName() { return itemNameStr; }
	WEAPON_GRADE getWeaponGrade() { return weaponGrade; }
	int getWeaponDmg() { return weaponDmg; }
	int getWeaponWeight() { return weaponWeight; }
	int getWeaponRng() { return weaponRng; }
	int getWeaponHit() { return weaponHit; }
	int getWeaponCrit() { return weaponCrit; }
	int getWeaponPrice() { return weaponPrice; }

	int getItemPower() { return itemPower; }
};

