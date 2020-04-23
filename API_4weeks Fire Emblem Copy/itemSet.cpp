#include "stdafx.h"
#include "itemSet.h"


itemSet::itemSet()
{
}


itemSet::~itemSet()
{
}

HRESULT itemSet::init()
{
	IMAGEMANAGER->addImage("√∂∞À", "images/Item/iron_sword.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("∞≠√∂∞À", "images/Item/steel_sword.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Ω≈º∫∞À", "images/Item/sacred_sword.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("√∂√¢", "images/Item/iron_spear.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("∞≠√∂√¢", "images/Item/steel_spear.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("¿⁄∫ß∏∞", "images/Item/javelin_spear.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("√∂µµ≥¢", "images/Item/iron_axe.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("∞≠√∂µµ≥¢", "images/Item/steel_axe.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("¥¯¡ˆ¥¬µµ≥¢", "images/Item/throwing_axe.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("√∂»∞", "images/Item/iron_bow.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("∞≠√∂»∞", "images/Item/steel_bow.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("ªÛ√≥æ‡", "images/Item/vulnerary.bmp", 48, 48, true, RGB(255, 0, 255));

	itemType = ITEMTYPE_NONE;
	consumableKind = CONSUMABLE_NONE;

	itemPower = 0;

	weaponGrade = WEAPON_GRADE_NONE;
	weaponKind = WEAPONKIND_NONE;
	weaponType = WEAPONTYPE_NONE;

	weaponDmg = 0;
	weaponWeight = 0;
	weaponRng = 0;
	weaponHit = 0;
	weaponCrit = 0;
	weaponPrice = 0;
	return S_OK;
}

void itemSet::update()
{
}

void itemSet::animation()
{
}

void itemSet::render()
{
}

void itemSet::release()
{
}

void itemSet::setWeaponItem(WEAPON_KIND _weaponkind)
{
	itemType = ITEM_WEAPON;

	switch (_weaponkind)
	{
	case WEAPONKIND_NONE:
		break;
	case IRON_SWORD:
		m_image = IMAGEMANAGER->findImage("√∂∞À");
		sprintf_s(itemNameStr, "Iron sword");
		weaponGrade = WEAPON_GRADE_E;
		weaponKind = IRON_SWORD;
		weaponType = SWORD;

		weaponDmg = 3;
		weaponWeight = 2;
		weaponRng = 1;
		weaponHit = 85;
		weaponCrit = 10;
		weaponPrice = 100;
		break;
	case STEEL_SWORD:
		m_image = IMAGEMANAGER->findImage("∞≠√∂∞À");
		sprintf_s(itemNameStr, "Steel sword");
		weaponGrade = WEAPON_GRADE_E;
		weaponKind = STEEL_SWORD;
		weaponType = SWORD;

		weaponDmg = 5;
		weaponWeight = 4;
		weaponRng = 1;
		weaponHit = 75;
		weaponCrit = 10;
		weaponPrice = 300;
		break;
	case SACRED_SWORD:
		m_image = IMAGEMANAGER->findImage("Ω≈º∫∞À");
		sprintf_s(itemNameStr, "Sacred sword");
		weaponGrade = WEAPON_GRADE_D;
		weaponKind = SACRED_SWORD;
		weaponType = SWORD;

		weaponDmg = 8;
		weaponWeight = 4;
		weaponRng = 1;
		weaponHit = 80;
		weaponCrit = 15;
		weaponPrice = 300;
		break;
	case IRON_AXE:
		m_image = IMAGEMANAGER->findImage("√∂µµ≥¢");
		sprintf_s(itemNameStr, "Iron axe");
		weaponGrade = WEAPON_GRADE_E;
		weaponKind = IRON_AXE;
		weaponType = AXE;

		weaponDmg = 5;
		weaponWeight = 5;
		weaponRng = 1;
		weaponHit = 75;
		weaponCrit = 5;
		weaponPrice = 100;
		break;
	case STEEL_AXE:
		m_image = IMAGEMANAGER->findImage("∞≠√∂µµ≥¢");
		sprintf_s(itemNameStr, "Steel axe");
		weaponGrade = WEAPON_GRADE_E;
		weaponKind = STEEL_AXE;
		weaponType = AXE;

		weaponDmg = 7;
		weaponWeight = 7;
		weaponRng = 1;
		weaponHit = 70;
		weaponCrit = 10;
		weaponPrice = 300;
		break;
	case THROWING_AXE:
		m_image = IMAGEMANAGER->findImage("¥¯¡ˆ¥¬µµ≥¢");
		sprintf_s(itemNameStr, "Throwing axe");
		weaponGrade = WEAPON_GRADE_E;
		weaponKind = THROWING_AXE;
		weaponType = AXE;

		weaponDmg = 4;
		weaponWeight = 5;
		weaponRng = 2;
		weaponHit = 80;
		weaponCrit = 15;
		weaponPrice = 250;
		break;
	case IRON_SPEAR:
		m_image = IMAGEMANAGER->findImage("√∂√¢");
		sprintf_s(itemNameStr, "Iron spear");
		weaponGrade = WEAPON_GRADE_E;
		weaponKind = IRON_SPEAR;
		weaponType = SPEAR;

		weaponDmg = 4;
		weaponWeight = 3;
		weaponRng = 1;
		weaponHit = 80;
		weaponCrit = 10;
		weaponPrice = 100;
		break;
	case STEEL_SPEAR:
		m_image = IMAGEMANAGER->findImage("∞≠√∂√¢");
		sprintf_s(itemNameStr, "Steel spear");
		weaponGrade = WEAPON_GRADE_E;
		weaponKind = STEEL_SPEAR;
		weaponType = SPEAR;

		weaponDmg = 6;
		weaponWeight = 5;
		weaponRng = 1;
		weaponHit = 75;
		weaponCrit = 15;
		weaponPrice = 300;
		break;
	case JAVELIN_SPEAR:
		m_image = IMAGEMANAGER->findImage("¿⁄∫ß∏∞");
		sprintf_s(itemNameStr, "Javelin");
		weaponGrade = WEAPON_GRADE_E;
		weaponKind = JAVELIN_SPEAR;
		weaponType = SPEAR;

		weaponDmg = 3;
		weaponWeight = 4;
		weaponRng = 2;
		weaponHit = 80;
		weaponCrit = 20;
		weaponPrice = 400;
		break;
	case IRON_BOW:
		m_image = IMAGEMANAGER->findImage("√∂»∞");
		sprintf_s(itemNameStr, "Iron bow");
		weaponGrade = WEAPON_GRADE_E;
		weaponKind = IRON_BOW;
		weaponType = BOW;

		weaponDmg = 4;
		weaponWeight = 1;
		weaponRng = 2;
		weaponHit = 80;
		weaponCrit = 20;
		weaponPrice = 100;
		break;
	case STEEL_BOW:
		m_image = IMAGEMANAGER->findImage("∞≠√∂»∞");
		sprintf_s(itemNameStr, "Steel bow");
		weaponGrade = WEAPON_GRADE_E;
		weaponKind = STEEL_BOW;
		weaponType = BOW;

		weaponDmg = 5;
		weaponWeight = 3;
		weaponRng = 2;
		weaponHit = 75;
		weaponCrit = 25;
		weaponPrice = 300;
		break;
	default:
		break;
	}
}

void itemSet::setConsumableItem(CONSUMABLE_KIND _consumablekind)
{
	itemType = ITEM_CONSUMABLE;

	switch (_consumablekind)
	{
	case CONSUMABLE_NONE:
		break;
	case VULNERARY:
		m_image = IMAGEMANAGER->findImage("ªÛ√≥æ‡");
		sprintf_s(itemNameStr, "Vulnerary");
		itemPower = 10;
		break;
	default:
		break;
	}
}
