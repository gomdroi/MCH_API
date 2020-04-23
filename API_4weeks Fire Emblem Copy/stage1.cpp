#include "stdafx.h"
#include "stage1.h"


stage1::stage1()
{
}


stage1::~stage1()
{
}

HRESULT stage1::init()
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
	TILEMANAGER->init(STAGE_ONE);
	TILEMANAGER->stageLoad(STAGE_ONE);
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

void stage1::update()
{	
	scene::update();
}


void stage1::render()
{
	
	IMAGEMANAGER->render("스테이지1",getMemDC());
	scene::render();
	
	
}

void stage1::release()
{
	//TILEMANAGER->release();
	//UNITMANAGER->release();
	//UIMANAGER->release();
}