#include "stdafx.h"
#include "UI_tileInfo.h"
#include "unitSet.h"
#include "tile.h"


UI_tileInfo::UI_tileInfo()
{
}


UI_tileInfo::~UI_tileInfo()
{
}

HRESULT UI_tileInfo::init()
{
	isCommand = false;
	isTagUnit = false;
	tileFront = IMAGEMANAGER->addImage("타일정보메인", "images/UI/tileinfo_front.bmp", 150, 165, true, RGB(255, 0, 255));
	tileNameBack = IMAGEMANAGER->addImage("타일이름백", "images/UI/tileinfo_nameback.bmp", 108, 54, true, RGB(255, 0, 255));

	unitFront = IMAGEMANAGER->addImage("유닛정보메인", "images/UI/playerinfo_firstback.bmp", 288, 144, true, RGB(255, 0, 255));
	unitHp = IMAGEMANAGER->addImage("유닛정보체력문자", "images/UI/playerinfo_first_hp.bmp", 48, 24, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("체력바앞쪽", "images/UI/hpbar_front.bmp", 138, 15,true,RGB(255,0,255));
	IMAGEMANAGER->addImage("체력바뒤쪽", "images/UI/hpbar_back.bmp", 138, 15, true, RGB(255, 0, 255));
	
	hpBar = new progressBar;
	hpBar->init("images/UI/hpbar_front.bmp", "images/UI/hpbar_back.bmp", 0, 0, 138, 15);
	hpBar->setGauge(10, 10);
	return S_OK;
}

void UI_tileInfo::update()
{
	//타일 정보 부분
	if (g_cursor.getPosition().x < 315)
	{
		rc_tileFront = RectMakeCenter((WINSIZEX - tileFront->getWidth() / 2) - 10, (WINSIZEY - tileFront->getHeight() / 2) - 10,150,165);		
	}
	else if (g_cursor.getPosition().x > 315)
	{
		rc_tileFront = RectMakeCenter(10 + (tileFront->getWidth() / 2), (WINSIZEY - tileFront->getHeight() / 2) - 10, 150, 165);
	}

	rc_tileNameBack = RectMake(rc_tileFront.left + 22, rc_tileFront.top + 40, 108, 54);
	rc_tileNameStr = RectMake(rc_tileNameBack.left + 7, rc_tileNameBack.top + 16, 96, 37);
	rc_tileDefStr = RectMake(rc_tileFront.left + 85, rc_tileFront.top + 92, 36, 21);
	rc_tileAvoStr = RectMake(rc_tileFront.left + 85, rc_tileFront.top + 116, 36, 21);

	//온 타일 유닛 정보 부분
	if (g_cursor.getCurTile()->getOnTileUnit() != nullptr)
	{
		isTagUnit = true;
		hpBar->setGauge(g_cursor.getCurTile()->getOnTileUnit()->getCurHp(), g_cursor.getCurTile()->getOnTileUnit()->getMaxHp());
	}
	else if (g_cursor.getCurTile()->getOnTileUnit() == nullptr)
	{
		isTagUnit = false;
	}
	

	if (isTagUnit)
	{
		if (g_cursor.getPosition().y < 240 && g_cursor.getPosition().x < 315)
		{
			rc_unitFront = RectMakeCenter((unitFront->getWidth() / 2), WINSIZEY - (unitFront->getHeight() / 2), 288, 144);
		}
		else if (g_cursor.getPosition().x > 315 || g_cursor.getPosition().y > 240 && g_cursor.getPosition().x < 315)
		{
			rc_unitFront = RectMakeCenter((unitFront->getWidth() / 2), (unitFront->getHeight() / 2), 288, 144);
		}
		rc_unitHpStr = RectMake(rc_unitFront.left + 175, rc_unitFront.top + 73, 90, 24);
		rc_unitNameStr = RectMake(rc_unitFront.left + 140, rc_unitFront.top + 37, 110, 33);
		rc_unitSmallMugShot = RectMake(rc_unitFront.left + 25, rc_unitFront.top + 25, 96, 96);
		//온 타일 유닛 hp
		hpBar->setX(rc_unitFront.left + 124);
		hpBar->setY(rc_unitFront.top + 103);
		hpBar->update();
	}
}

void UI_tileInfo::animation()
{
}

void UI_tileInfo::render()
{
	if (isTurning)	return;	
	if (isBattleStart) return;
	if (isEnemyTurn) return;
	HFONT myFont = CreateFont(26, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "둥근모꼴");
	HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
	SetBkMode(getMemDC(), TRANSPARENT);
	if (!isCommandStart)
	{
		
		//타일 정보
		tileFront->render(getMemDC(), rc_tileFront.left, rc_tileFront.top);
		tileNameBack->alphaRender(getMemDC(), rc_tileNameBack.left, rc_tileNameBack.top, 122);
		
		SetTextColor(getMemDC(), RGB(255, 255, 255));
		DrawText(getMemDC(), g_cursor.getCurTile()->getTileNameStr(), -1, &rc_tileNameStr, DT_CENTER);
		SetTextColor(getMemDC(), RGB(0, 0, 0));
		DrawText(getMemDC(), g_cursor.getCurTile()->getTileDefStr(), -1, &rc_tileDefStr, DT_RIGHT);
		DrawText(getMemDC(), g_cursor.getCurTile()->getTileAvoStr(), -1, &rc_tileAvoStr, DT_RIGHT);

		//온 타일 유닛 정보
		if (isTagUnit)
		{
			unitFront->alphaRender(getMemDC(), rc_unitFront.left, rc_unitFront.top, 180);
			unitHp->render(getMemDC(), rc_unitFront.left + 121, rc_unitFront.top + 73);
			g_cursor.getCurTile()->getOnTileUnit()->getSmallMugShot()->render(getMemDC(), rc_unitSmallMugShot.left, rc_unitSmallMugShot.top);
			sprintf_s(unitHpStr, "%d/%d", g_cursor.getCurTile()->getOnTileUnit()->getCurHp(),
				g_cursor.getCurTile()->getOnTileUnit()->getMaxHp());
			DrawText(getMemDC(), unitHpStr, -1, &rc_unitHpStr, DT_CENTER);
			DrawText(getMemDC(), g_cursor.getCurTile()->getOnTileUnit()->getUnitNameStr(), -1, &rc_unitNameStr, DT_CENTER);
			hpBar->render();
		}
	}	

	SelectObject(getMemDC(), oldFont);
	DeleteObject(myFont);
}

void UI_tileInfo::release()
{
	SAFE_DELETE(hpBar);
}
