#include "stdafx.h"
#include "events.h"


HRESULT events::init()
{
	IMAGEMANAGER->addImage("�Դ�����", "images/Potion.bmp", 32 * 1.5, 32 * 1.5, true, RGB(255, 0, 255));
	m_position = { 0,0 };
	m_collider = RectMakeCenter(m_position.x, m_position.y, 32 * 1.5, 32 * 1.5);
	sprintf_s(eventName, "��û��");

	m_eventMaxHp = 0;
	m_eventCurHp = 0;
	m_eventAtk = 0;
	m_eventDef = 0;
	m_eventExp = 0;
	m_eventGold = 0;
	m_eventPower = 0;

	m_eventNumber = 0;
	return S_OK;
}

void events::release()
{
}

void events::update()
{

}

void events::render()
{
	m_collider = RectMakeCenter(m_position.x, m_position.y, 32 , 32 );
	m_image->render(getMemDC(), m_collider.left, m_collider.top);
	/*RectangleMake(getMemDC(), m_collider.left, m_collider.top, 32, 32);*/

	if (PtInRect(&m_collider, m_ptMouse))
	{
		RECT rc = RectMake(20, 220, 260, 100);
		m_image->render(getMemDC(), 20, 165);

		HFONT myFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�޸ո���ü");
		HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
		SetBkMode(getMemDC(), TRANSPARENT);

		sprintf_s(eventHp, "H  P : %0.f / %0.f", m_eventCurHp, m_eventMaxHp);
		TextOut(getMemDC(), 70, 160, eventHp, strlen(eventHp));
		sprintf_s(eventAtk, "ATK : %0.f", m_eventAtk);
		TextOut(getMemDC(), 70, 180, eventAtk, strlen(eventAtk));
		sprintf_s(eventDef, "DEF : %0.f", eventDef);
		TextOut(getMemDC(), 70, 200, eventDef, strlen(eventDef));
		/*	sprintf_s(monsterExpGold, "EXP : %0.f        GOLD : %0.f", m_monsterExp, m_monsterGold);
			TextOut(getMemDC(), 50, 100, monsterExpGold, strlen(monsterExpGold));*/
			/*wsprintf(cMonsterName, "LEVEL : %d", m_player->getLevel());*/
		TextOut(getMemDC(), 70, 138, eventName, strlen(eventName));
		DrawText(getMemDC(), eventExplain, -1, &rc, DT_WORDBREAK);
		/*TextOut(getMemDC(), 20, 220, eventExplain, strlen(eventExplain));*/

		SelectObject(getMemDC(), oldFont);
		DeleteObject(myFont);
	}
}

void events::animation()
{
}

