#include "stdafx.h"
#include "UI_command.h"
#include "unitSet.h"
#include "itemSet.h"

UI_command::UI_command()
{
}


UI_command::~UI_command()
{
}

HRESULT UI_command::init()
{
	//이동확정 선택 초기화 부분
	IMAGEMANAGER->addImage("행동결정메인_스케일2", "images/UI/unitui_scale2.bmp", 147, 123, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("행동결정메인_스케일3", "images/UI/unitui_scale3.bmp", 147, 171, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("행동결정메인_스케일4", "images/UI/unitui_scale4.bmp", 147, 219, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("행동결정_공격", "images/UI/attack.bmp", 90, 33, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("행동결정_거래", "images/UI/trade.bmp", 81, 33, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("행동결정_아이템", "images/UI/item.bmp", 60, 33, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("행동결정_상점", "images/UI/vendor.bmp", 93, 33, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("행동결정_대기", "images/UI/wait.bmp", 57, 33, true, RGB(255, 0, 255));

	moveConfirmFront = IMAGEMANAGER->findImage("행동결정메인_스케일2");
	moveConfirmFrontNum = 2;

	for (int i = 0; i < 5; i++)
	{
		commandBar[i].img = IMAGEMANAGER->findImage("행동결정_대기");
	}

	isAtkPossible = false;
	isTradePossible = false;
	isOnVender = false;
	commandState = COMMAND_MOVECONFIRM;

	//공격아이템 선택 초기화 부분
	IMAGEMANAGER->addImage("공격아이템선택_스케일1", "images/UI/atkitemui_scale1.bmp", 315, 75, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("공격아이템선택_스케일2", "images/UI/atkitemui_scale2.bmp", 315, 123, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("공격아이템선택_스케일3", "images/UI/atkitemui_scale3.bmp", 315, 171, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("공격스탯", "images/UI/atkstats.bmp", 315, 171, true, RGB(255, 0, 255));
	return S_OK;
}

void UI_command::update()
{
	if (isCommandStart)
	{
		switch (commandState)
		{
		case COMMAND_MOVECONFIRM:		
			for (int i = 0; i < moveConfirmFrontNum; i++)
			{
				commandBar[i].position.x = commandBar[i].rc.left;
				commandBar[i].position.y = commandBar[i].rc.top;
			}
			
			for (int i = 0; i < moveConfirmFrontNum; i++)
			{
				commandBar[i].rc = RectMake(rc_moveConfirmFront.left + 15,
					(rc_moveConfirmFront.top + 24) + (33 * i) + (15 * i),
					commandBar[i].img->getWidth(), commandBar[i].img->getHeight());
			}
		break;

		case COMMAND_ATKITEMCONFIRM:
			for (int i = 0; i < g_cursor.getSelectedUnit()->getInven().size(); i++)
			{
				itemBar[i].position.x = itemBar[i].rc.left;
				itemBar[i].position.y = itemBar[i].rc.top;
			}

			for (int i = 0; i < g_cursor.getSelectedUnit()->getInven().size(); i++)
			{
				itemBar[i].rc = RectMake(rc_atkItemSelectFront.left + 12,
					(rc_atkItemSelectFront.top + 12) + (48 * i), 48, 48);
				rc_itemName[i] = RectMake(rc_atkItemSelectFront.left + 60,
					(rc_atkItemSelectFront.top + 18) + (48 * i), 192, 48);
			}
		break;

		default:
			break;
		}
	}		
}

void UI_command::animation()
{
}

void UI_command::render()
{
	HFONT myFont = CreateFont(30, 0, 0, 0, 300, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "둥근모꼴");
	HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	SetBkMode(getMemDC(), TRANSPARENT);

	if (isCommandStart && !isBattleStart)
	{
		switch (commandState)
		{
		case COMMAND_MOVECONFIRM:
			moveConfirmFront->render(getMemDC(), rc_moveConfirmFront.left, rc_moveConfirmFront.top);

			for (int i = 0; i < moveConfirmFrontNum; i++)
			{
				commandBar[i].img->render(getMemDC(), commandBar[i].rc.left, commandBar[i].rc.top);
			}
			break;
		case COMMAND_ATKITEMCONFIRM:
			

			atkItemSelectFront->render(getMemDC(), rc_atkItemSelectFront.left, rc_atkItemSelectFront.top);
			atkStatusFront->render(getMemDC(), rc_atkStatusFront.left, rc_atkStatusFront.top);
			for (int i = 0; i < g_cursor.getSelectedUnit()->getInven().size(); i++)
			{
				itemBar[i].img->render(getMemDC(), itemBar[i].rc.left, itemBar[i].rc.top);
				DrawText(getMemDC(), g_cursor.getSelectedUnit()->getInven()[i]->getItemName(), -1, &rc_itemName[i], DT_LEFT);				
			}
			sprintf_s(afterAtkStatusStr, "%d", g_cursor.getSelectedUnit()->getStrength() +
				g_cursor.getSelectedUnit()->getInven()[g_cursor.getMenuNum()]->getWeaponDmg());
			sprintf_s(afterCritStatusStr, "%d", g_cursor.getSelectedUnit()->getLuck() +
				g_cursor.getSelectedUnit()->getInven()[g_cursor.getMenuNum()]->getWeaponCrit());
			sprintf_s(afterHitStatusStr, "%d", g_cursor.getSelectedUnit()->getSkill() +
				g_cursor.getSelectedUnit()->getInven()[g_cursor.getMenuNum()]->getWeaponHit());
			sprintf_s(afterAvoidStatusStr, "%d", g_cursor.getSelectedUnit()->getAvoid());
			DrawText(getMemDC(), afterAtkStatusStr, -1, &rc_atkStatusStr, DT_RIGHT);
			DrawText(getMemDC(), afterCritStatusStr, -1, &rc_critStatusStr, DT_RIGHT);
			DrawText(getMemDC(), afterHitStatusStr, -1, &rc_hitStatusStr, DT_RIGHT);
			DrawText(getMemDC(), afterAvoidStatusStr, -1, &rc_avoidStatusStr, DT_RIGHT);
		
			break;
		default:
			break;
		}
	}	

	SelectObject(getMemDC(), oldFont);
	DeleteObject(myFont);
}

void UI_command::release()
{
}

void UI_command::adjustCommandWindow()
{
	switch (commandState)
	{
	case COMMAND_MOVECONFIRM:
		//창크기 정보 수집
		if (isAtkPossible) moveConfirmFrontNum += 1;
		if (isTradePossible) moveConfirmFrontNum += 1;
		if (isOnVender) moveConfirmFrontNum += 1;
		//수집된 정보를 바탕으로 UI창 크기 설정
		switch (moveConfirmFrontNum)
		{
		case 2:
			rc_moveConfirmFront = RectMake((WINSIZEX - 180), 87, 147, 123);
			moveConfirmFront = IMAGEMANAGER->findImage("행동결정메인_스케일2");
			break;
		case 3:
			rc_moveConfirmFront = RectMake((WINSIZEX - 180), 87, 147, 171);
			moveConfirmFront = IMAGEMANAGER->findImage("행동결정메인_스케일3");
			break;
		case 4:
			rc_moveConfirmFront = RectMake((WINSIZEX - 180), 87, 147, 219);
			moveConfirmFront = IMAGEMANAGER->findImage("행동결정메인_스케일4");
			break;
		default:
			break;
		}
		break;
	case COMMAND_ATKITEMCONFIRM:
		rc_atkStatusFront = RectMake(375, 279, 315, 171);
		rc_atkStatusStr = RectMake(rc_atkStatusFront.left + 66, rc_atkStatusFront.top + 72, 66, 33);
		rc_critStatusStr = RectMake(rc_atkStatusFront.left + 195, rc_atkStatusFront.top + 72, 99, 33);
		rc_hitStatusStr = RectMake(rc_atkStatusFront.left + 57, rc_atkStatusFront.top + 120, 75, 33);
		rc_avoidStatusStr = RectMake(rc_atkStatusFront.left + 219, rc_atkStatusFront.top + 120, 75, 33);
		atkStatusFront = IMAGEMANAGER->findImage("공격스탯");
		switch (g_cursor.getSelectedUnit()->getInven().size())
		{
		case 1:
			rc_atkItemSelectFront = RectMake(39, 39, 315, 75);
			atkItemSelectFront = IMAGEMANAGER->findImage("공격아이템선택_스케일1");
			break;
		case 2:
			rc_atkItemSelectFront = RectMake(39, 39, 315, 123);
			atkItemSelectFront = IMAGEMANAGER->findImage("공격아이템선택_스케일2");
			break;
		case 3:
			rc_atkItemSelectFront = RectMake(39, 39, 315, 171);
			atkItemSelectFront = IMAGEMANAGER->findImage("공격아이템선택_스케일3");
			break;
		default:
			break;
		}
		
		break;
	default:
		break;
	}
	
}

void UI_command::arrangementCommand()
{
	switch (commandState)
	{
	case COMMAND_MOVECONFIRM:
		//커맨드의 이미지 바꿔주기
		for (int i = 0; i < moveConfirmFrontNum; i++)
		{
			if (i == 0)
			{
				if (isAtkPossible)
				{
					commandBar[i].img = IMAGEMANAGER->findImage("행동결정_공격");
					commandBar[i].commandType = COMMAND_ATK;
				}
				else if (isTradePossible)
				{
					commandBar[i].img = IMAGEMANAGER->findImage("행동결정_거래");
					commandBar[i].commandType = COMMAND_TRADE;
				}
				else if (isOnVender)
				{
					commandBar[i].img = IMAGEMANAGER->findImage("행동결정_상점");
					commandBar[i].commandType = COMMAND_VENDOR;
				}
				else
				{
					commandBar[i].img = IMAGEMANAGER->findImage("행동결정_아이템");
					commandBar[i].commandType = COMMAND_ITEM;
				}
			}
			if (i == 1)
			{
				if (isAtkPossible)
				{
					if (isTradePossible)
					{
						commandBar[i].img = IMAGEMANAGER->findImage("행동결정_거래");
						commandBar[i].commandType = COMMAND_TRADE;
					}
					else if (isOnVender)
					{
						commandBar[i].img = IMAGEMANAGER->findImage("행동결정_상점");
						commandBar[i].commandType = COMMAND_VENDOR;
					}
					else
					{
						commandBar[i].img = IMAGEMANAGER->findImage("행동결정_아이템");
						commandBar[i].commandType = COMMAND_ITEM;
					}
				}
				else if (isTradePossible)
				{
					if (isOnVender)
					{
						commandBar[i].img = IMAGEMANAGER->findImage("행동결정_상점");
						commandBar[i].commandType = COMMAND_VENDOR;
					}
					else if (moveConfirmFrontNum >= 3)
					{
						commandBar[i].img = IMAGEMANAGER->findImage("행동결정_아이템");
						commandBar[i].commandType = COMMAND_ITEM;
					}
				}
				else
				{
					commandBar[i].img = IMAGEMANAGER->findImage("행동결정_대기");
					commandBar[i].commandType = COMMAND_WAIT;
				}
			}
			if (i == 2)
			{
				if (isAtkPossible && isTradePossible)
				{
					if (isOnVender)
					{
						commandBar[i].img = IMAGEMANAGER->findImage("행동결정_상점");
						commandBar[i].commandType = COMMAND_VENDOR;
					}
					else
					{
						commandBar[i].img = IMAGEMANAGER->findImage("행동결정_아이템");
						commandBar[i].commandType = COMMAND_ITEM;
					}
				}
				else
				{
					commandBar[i].img = IMAGEMANAGER->findImage("행동결정_대기");
					commandBar[i].commandType = COMMAND_WAIT;
				}
			}
			if (i == 3)
			{
				if (moveConfirmFrontNum > 4)
				{
					commandBar[i].img = IMAGEMANAGER->findImage("행동결정_아이템");
				}
				else
				{
					commandBar[i].img = IMAGEMANAGER->findImage("행동결정_대기");
				}
			}
			if (i == 4)
			{
				commandBar[i].img = IMAGEMANAGER->findImage("행동결정_대기");
			}
		}
		break;
	case COMMAND_ATKITEMCONFIRM:
		for (int i = 0; i < g_cursor.getSelectedUnit()->getInven().size(); i++)
		{
			itemBar[i].img = g_cursor.getSelectedUnit()->getInven()[i]->getImage();
		}		
		break;
	default:
		break;
	}
	
}
