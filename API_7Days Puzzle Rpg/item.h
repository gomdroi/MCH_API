#pragma once
#include "objects.h"
typedef enum ITEMTYPE
{
	ITEM_NONE,
	ITEM_SWORD1,
	ITEM_SWORD2,
	ITEM_SWORD3,
	ITEM_SWORD4,
	ITEM_BOW1,
	ITEM_SHIELD1,
	ITEM_SHIELD2,
	ITEM_SHIELD3,
	ITEM_HPRING,
	ITEM_DUALRING1,
	ITEM_DUALRING2,
	ITEM_KEY,
	ITEM_POTION,
	ITEM_CLASSBOOK
};
class item : public objects
{
private:
	char itemName[128];

	float m_itemHp;
	float m_itemAtk;
	float m_itemDef;
	float m_itemPrice;
	float m_itemPower;

	char itemExplain[256];
	char itemHp[128];
	char itemAtk[128];
	char itemDef[128];
	char itemPrice[128];

	/*POINT* m_stickedTilePosition;*/
	int m_itemNumber;
	ITEMTYPE itemType;

	bool isPurchased;
	bool isEquiped;

public:

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void animation();
	virtual void setItem(ITEMTYPE _itemtype);

	int getItemNumber() { return m_itemNumber; }
	void setItemNumber(int _itemnumber) { m_itemNumber = _itemnumber; }
	bool getIsPurchased() { return isPurchased; }
	void setIsPurchased(bool _isPurchased) { isPurchased = _isPurchased; }
	bool getIsEquiped() { return isEquiped; }
	void setIsEquiped(bool _isequiped) { isEquiped = _isequiped; }

	float getItemHp() { return m_itemHp; }
	void setItemHp(float _itemhp) { m_itemHp = _itemhp; }
	float getItemAtk() { return m_itemAtk; }
	void setItemAtk(float _itematk) { m_itemAtk = _itematk; }
	float getItemDef() { return m_itemDef; }
	void setItemDef(float _itemdef) { m_itemDef = _itemdef; }
	float getItemPrice() { return m_itemPrice; }
	void setItemPrice(float _itemprice) { m_itemPrice = _itemprice; }
	float getItemPower() { return m_itemPower; }
	void setItemPower(float _itempower) { m_itemPower = _itempower; }
	ITEMTYPE getItemType() { return itemType; }

	item();
	~item();
};

