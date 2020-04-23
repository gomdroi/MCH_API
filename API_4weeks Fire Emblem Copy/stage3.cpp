#include "stdafx.h"
#include "stage3.h"


stage3::stage3()
{
}


stage3::~stage3()
{
}

HRESULT stage3::init()
{
	/*bgMap = new image;*/
	// 백퍼퍼 (카메라)
	int iCameraX;
	int iCameraY;
	int iCameraWidth;
	int iCameraHeight;
	iCameraX = 0;
	iCameraY = 0;
	iCameraWidth = getMemDC_Width();
	iCameraHeight = getMemDC_Height();

	TILEMANAGER->release();
	UNITMANAGER->release();

	TILEMANAGER->init(STAGE_THREE);
	TILEMANAGER->stageLoad(STAGE_THREE);
	UNITMANAGER->init();
	UIMANAGER->init();
	CAMERA->init(iCameraX, iCameraY, iCameraWidth, iCameraHeight);

	g_cursor.setCurTile(TILEMANAGER->getTileVector()[36]);
	g_cursor.setPosition(PointFloatMake(120, 120));
	g_cursor.setTileMapSizeX(TILEMANAGER->getMapMaxX());
	g_cursor.setTileMapSizeY(TILEMANAGER->getMapMaxY());

	isEnemyTurn = false;
	isPlayerTurn = true;
	isTurning = false;
	return S_OK;
}

void stage3::update()
{

	scene::update();

}

void stage3::render()
{

	IMAGEMANAGER->render("스테이지3", getMemDC(),0 - CAMERA->m_cameraPos.x,0 - CAMERA->m_cameraPos.y);
	scene::render();

}

void stage3::release()
{
	//TILEMANAGER->release();
	//UNITMANAGER->release();
	//UIMANAGER->release();
}
