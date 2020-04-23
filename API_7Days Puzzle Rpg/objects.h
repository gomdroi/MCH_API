#pragma once
#include "gameNode.h"
class objects : public gameNode
{
protected:
	image* m_image;
	POINT m_position;
	RECT m_collider;

private:

public:

	image* getImage() { return m_image; }
	void setImage(image* _image) { m_image = _image; }

	POINT getPosition() { return m_position; }
	void setPosition(POINT _position) { m_position = _position; }

	RECT getCollider() { return m_collider; }
	void setCollider(RECT _collider) { m_collider = _collider; }

	objects();
	~objects();
};

