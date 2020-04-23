#pragma once
#include "singletonBase.h"

class events;
class eventManager : public singletonBase<eventManager>
{
private:
	vector<events*> m_eventVector;
	STAGE m_stage;
	int m_totalEvent;
public:

	virtual HRESULT init();
	virtual HRESULT init(STAGE _stage, int _eventplus);
	virtual void release();
	virtual void update();
	virtual void render();

	vector<events*> getEventVector() { return m_eventVector; }

	eventManager();
	~eventManager();
};

