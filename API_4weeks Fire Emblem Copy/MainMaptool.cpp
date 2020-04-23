#include "stdafx.h"
#include "MainMaptool.h"
#include "tile.h"

MainMaptool::MainMaptool()
{
}


MainMaptool::~MainMaptool()
{
}

HRESULT MainMaptool::init()
{
	mapMaxX = 15;
	mapMaxY = 10;
	mapSize = mapMaxX * mapMaxY;
	TILEMANAGER->init();
	maptoolVector = TILEMANAGER->getTileVector();

	return S_OK;
}

void MainMaptool::init(STAGE_NUMBER _stagenumber)
{
	switch (_stagenumber)
	{
	case STAGE_ONE:
		mapMaxX = 15;
		mapMaxY = 10;
		mapSize = mapMaxX * mapMaxY;
		TILEMANAGER->init(_stagenumber);
		maptoolVector = TILEMANAGER->getTileVector();

		break;
	case STAGE_TWO:
		mapMaxX = 15;
		mapMaxY = 14;
		mapSize = mapMaxX * mapMaxY;
		TILEMANAGER->init(_stagenumber);
		maptoolVector = TILEMANAGER->getTileVector();

		break;
	case STAGE_THREE:
		mapMaxX = 17;
		mapMaxY = 16;
		mapSize = mapMaxX * mapMaxY;
		TILEMANAGER->init(_stagenumber);
		maptoolVector = TILEMANAGER->getTileVector();

		break;
	default:
		break;
	}
}

void MainMaptool::update()
{
	//if (SUBWIN->GetIsActive())	return;

	control();
	SetMptmouse.x = m_ptMouse.x + CAMERA->m_cameraPos.x;
	SetMptmouse.y = m_ptMouse.y + CAMERA->m_cameraPos.y;

	//스테이지 셀렉트
	if (SUBWIN->getStageSelect())
	{
		if (SUBWIN->getStageNum() == STAGE_NUMBER::STAGE_ONE)
		{
			stageBack = IMAGEMANAGER->findImage("스테이지1");
			init(STAGE_ONE);
			SUBWIN->setStageSelect(false);
		}
		else if (SUBWIN->getStageNum() == STAGE_NUMBER::STAGE_TWO)
		{
			stageBack = IMAGEMANAGER->findImage("스테이지2");
			init(STAGE_TWO);
			SUBWIN->setStageSelect(false);
		}
		else if (SUBWIN->getStageNum() == STAGE_NUMBER::STAGE_THREE)
		{
			stageBack = IMAGEMANAGER->findImage("스테이지3");
			init(STAGE_THREE);
			SUBWIN->setStageSelect(false);
		}
	}

	if (SUBWIN->getIsSave())
	{
		save();
		SUBWIN->setIsSave(false);
	}

	if (SUBWIN->getIsLoad())
	{
		load();
		SUBWIN->setIsLoad(false);
	}
	
}

void MainMaptool::render()
{
	if (stageBack != nullptr)
	{
		stageBack->render(getMemDC(), 0 - CAMERA->m_cameraPos.x, 0 - CAMERA->m_cameraPos.y);
	}
	TILEMANAGER->render();
}

void MainMaptool::release()
{
}

