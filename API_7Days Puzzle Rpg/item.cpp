#include "stdafx.h"
#include "item.h"


HRESULT item::init()
{
	IMAGEMANAGER->addImage("팔림", "images/Sold.bmp", 32 , 32 , true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("체력포션", "images/Hp_Potion", 32, 32, true, RGB(255, 0, 255));
	m_position = { 0,0 };
	m_collider = RectMakeCenter(m_position.x, m_position.y, 32, 32 );
	sprintf_s(itemName, "똘박이"); 

	m_itemAtk = 0;
	m_itemDef = 0;
	m_itemPrice = 0;
	m_itemPower = 0;

	m_itemNumber = 0;
	itemType = ITEM_NONE;

	bool isPurchased = false;
	bool isEquiped = false;

	m_itemNumber = 0;
	return S_OK;
}

void item::release()
{
}

void item::update()
{
	

}

void item::render()
{

	m_collider = RectMakeCenter(m_position.x, m_position.y, 32, 32);
	m_image->render(getMemDC(), m_collider.left, m_collider.top);
	if (isPurchased == true)
	{
		m_image = IMAGEMANAGER->findImage("팔림");
	}
	if (PtInRect(&m_collider, m_ptMouse) && isPurchased == false)
	{
		RECT rc = RectMake(20, 220, 260, 100);
		m_image->render(getMemDC(), 20, 165);
		
	/*	HFONT myFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "Fonts/Demonized.ttf");*/
		HFONT myFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "휴먼매직체");
		HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
		SetBkMode(getMemDC(), TRANSPARENT);

		sprintf_s(itemHp, "H  P + %0.f", m_itemHp);
		TextOut(getMemDC(), 70, 160, itemHp, strlen(itemHp));
		sprintf_s(itemAtk, "ATK + %0.f", m_itemAtk);
		TextOut(getMemDC(), 70, 180, itemAtk, strlen(itemAtk));
		sprintf_s(itemDef, "DEF + %0.f", m_itemDef);
		TextOut(getMemDC(), 70, 200, itemDef, strlen(itemDef));
		/*	sprintf_s(monsterExpGold, "EXP : %0.f        GOLD : %0.f", m_monsterExp, m_monsterGold);
			TextOut(getMemDC(), 50, 100, monsterExpGold, strlen(monsterExpGold));*/
			/*wsprintf(cMonsterName, "LEVEL : %d", m_player->getLevel());*/
		TextOut(getMemDC(), 70, 140, itemName, strlen(itemName));
		sprintf_s(itemPrice, "PRICE : %0.f", m_itemPrice);
		TextOut(getMemDC(), 200, 200, itemPrice, strlen(itemPrice));
		DrawText(getMemDC(), itemExplain, -1, &rc, DT_WORDBREAK);
		/*TextOut(getMemDC(), 20, 220, eventExplain, strlen(eventExplain));*/

		SelectObject(getMemDC(), oldFont);
		DeleteObject(myFont);
	}
}

void item::animation()
{
}

