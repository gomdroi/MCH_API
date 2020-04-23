#include "stdafx.h"
#include "itemManager.h"
#include "item.h"
#include "player.h"


HRESULT itemManager::init()
{

	//아이템 밀어넣고
	for (int i = 0; i < 13; i++)
	{
		item* venderItem = new item;
		venderItem->init();
		m_venderVector.push_back(venderItem);
	}
	//세팅.....
	m_venderVector[0]->setItem(ITEM_SWORD1);
	m_venderVector[1]->setItem(ITEM_SWORD2);
	m_venderVector[2]->setItem(ITEM_SWORD3);
	m_venderVector[3]->setItem(ITEM_SWORD4);
	m_venderVector[4]->setItem(ITEM_BOW1);
	m_venderVector[5]->setItem(ITEM_SHIELD1);
	m_venderVector[6]->setItem(ITEM_SHIELD2);
	m_venderVector[7]->setItem(ITEM_SHIELD3);
	m_venderVector[8]->setItem(ITEM_HPRING);
	m_venderVector[9]->setItem(ITEM_DUALRING1);
	m_venderVector[10]->setItem(ITEM_DUALRING2);
	m_venderVector[11]->setItem(ITEM_POTION);
	m_venderVector[12]->setItem(ITEM_CLASSBOOK);

	inVender = false;
	return S_OK;
}

void itemManager::release()
{
	vector<item*>::iterator iIter = m_itemVector.begin();
	for (iIter; iIter != m_itemVector.end();)
	{
		SAFE_DELETE((*iIter));
		iIter = m_itemVector.erase(iIter);
	}

	vector<item*>::iterator vIter = m_venderVector.begin();
	for (vIter; vIter != m_venderVector.end();)
	{
		SAFE_DELETE((*vIter));
		vIter = m_venderVector.erase(vIter);
	}
}

void itemManager::update()
{
	if (inVender)
	{
		for (int i = 0; i < m_venderVector.size(); i++)
		{
			m_venderVector[i]->setPosition(PointMake(20 + ((m_venderVector[i]->getImage()->getWidth() + 10) * (i % 7)), 568 + ((m_venderVector[i]->getImage()->getHeight() + 20) * (i / 7))));
		}
	}
	else
		for (int i = 0; i < m_venderVector.size(); i++)
		{
			m_venderVector[i]->setPosition(PointMake(10000,10000));
		}
	
		
	for (int i = 0; i < m_itemVector.size(); i++)
	{
		m_itemVector[i]->setPosition(PointMake(20 + ((m_itemVector[i]->getImage()->getWidth() + 10) * (i % 7)), 340 + ((m_itemVector[i]->getImage()->getHeight() + 20) * (i / 7))));
	}

	//예비용 업뎃
	for (int i = 0; i < m_venderVector.size(); i++)
	{
		m_venderVector[i]->update();
	}
	for (int i = 0; i < m_itemVector.size(); i++)
	{
		m_itemVector[i]->update();
	}
}

void itemManager::render()
{
	

	for (int i = 0; i < m_venderVector.size(); i++)
	{
		m_venderVector[i]->render();
	}
	//인벤토리 렌더링
	for (int i = 0; i < m_itemVector.size(); i++)
	{
		m_itemVector[i]->render();
	}
}

void itemManager::buyItem(ITEMTYPE _itemtype)
{
		item* invenItem = new item;
		invenItem->init();
		invenItem->setItem(_itemtype);
		m_itemVector.push_back(invenItem);
}

itemManager::itemManager()
{
}


itemManager::~itemManager()
{
}
