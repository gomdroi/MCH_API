#pragma once
#include "singletonBase.h"
class camera : public singletonBase<camera>
{
public:

	POINT m_cameraPos;
	RECT rc_camera;
	POINT m_preCameraPos;

	int m_cameraWidth;
	int m_cameraHeight;


public:
	camera();
	~camera();

	virtual HRESULT init(int iCameraX, int iCameraY, int iCameraWidth, int iCameraHeight);
	virtual void update();
	virtual void render();
	virtual void release();

	POINT getCameraPos() { return m_cameraPos; }
	void setCameraPos(POINT _camerapos) { m_cameraPos = _camerapos; }

	RECT getCameraRect() { return rc_camera; }
	void setCameraRect(RECT _camerarect) { rc_camera = _camerarect; }


};