void events::setEvent(EVENTTYPE _eventtype)
{
	eventType = _eventtype;

	switch (eventType)
	{
	case EVENT_NONE:
		break;
	case EVENT_FIRETRAP:
		m_image = new image;
		m_image->init("images/Fire_Trap.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(eventName, "�Ҳ� ����");
		sprintf_s(eventExplain, "������ �̴߰�.");
		m_eventMaxHp = 0;
		m_eventCurHp = m_eventMaxHp;
		m_eventAtk = 4;
		m_eventDef = 0;
		m_eventExp = 0;
		m_eventGold = 0;
		m_eventPower = 0;
 
		break;
	case EVENT_BITETRAP:
		m_image = new image;
		m_image->init("images/Bite_Trap.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(eventName, "�̻� ����");
		sprintf_s(eventExplain, "������ ������.");
		m_eventMaxHp = 0;
		m_eventCurHp = m_eventMaxHp;
		m_eventAtk = 6;
		m_eventDef = 0;
		m_eventExp = 0;
		m_eventGold = 0;
		m_eventPower = 0;
		break;
	case EVENT_CLAWTRAP:
		m_image = new image;
		m_image->init("images/Claw_Trap.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(eventName, "���� ����");
		sprintf_s(eventExplain, "�߻� ����̰� ������ ��������.");
		m_eventMaxHp = 0;
		m_eventCurHp = m_eventMaxHp;
		m_eventAtk = 10;
		m_eventDef = 0;
		m_eventExp = 0;
		m_eventGold = 0;
		m_eventPower = 0;
		break;
	case EVENT_POSIONCLAWTRAP:
		m_image = new image;
		m_image->init("images/PoisonClaw_Trap.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(eventName, "������ ����");
		sprintf_s(eventExplain, "�ߵ����ؼ� 3���� �������� �ɷ�ġ�� ��������.");
		m_eventMaxHp = 0;
		m_eventCurHp = m_eventMaxHp;
		m_eventAtk = 5;
		m_eventDef = 0;
		m_eventExp = 0;
		m_eventGold = 0;
		m_eventPower = 0;
		break;
	case EVENT_WINDTRAP:
		m_image = new image;
		m_image->init("images/Wind_Trap.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(eventName, "�ٶ� ����");
		sprintf_s(eventExplain, "�ٶ��� �ʹ� �ż��� �������� �����.");
		m_eventMaxHp = 0;
		m_eventCurHp = m_eventMaxHp;
		m_eventAtk = 20;
		m_eventDef = 0;
		m_eventExp = 0;
		m_eventGold = 0;
		m_eventPower = 0;
		break;
	case EVENT_HPSHRINE:
		m_image = new image;
		m_image->init("images/Hp_Shrine.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(eventName, "ġ�� ���");
		sprintf_s(eventExplain, "��տ� ���� ��� ���� �������°� ��������. (HP 20 ȸ��)");
		m_eventMaxHp = 0;
		m_eventCurHp = m_eventMaxHp;
		m_eventAtk = 0;
		m_eventDef = 0;
		m_eventExp = 0;
		m_eventGold = 0;
		m_eventPower = 20;
		break;
	case EVENT_DEFSHRINE:
		m_image = new image;
		m_image->init("images/Def_Shrine.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(eventName, "��� ���");
		sprintf_s(eventExplain, "��տ� ���� ��� 3���� �������� ������ �����Ѵ�.");
		m_eventMaxHp = 0;
		m_eventCurHp = m_eventMaxHp;
		m_eventAtk = 0;
		m_eventDef = 0;
		m_eventExp = 0;
		m_eventGold = 0;
		m_eventPower = 2;
		break;
	case EVENT_EXPSHRINE:
		m_image = new image;
		m_image->init("images/Exp_Shrine.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(eventName, "����ġ ���");
		sprintf_s(eventExplain, "��տ� ���� ��� �Ӹ��� ��������. (EXP ����)");
		m_eventMaxHp = 0;
		m_eventCurHp = m_eventMaxHp;
		m_eventAtk = 0;
		m_eventDef = 0;
		m_eventExp = 0;
		m_eventGold = 0;
		m_eventPower = 10;
		break;
	case EVENT_GOLDBAG:
		m_image = new image;
		m_image->init("images/Gold.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(eventName, "��� ����");
		sprintf_s(eventExplain, "������ �긮�� ����? (GOLD ȹ��)");
		m_eventMaxHp = 0;
		m_eventCurHp = m_eventMaxHp;
		m_eventAtk = 0;
		m_eventDef = 0;
		m_eventExp = 0;
		m_eventGold = 0;
		m_eventPower = 10;
		break;
	case EVENT_VENDER:
		m_image = new image;
		m_image->init("images/Vender.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(eventName, "����");
		sprintf_s(eventExplain, "�������� ���� �� �� �ִ�. SPACE Ű�� �̿��ؼ� ĳ���͸� �������� ���߰� �ؾ� ������ �����ϴ�.");
		m_eventMaxHp = 0;
		m_eventCurHp = m_eventMaxHp;
		m_eventAtk = 0;
		m_eventDef = 0;
		m_eventExp = 0;
		m_eventGold = 0;
		m_eventPower = 0;
		break;
	case EVENT_CLASSCHANGE:
		m_image = new image;
		m_image->init("images/Change1.bmp", 32, 32, true, RGB(255, 0, 255));
		sprintf_s(eventName, "���� å");
		sprintf_s(eventExplain, "�ż��� ���� ��������.");
		m_eventMaxHp = 0;
		m_eventCurHp = m_eventMaxHp;
		m_eventAtk = 0;
		m_eventDef = 0;
		m_eventExp = 0;
		m_eventGold = 0;
		m_eventPower = 0;
		break;
	default:
		break;
	}
}

events::events()
{
}


events::~events()
{
}