void MainMaptool::control()
{
	/*
	(마우스.x + 카메라.x) / 1개의 타일 사이즈 x -> x좌표(타일맵)
	(마우스.y + 카메라.y) / 1개의 타일 사이즈 y -> y좌표

	해당 타일의 속성에 접근 -> 수정
	*/
	if (SUBWIN->getIsTerrainPlace())
	{
		if (mainMaptoolLbuttonDown)
		{
			for (int i = 0; i < mapSize; i++)
			{
				if (PtInRect(&maptoolVector[i]->getRc(), SetMptmouse))
				{
					maptoolVector[i]->setTile(SUBWIN->getTerrainType());
				}
			}
		}
	}
	
	if (SUBWIN->getIsUnitPlace())
	{
		if (mainMaptoolLbuttonDown)
		{
			for (int i = 0; i < mapSize; i++)
			{
				if (PtInRect(&maptoolVector[i]->getRc(), SetMptmouse))
				{
					//maptoolVector[i]->setImage(SUBWIN->getSelectedUnitImage());
					maptoolVector[i]->setSettingUnitType(SUBWIN->getUnitType());
				}
			}
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		isDebug = !isDebug;
	}

	//카메라 테스트
	if (KEYMANAGER->isStayKeyDown('W'))
	{
		if (CAMERA->m_cameraPos.y > 0)
		{
			CAMERA->m_cameraPos.y -= 4;
		}
		else
		{
			CAMERA->m_cameraPos.y = 0;
		}
	}

	if (KEYMANAGER->isStayKeyDown('S'))
	{
		if (CAMERA->m_cameraPos.y + WINSIZEY < mapMaxY * TILEY)
		{
			CAMERA->m_cameraPos.y += 4;
		}
		else
		{
			if (mapMaxY * TILEY < WINSIZEY)
			{
				CAMERA->m_cameraPos.y = 0;
			}
			else
			{
				CAMERA->m_cameraPos.y = mapMaxY * TILEY - WINSIZEY;
			}
		}
	}

	if (KEYMANAGER->isStayKeyDown('D'))
	{
		if (CAMERA->m_cameraPos.x + WINSIZEX < mapMaxX * TILEX)
		{
			CAMERA->m_cameraPos.x += 4;
		}
		else
		{
			if (mapMaxX * TILEX < WINSIZEX)
			{
				CAMERA->m_cameraPos.x = 0;
			}
			else
			{
				CAMERA->m_cameraPos.x = mapMaxX * TILEX - WINSIZEX;
			}
		}
	}

	if (KEYMANAGER->isStayKeyDown('A'))
	{
		if (CAMERA->m_cameraPos.x > 0)
		{
			CAMERA->m_cameraPos.x -= 4;
		}
		else
		{
			CAMERA->m_cameraPos.x = 0;
		}
	}
}

void MainMaptool::save()
{
	char str[1000];							

	switch (SUBWIN->getStageNum())
	{
	case STAGE_ONE:
		file = CreateFile("maps/stage1.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case STAGE_TWO:
		file = CreateFile("maps/stage2.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case STAGE_THREE:
		file = CreateFile("maps/stage3.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	}

	ZeroMemory(str, sizeof(str));
	sprintf_s(str, "x : %d, y : %d\r\n", mapMaxX, mapMaxY);
	WriteFile(file, str, strlen(str), &write, NULL);

	for (int i = 0; i < mapSize; i++)
	{
			ZeroMemory(str, sizeof(str));
			sprintf_s(str, "%d,%d\r\n",
				maptoolVector[i]->getTerrainType(), maptoolVector[i]->getSettingUnitType());

			WriteFile(file, str, strlen(str), &write, NULL);
	}

	CloseHandle(file);
}

void MainMaptool::load()
{
	char str[1000];
	char editTemp[256] = { 0 };
	switch (SUBWIN->getStageNum())
	{
	case STAGE_ONE:
		sprintf_s(editTemp, "maps/stage1.map");
		break;
	case STAGE_TWO:
		sprintf_s(editTemp, "maps/stage2.map");
		break;
	case STAGE_THREE:
		sprintf_s(editTemp, "maps/stage3.map");
		break;
	}

	ifstream inFile(editTemp);

	ZeroMemory(str, sizeof(str));
	inFile.getline(str, 1000);
	sscanf_s(str, "x : %d, y : %d\r\n", &mapMaxX, &mapMaxY);

	for (int i = 0; i < mapSize; i++)
	{		
		ZeroMemory(str, sizeof(str));
		inFile.getline(str, 1000);

		sscanf_s(str, "%d,%d\r\n",&savedType,&savedUnit);
		maptoolVector[i]->setTile(savedType);
		maptoolVector[i]->setSettingUnitType(savedUnit);
	}

	inFile.close();
}