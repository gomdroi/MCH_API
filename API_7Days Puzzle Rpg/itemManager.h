#pragma once
#include "singletonBase.h"

enum ITEMTYPE;
class item;
class player;
class itemManager : public singletonBase<itemManager>
{
private:
	vector<item*> m_itemVector;
	vector<item*> m_venderVector;
	bool inVender;
public:

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void buyItem(ITEMTYPE _itemtype);

	vector<item*> getItemVector() { return m_itemVector; }
	vector<item*> getVenderVector() { return m_venderVector; }

	bool getInVender() { return inVender; }
	void setInVender(bool _invender) { inVender = _invender; }

	itemManager();
	~itemManager();
};

