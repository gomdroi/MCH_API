#include "stdafx.h"
#include "camera.h"


camera::camera()
{

}


camera::~camera()
{

}

HRESULT camera::init(int iCameraX, int iCameraY, int iCameraWidth, int iCameraHeight)
{
	m_cameraPos.x = iCameraX;
	m_cameraPos.y = iCameraY;
	m_cameraWidth = iCameraWidth;
	m_cameraHeight = iCameraHeight;
	rc_camera = RectMakeCenter(m_cameraPos.x + m_cameraWidth / 2, m_cameraPos.y + m_cameraHeight / 2, WINSIZEX, WINSIZEY);

	
	return S_OK;
}

void camera::update()
{
	rc_camera = RectMakeCenter(m_cameraPos.x + m_cameraWidth / 2, m_cameraPos.y + m_cameraHeight / 2, WINSIZEX, WINSIZEY);
}

void camera::render()
{
}

void camera::release()
{
}
