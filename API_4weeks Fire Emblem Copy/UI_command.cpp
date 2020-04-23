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
	//�̵�Ȯ�� ���� �ʱ�ȭ �κ�
	IMAGEMANAGER->addImage("�ൿ��������_������2", "images/UI/unitui_scale2.bmp", 147, 123, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�ൿ��������_������3", "images/UI/unitui_scale3.bmp", 147, 171, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�ൿ��������_������4", "images/UI/unitui_scale4.bmp", 147, 219, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("�ൿ����_����", "images/UI/attack.bmp", 90, 33, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�ൿ����_�ŷ�", "images/UI/trade.bmp", 81, 33, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�ൿ����_������", "images/UI/item.bmp", 60, 33, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�ൿ����_����", "images/UI/vendor.bmp", 93, 33, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�ൿ����_���", "images/UI/wait.bmp", 57, 33, true, RGB(255, 0, 255));

	moveConfirmFront = IMAGEMANAGER->findImage("�ൿ��������_������2");
	moveConfirmFrontNum = 2;

	for (int i = 0; i < 5; i++)
	{
		commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_���");
	}

	isAtkPossible = false;
	isTradePossible = false;
	isOnVender = false;
	commandState = COMMAND_MOVECONFIRM;

	//���ݾ����� ���� �ʱ�ȭ �κ�
	IMAGEMANAGER->addImage("���ݾ����ۼ���_������1", "images/UI/atkitemui_scale1.bmp", 315, 75, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("���ݾ����ۼ���_������2", "images/UI/atkitemui_scale2.bmp", 315, 123, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("���ݾ����ۼ���_������3", "images/UI/atkitemui_scale3.bmp", 315, 171, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("���ݽ���", "images/UI/atkstats.bmp", 315, 171, true, RGB(255, 0, 255));
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
	HFONT myFont = CreateFont(30, 0, 0, 0, 300, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�ձٸ��");
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
		//âũ�� ���� ����
		if (isAtkPossible) moveConfirmFrontNum += 1;
		if (isTradePossible) moveConfirmFrontNum += 1;
		if (isOnVender) moveConfirmFrontNum += 1;
		//������ ������ �������� UIâ ũ�� ����
		switch (moveConfirmFrontNum)
		{
		case 2:
			rc_moveConfirmFront = RectMake((WINSIZEX - 180), 87, 147, 123);
			moveConfirmFront = IMAGEMANAGER->findImage("�ൿ��������_������2");
			break;
		case 3:
			rc_moveConfirmFront = RectMake((WINSIZEX - 180), 87, 147, 171);
			moveConfirmFront = IMAGEMANAGER->findImage("�ൿ��������_������3");
			break;
		case 4:
			rc_moveConfirmFront = RectMake((WINSIZEX - 180), 87, 147, 219);
			moveConfirmFront = IMAGEMANAGER->findImage("�ൿ��������_������4");
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
		atkStatusFront = IMAGEMANAGER->findImage("���ݽ���");
		switch (g_cursor.getSelectedUnit()->getInven().size())
		{
		case 1:
			rc_atkItemSelectFront = RectMake(39, 39, 315, 75);
			atkItemSelectFront = IMAGEMANAGER->findImage("���ݾ����ۼ���_������1");
			break;
		case 2:
			rc_atkItemSelectFront = RectMake(39, 39, 315, 123);
			atkItemSelectFront = IMAGEMANAGER->findImage("���ݾ����ۼ���_������2");
			break;
		case 3:
			rc_atkItemSelectFront = RectMake(39, 39, 315, 171);
			atkItemSelectFront = IMAGEMANAGER->findImage("���ݾ����ۼ���_������3");
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
		//Ŀ�ǵ��� �̹��� �ٲ��ֱ�
		for (int i = 0; i < moveConfirmFrontNum; i++)
		{
			if (i == 0)
			{
				if (isAtkPossible)
				{
					commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_����");
					commandBar[i].commandType = COMMAND_ATK;
				}
				else if (isTradePossible)
				{
					commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_�ŷ�");
					commandBar[i].commandType = COMMAND_TRADE;
				}
				else if (isOnVender)
				{
					commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_����");
					commandBar[i].commandType = COMMAND_VENDOR;
				}
				else
				{
					commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_������");
					commandBar[i].commandType = COMMAND_ITEM;
				}
			}
			if (i == 1)
			{
				if (isAtkPossible)
				{
					if (isTradePossible)
					{
						commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_�ŷ�");
						commandBar[i].commandType = COMMAND_TRADE;
					}
					else if (isOnVender)
					{
						commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_����");
						commandBar[i].commandType = COMMAND_VENDOR;
					}
					else
					{
						commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_������");
						commandBar[i].commandType = COMMAND_ITEM;
					}
				}
				else if (isTradePossible)
				{
					if (isOnVender)
					{
						commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_����");
						commandBar[i].commandType = COMMAND_VENDOR;
					}
					else if (moveConfirmFrontNum >= 3)
					{
						commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_������");
						commandBar[i].commandType = COMMAND_ITEM;
					}
				}
				else
				{
					commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_���");
					commandBar[i].commandType = COMMAND_WAIT;
				}
			}
			if (i == 2)
			{
				if (isAtkPossible && isTradePossible)
				{
					if (isOnVender)
					{
						commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_����");
						commandBar[i].commandType = COMMAND_VENDOR;
					}
					else
					{
						commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_������");
						commandBar[i].commandType = COMMAND_ITEM;
					}
				}
				else
				{
					commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_���");
					commandBar[i].commandType = COMMAND_WAIT;
				}
			}
			if (i == 3)
			{
				if (moveConfirmFrontNum > 4)
				{
					commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_������");
				}
				else
				{
					commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_���");
				}
			}
			if (i == 4)
			{
				commandBar[i].img = IMAGEMANAGER->findImage("�ൿ����_���");
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
