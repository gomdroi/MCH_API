#include "stdafx.h"
#include "camera.h"


camera::camera()
{
}


camera::~camera()
{
}

HRESULT camera::init()
{
	_cameraX = 0;
	_cameraY = 0;
	return S_OK;
}
