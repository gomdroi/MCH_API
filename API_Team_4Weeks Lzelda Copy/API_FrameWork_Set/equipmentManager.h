#pragma once
#include "singletonBase.h"
#include "playerInteractionObject.h"
#include "agahnim_bolt.h"
#include "agahnim_bubble.h"
#include "agahnim_smallbubble.h"

class equipment;
class equipmentManager : public singletonBase<equipmentManager>
{
private:
	typedef vector<equipment*> vEquipment;
	vEquipment _equipVector;

	int _startTime;
	bool _isBoom; // 폭발여부 체크

	bool _isInteractionObject;		//상호작용 오브젝트를 가지고 있는지 여부
	bool _isFireInteractionObject;		//상호작용 오브젝트를 발사하는지 여부


public:
	void addEquipment(equipment* equipment) { _equipVector.push_back(equipment); }
	vEquipment getEquipmentVector() { return _equipVector; }
public:
	virtual HRESULT init();
	virtual void render(HDC hdc);
	virtual void release();
	virtual void update();
	void FireEquipment(EQUIPMENT_KIND kind, POINT position, float angle, float speed, bool isPlayer);
	equipmentManager();
	virtual ~equipmentManager();


	bool getIsBoom() { return _isBoom; }
	bool getIsInteractionObject() { return _isInteractionObject; };
	bool getIsFireInteractionObject() {return _isFireInteractionObject;};
	void setIsInteractionObject(bool isInteractionObject) { _isInteractionObject = isInteractionObject; };
	void setIsFireInteractionObject(bool isFireInteractionObject) { _isFireInteractionObject = isFireInteractionObject; };
	void setIsInteractionObject(bool isInteractionObject, POINT position, INTERACTIONOBJECT obj);
	void setIsFireInteractionObject(bool isFireInteractionObject, float angle);

	void isActiveOff();
};