void item::setItem(ITEMTYPE _itemtype)
{
	itemType = _itemtype;

	switch (itemType)
	{
	case ITEM_NONE:
		break;
	case ITEM_SWORD1:
		m_image = new image;
		m_image->init("images/Sword1.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "철검");
		sprintf_s(itemExplain, "단단함");

		m_itemHp = 0;
		m_itemAtk = 1;
		m_itemDef = 0;
		m_itemPrice = 10;
		m_itemPower = 0;

		break;
	case ITEM_SWORD2:
		m_image = new image;
		m_image->init("images/Sword2.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "좋은검");
		sprintf_s(itemExplain, "단단함x2");

		m_itemHp = 0;
		m_itemAtk = 3;
		m_itemDef = 0;
		m_itemPrice = 30;
		m_itemPower = 0;

		break;
	case ITEM_SWORD3:
		m_image = new image;
		m_image->init("images/Sword3.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "개좋은검");
		sprintf_s(itemExplain, "단단함x4");

		m_itemHp = 0;
		m_itemAtk = 5;
		m_itemDef = 0;
		m_itemPrice = 50;
		m_itemPower = 0;

		break;
	case ITEM_SWORD4:
		m_image = new image;
		m_image->init("images/Sword4.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "드래곤 슬레이어");
		sprintf_s(itemExplain, "나~ 괜찮은걸까~ 지금 이대로~ 어른이 되버린 다음에는 점점 더 다른 사람들과 달라 지겠지, 용살검");

		m_itemHp = 100;
		m_itemAtk = 10;
		m_itemDef = 1;
		m_itemPrice = 500;
		m_itemPower = 0;

		break;
	case ITEM_BOW1:
		m_image = new image;
		m_image->init("images/GodBow.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "신살의 활");
		sprintf_s(itemExplain, "크킄...난 신을 쏠거라능..");

		m_itemHp = 200;
		m_itemAtk = 20;
		m_itemDef = 10;
		m_itemPrice = 1000;
		m_itemPower = 0;

		break;
	case ITEM_SHIELD1:
		m_image = new image;
		m_image->init("images/Shield1.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "철방패");
		sprintf_s(itemExplain, "좋음");

		m_itemHp = 5;
		m_itemAtk = 0;
		m_itemDef = 1;
		m_itemPrice = 10;
		m_itemPower = 0;

		break;
	case ITEM_SHIELD2:
		m_image = new image;
		m_image->init("images/Shield2.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "음방패");
		sprintf_s(itemExplain, "좋음x2");

		m_itemHp = 5;
		m_itemAtk = 0;
		m_itemDef = 2;
		m_itemPrice = 30;
		m_itemPower = 0;

		break;
	case ITEM_SHIELD3:
		m_image = new image;
		m_image->init("images/Shield3.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "각방패");
		sprintf_s(itemExplain, "좋음x4");

		m_itemHp = 10;
		m_itemAtk = 0;
		m_itemDef = 3;
		m_itemPrice = 70;
		m_itemPower = 0;

		break;
	case ITEM_HPRING:
		m_image = new image;
		m_image->init("images/Hp_Ring.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "루비 반지");
		sprintf_s(itemExplain, "체력짱짱맨, 체력도 조금 채워드림");

		m_itemHp = 15;
		m_itemAtk = 0;
		m_itemDef = 0;
		m_itemPrice = 20;
		m_itemPower = 15;

		break;
	case ITEM_DUALRING1:
		m_image = new image;
		m_image->init("images/DualRing1.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "듀얼링");
		sprintf_s(itemExplain, "왼손 전용, 오른손에 끼지마시오.");

		m_itemHp = 0;
		m_itemAtk = 1;
		m_itemDef = 1;
		m_itemPrice = 300;
		m_itemPower = 0;
		break;
	case ITEM_DUALRING2:
		m_image = new image;
		m_image->init("images/DualRing2.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "듀얼링");
		sprintf_s(itemExplain, "오른손 전용, 왼손에 끼지마시오.");

		m_itemHp = 0;
		m_itemAtk = 1;
		m_itemDef = 1;
		m_itemPrice = 300;
		m_itemPower = 0;
		break;
	case ITEM_KEY:

		break;
	case ITEM_POTION:
		m_image = new image;
		m_image->init("images/Hp_Potion.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "물약");
		sprintf_s(itemExplain, "체력을 30 회복한다. 스테이지를 넘어가면 다시 구입 가능");

		m_itemHp = 0;
		m_itemAtk = 0;
		m_itemDef = 0;
		m_itemPrice = 10;
		m_itemPower = 30;
		break;
	case ITEM_CLASSBOOK:
		m_image = new image;
		m_image->init("images/Change2.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(itemName, "힘의 수정");
		sprintf_s(itemExplain, "가지고 있으면 좋은 일이 생길지도 모른다.");

		m_itemHp = 1;
		m_itemAtk = 1;
		m_itemDef = 1;
		m_itemPrice = 100;
		m_itemPower = 30;
		break;
	default:
		break;
	}

}

item::item()
{
}


item::~item()
{
}
