#include "stdafx.h"
#include "itemManager.h"
#include "itemSet.h"

itemManager::itemManager()
{
}


itemManager::~itemManager()
{
}

HRESULT itemManager::init()
{
	for (int i = 0; i < 12; i++)
	{
		itemSet* shopItem = new itemSet;
		shopItem->init();
		shopVector.push_back(shopItem);
	}
	shopVector[0]->setWeaponItem(IRON_SWORD);
	shopVector[1]->setWeaponItem(STEEL_SWORD);
	shopVector[2]->setWeaponItem(IRON_AXE);
	shopVector[3]->setWeaponItem(STEEL_AXE);
	shopVector[4]->setWeaponItem(IRON_SPEAR);
	shopVector[5]->setWeaponItem(STEEL_SPEAR);
	shopVector[6]->setWeaponItem(IRON_BOW);
	shopVector[7]->setWeaponItem(STEEL_BOW);
	shopVector[8]->setConsumableItem(VULNERARY);
	shopVector[9]->setWeaponItem(THROWING_AXE);
	shopVector[10]->setWeaponItem(JAVELIN_SPEAR);
	shopVector[11]->setWeaponItem(SACRED_SWORD);
	

	return S_OK;
}

void itemManager::update()
{
}

void itemManager::animation()
{
}

void itemManager::render()
{
}

void itemManager::release()
{
	vector<itemSet*>::iterator iIter = shopVector.begin();

	for (iIter; iIter != shopVector.end();)
	{
		SAFE_DELETE((*iIter));
		iIter = shopVector.erase(iIter);
	}
}
