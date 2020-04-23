#include "stdafx.h"
#include "UI_battleinfo.h"
#include "tile.h"
#include "unitSet.h"

UI_battleinfo::UI_battleinfo()
{
}


UI_battleinfo::~UI_battleinfo()
{
}

HRESULT UI_battleinfo::init()
{
	battleInfo = IMAGEMANAGER->addImage("전투체력정보", "images/UI/battleinfo.bmp", 477, 87, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("전투체력바앞쪽", "images/UI/battlehp_front.bmp", 141, 18, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("전투체력바뒤쪽", "images/UI/battlehp_back.bmp", 141, 18, true, RGB(255, 0, 255));

	allyHpBar = new progressBar;
	allyHpBar->init("images/UI/battlehp_front.bmp", "images/UI/battlehp_back.bmp", 0, 0, 141, 18);
	allyHpBar->setGauge(10, 10);

	enemyHpBar = new progressBar;
	enemyHpBar->init("images/UI/battlehp_front.bmp", "images/UI/battlehp_back.bmp", 0, 0, 141, 18);
	enemyHpBar->setGauge(10, 10);

	isBattleStart = false;
	return S_OK;
}

void UI_battleinfo::update()
{
	if (isBattleStart)
	{
		//커서가 위쪽에 있으면	
		if (g_cursor.getSelectedUnit()->getPosition().y - CAMERA->m_cameraPos.y <= 144)
		{
			rc_battleInfo = RectMake(123 - CAMERA->m_cameraPos.x, g_cursor.getSelectedUnit()->getOccupiedTile()->getPosition().y + 120 - CAMERA->m_cameraPos.y, 477, 87);
		}
		//보통때
		else
		{
			rc_battleInfo = RectMake(123 - CAMERA->m_cameraPos.x, g_cursor.getSelectedUnit()->getOccupiedTile()->getPosition().y - 168 - CAMERA->m_cameraPos.y, 477, 87);
		}

		rc_allyHpStr = RectMake(rc_battleInfo.left + 22, rc_battleInfo.top + 49, 48, 24);
		rc_allyNameStr = RectMake(rc_battleInfo.left + 31, rc_battleInfo.top + 10, 174, 33);
		rc_enemyHpStr = RectMake(rc_battleInfo.left + 262, rc_battleInfo.top + 49, 48, 24);
		rc_enemyNameStr = RectMake(rc_battleInfo.left + 271, rc_battleInfo.top + 10, 174, 33);

		if (isEnemyTurn)
		{
			if (g_cursor.getSelectedUnit() != nullptr)
			{
				enemyHpBar->setGauge(g_cursor.getSelectedUnit()->getCurHp(), g_cursor.getSelectedUnit()->getMaxHp());
				enemyHpBar->setX(rc_battleInfo.left + 315);
				enemyHpBar->setY(rc_battleInfo.top + 54);
				enemyHpBar->update();
			}
			if (g_cursor.getReceiveUnit() != nullptr)
			{
				allyHpBar->setGauge(g_cursor.getReceiveUnit()->getCurHp(), g_cursor.getReceiveUnit()->getMaxHp());
				allyHpBar->setX(rc_battleInfo.left + 75);
				allyHpBar->setY(rc_battleInfo.top + 54);
				allyHpBar->update();
			}
		}
		else if (isPlayerTurn)
		{
			if (g_cursor.getSelectedUnit() != nullptr)
			{
				allyHpBar->setGauge(g_cursor.getSelectedUnit()->getCurHp(), g_cursor.getSelectedUnit()->getMaxHp());
				allyHpBar->setX(rc_battleInfo.left + 75);
				allyHpBar->setY(rc_battleInfo.top + 54);
				allyHpBar->update();
			}
			if (g_cursor.getReceiveUnit() != nullptr)
			{
				enemyHpBar->setGauge(g_cursor.getReceiveUnit()->getCurHp(), g_cursor.getReceiveUnit()->getMaxHp());
				enemyHpBar->setX(rc_battleInfo.left + 315);
				enemyHpBar->setY(rc_battleInfo.top + 54);
				enemyHpBar->update();
			}
		}
	}	
}

void UI_battleinfo::animation()
{

}

void UI_battleinfo::render()
{
	HFONT myFont = CreateFont(26, 0, 0, 0, 300, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "둥근모꼴");
	HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	SetBkMode(getMemDC(), TRANSPARENT);

	if (isBattleStart)
	{
		battleInfo->render(getMemDC(), rc_battleInfo.left, rc_battleInfo.top);
		allyHpBar->render();
		enemyHpBar->render();
		if (isEnemyTurn)
		{
			DrawText(getMemDC(), g_cursor.getReceiveUnit()->getUnitNameStr(), -1, &rc_allyNameStr, DT_CENTER);
			DrawText(getMemDC(), g_cursor.getSelectedUnit()->getUnitNameStr(), -1, &rc_enemyNameStr, DT_CENTER);
			sprintf_s(allyHpStr, "%d", g_cursor.getReceiveUnit()->getCurHp());
			sprintf_s(enemyHpStr, "%d", g_cursor.getSelectedUnit()->getCurHp());
			DrawText(getMemDC(), allyHpStr, -1, &rc_allyHpStr, DT_CENTER);
			DrawText(getMemDC(), enemyHpStr, -1, &rc_enemyHpStr, DT_CENTER);
		}
		else if (isPlayerTurn)
		{
			DrawText(getMemDC(), g_cursor.getSelectedUnit()->getUnitNameStr(), -1, &rc_allyNameStr, DT_CENTER);
			DrawText(getMemDC(), g_cursor.getReceiveUnit()->getUnitNameStr(), -1, &rc_enemyNameStr, DT_CENTER);
			sprintf_s(allyHpStr, "%d", g_cursor.getSelectedUnit()->getCurHp());
			sprintf_s(enemyHpStr, "%d", g_cursor.getReceiveUnit()->getCurHp());
			DrawText(getMemDC(), allyHpStr, -1, &rc_allyHpStr, DT_CENTER);
			DrawText(getMemDC(), enemyHpStr, -1, &rc_enemyHpStr, DT_CENTER);
		}
		
	}

	SelectObject(getMemDC(), oldFont);
	DeleteObject(myFont);
}

void UI_battleinfo::release()
{
	SAFE_DELETE(allyHpBar);
	SAFE_DELETE(enemyHpBar);
}
