#include "stdafx.h"
#include "stage2.h"


stage2::stage2()
{
}


stage2::~stage2()
{
}

HRESULT stage2::init()
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

	g_cursor.init();
	TILEMANAGER->init(STAGE_TWO);
	TILEMANAGER->stageLoad(STAGE_TWO);
	UNITMANAGER->init();
	UIMANAGER->init();
	CAMERA->init(iCameraX, iCameraY, iCameraWidth, iCameraHeight);

	g_cursor.setCurTile(TILEMANAGER->getTileVector()[32]);
	g_cursor.setPosition(PointFloatMake(120, 120));
	g_cursor.setTileMapSizeX(TILEMANAGER->getMapMaxX());
	g_cursor.setTileMapSizeY(TILEMANAGER->getMapMaxY());


	isEnemyTurn = false;
	isPlayerTurn = true;
	isTurning = false;
	return S_OK;
}

void stage2::update()
{

	scene::update();

}

void stage2::render()
{

	IMAGEMANAGER->render("스테이지2", getMemDC(),0 - CAMERA->m_cameraPos.x,0 - CAMERA->m_cameraPos.y);
	scene::render();

}

void stage2::release()
{
	//TILEMANAGER->release();
	//UNITMANAGER->release();
	//UIMANAGER->release();
}
