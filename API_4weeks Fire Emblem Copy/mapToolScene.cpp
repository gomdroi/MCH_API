#include "stdafx.h"
#include "mapToolScene.h"
#include "tile.h"
#include "MainMaptool.h"
#include "subMaptool.h"

mapToolScene::mapToolScene()
{
}


mapToolScene::~mapToolScene()
{
}

HRESULT mapToolScene::init()
{
	/*
	메인 맵툴과  서브 맵툴을 이니트하고 불러와야합니다
	둘다 업데이트 랜더 해야합니다??

	*/
	int iCameraX;
	int iCameraY;
	int iCameraWidth;
	int iCameraHeight;
	iCameraX = getMemDC_X();
	iCameraY = getMemDC_Y();
	iCameraWidth = getMemDC_Width();
	iCameraHeight = getMemDC_Height();

	m_mainMaptool = new MainMaptool;
	m_subMaptool = new subMaptool;
	m_mainMaptool->init();
	SUBWIN->init();
	SUBWIN->SetScene(m_subMaptool);
	CAMERA->init(iCameraX, iCameraY, iCameraWidth, iCameraHeight);
	return S_OK;
}

void mapToolScene::mapInit()
{
	
}

void mapToolScene::update()
{	
	SUBWIN->update();
	m_mainMaptool->update();
}

void mapToolScene::render()
{
	SUBWIN->render();
	m_mainMaptool->render();
}

void mapToolScene::release()
{
}
