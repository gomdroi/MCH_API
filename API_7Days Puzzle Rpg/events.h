#pragma once
#include "objects.h"
enum EVENTTYPE
{
	EVENT_NONE,
	EVENT_FIRETRAP,
	EVENT_BITETRAP,
	EVENT_CLAWTRAP,
	EVENT_POSIONCLAWTRAP,
	EVENT_WINDTRAP,
	EVENT_HPSHRINE,
	EVENT_DEFSHRINE,
	EVENT_EXPSHRINE,
	EVENT_GOLDBAG,
	EVENT_VENDER,
	EVENT_CLASSCHANGE
};
class events :
	public objects
{
private:
	char eventName[128];

	float m_eventMaxHp;
	float m_eventCurHp;
	float m_eventAtk;
	float m_eventDef;
	float m_eventExp;
	float m_eventGold;
	float m_eventPower;

	char eventExplain[256];
	char eventHp[128];
	char eventAtk[128];
	char eventDef[128];
	char eventExpGold[128];

	/*POINT* m_stickedTilePosition;*/
	int m_eventNumber;
	EVENTTYPE eventType;

	bool isMapPlaced;

public:

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void animation();
	virtual void setEvent(EVENTTYPE _eventtype);

	int getEventNumber() { return m_eventNumber; }
	void setEventNumber(int _eventnumber) { m_eventNumber = _eventnumber; }
	bool getIsMapPlaced() { return isMapPlaced; }
	void setIsMapPlaced(bool _isplaced) { isMapPlaced = _isplaced; }
	
	float getEventMaxHp() { return m_eventMaxHp; }
	void setEventMaxHp(float _eventmaxhp) { m_eventMaxHp = _eventmaxhp; }
	float getEventCurHp() { return m_eventCurHp; }
	void setEventCurHp(float _eventcurhp) { m_eventCurHp = _eventcurhp; }
	float getEventAtk() { return m_eventAtk; }
	void setEventAtk(float _eventatk) { m_eventAtk = _eventatk; }
	float getEventDef() { return m_eventDef; }
	void setEventDef(float _eventdef) { m_eventDef = _eventdef; }
	float getEventExp() { return m_eventExp; }
	void setEventExp(float _eventexp) { m_eventExp = _eventexp; }
	float getEventGold() { return m_eventGold; }
	void setEventGold(float _eventgold) { m_eventGold = _eventgold; }
	float getEventPower() { return m_eventPower; }
	void setEventPower(float _eventpower) { m_eventPower = _eventpower; }
	EVENTTYPE getEventType() { return eventType; }
	events();
	~events();
};

