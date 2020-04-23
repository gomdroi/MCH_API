#include "stdafx.h"
#include "eventManager.h"
#include "events.h"


HRESULT eventManager::init()
{
	IMAGEMANAGER->addImage("불함정", "images/Fire_Trap.bmp", 32 * 1.5, 32 * 1.5, true, RGB(255, 0, 255));	
	IMAGEMANAGER->addImage("이빨함정", "images/Bite_Trap.bmp", 32 * 1.5, 32 * 1.5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("발톱함정", "images/Claw_Trap.bmp", 32 * 1.5, 32 * 1.5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("독발톱함정", "images/PoisonClaw_Trap.bmp", 32 * 1.5, 32 * 1.5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("바람함정", "images/Wind_Trap.bmp", 32 * 1.5, 32 * 1.5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("포션사원", "images/Hp_Shrine.bmp", 32 * 1.5, 32 * 1.5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("방어사원", "images/Def_Shrine.bmp", 32 * 1.5, 32 * 1.5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("포션사원", "images/Exp_Shrine.bmp", 32 * 1.5, 32 * 1.5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("골드가방", "images/Gold.bmp", 32 * 1.5, 32 * 1.5, true, RGB(255, 0, 255));

	m_totalEvent = 0;
	return S_OK;
}

HRESULT eventManager::init(STAGE _stage, int _eventplus)
{
	m_stage = _stage;


	//지정 수 많큼 이벤트를 세팅하고
	for (int i = 0; i < _eventplus; i++)
	{
		events* event = new events;
		event->init();
		event->setEventNumber(m_totalEvent + i);

		//이벤트를 설정해준다
		switch (m_stage)
		{
		case STAGE_NONE:
			break;
		case STAGE_ONE:
			if (i < 2)
			{
				event->setEvent(EVENT_FIRETRAP);
			}
			else if (i == 2)
			{
				event->setEvent(EVENT_VENDER);
			}
			else
			{
				event->setEvent(EVENT_HPSHRINE);
			}
			break;
		case STAGE_TWO:
			if (i < 1)
			{
				event->setEvent(EVENT_FIRETRAP);
			}
			else if (i == 1)
			{
				event->setEvent(EVENT_BITETRAP);
			}
			else
			{
				event->setEvent(EVENT_HPSHRINE);
			}
			break;
		case STAGE_THREE:
			if (i < 1)
			{
				event->setEvent(EVENT_HPSHRINE);
			}
			else
			{
				event->setEvent(EVENT_BITETRAP);
			}
			break;
		case STAGE_FOUR:
			if (i < 1)
			{
				event->setEvent(EVENT_CLAWTRAP);
			}
			else
			{
				event->setEvent(EVENT_DEFSHRINE);
			}
			break;
		case STAGE_FIVE:
			if (i < 1)
			{
				event->setEvent(EVENT_CLAWTRAP);
			}
			else if (i == 1)
			{
				event->setEvent(EVENT_HPSHRINE);
			}
			else if (i == 2)
			{
				event->setEvent(EVENT_DEFSHRINE);
			}
			else if (i == 3)
			{
				event->setEvent(EVENT_HPSHRINE);
			}
			else if (i == 4)
			{
				event->setEvent(EVENT_VENDER);
			}
			break;
		case STAGE_SIX:
			if (i < 2)
			{
				event->setEvent(EVENT_WINDTRAP);
			}
			else if (i == 2)
			{
				event->setEvent(EVENT_HPSHRINE);
			}
			else
			{
				event->setEvent(EVENT_GOLDBAG);
			}
			break;
		case STAGE_SEVEN:
			if (i < 1)
			{
				event->setEvent(EVENT_CLASSCHANGE);
			}
			else if (i == 1)
			{
				event->setEvent(EVENT_POSIONCLAWTRAP);
			}
			else
			{
				event->setEvent(EVENT_VENDER);
			}
			break;
		case STAGE_EIGHT:
			if (i < 1)
			{
				event->setEvent(EVENT_EXPSHRINE);
			}
			else
			{
				event->setEvent(EVENT_VENDER);
			}
			break;
		case STAGE_NINE:
			if (i < 1)
			{
				event->setEvent(EVENT_WINDTRAP);
			}
			else if (i == 1)
			{
				event->setEvent(EVENT_DEFSHRINE);
			}
			else
			{
				event->setEvent(EVENT_EXPSHRINE);
			}
			break;
		case STAGE_TEN:
			if (i < 2)
			{
				event->setEvent(EVENT_WINDTRAP);
			}
			else if (i == 2)
			{
				event->setEvent(EVENT_VENDER);
			}
			else
			{
				event->setEvent(EVENT_POSIONCLAWTRAP);
			}
			break;
		default:
			break;
		}

		m_eventVector.push_back(event);
	}
	m_totalEvent += _eventplus;


	return S_OK;
}

void eventManager::release()
{
	vector<events*>::iterator eIter = m_eventVector.begin();
	for (eIter; eIter != m_eventVector.end();)
	{
		SAFE_DELETE((*eIter));
		eIter = m_eventVector.erase(eIter);
	}
}

void eventManager::update()
{
	for (int i = 0; i < m_eventVector.size(); i++)
	{
		m_eventVector[i]->update();
	}
}

void eventManager::render()
{
	for (int i = 0; i < m_eventVector.size(); i++)
	{
		m_eventVector[i]->render();
	}
}

eventManager::eventManager()
{
}


eventManager::~eventManager()
{
}
