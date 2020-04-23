#pragma once
#include "gameNode.h"
class objects : public gameNode
{
protected:

	POINTFLOAT m_position;
	RECT m_rc;
	image* m_image;

	int aniDelayTime;
	bool isForwards;
public:
	objects();
	~objects();

	virtual HRESULT init();
	virtual void update();
	virtual void animation();
	virtual void render();
	virtual void release();

	image* getImage() { return m_image; }
	void setImage(image* _image) { m_image = _image; }
	RECT getRc() { return m_rc; }
	void setRc(RECT _rc) { m_rc = _rc; }
	const POINTFLOAT getPosition() { return m_position; }
	void setPosition(POINTFLOAT _position) { m_position = _position; }
};

