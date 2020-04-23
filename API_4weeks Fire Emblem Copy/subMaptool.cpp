#include "stdafx.h"
#include "subMaptool.h"
#include "MainMaptool.h"


subMaptool::subMaptool()
{
}


subMaptool::~subMaptool()
{
}

HRESULT subMaptool::init()
{
	IMAGEMANAGER->addImage("스테이지1패널", "images/maptool/stage1.bmp", 80, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지2패널", "images/maptool/stage2.bmp", 80, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지3패널", "images/maptool/stage3.bmp", 80, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지세이브", "images/maptool/save.bmp", 120, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지로드", "images/maptool/load.bmp", 120, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("지형종류바", "images/maptool/terraintype.bmp", 240, 24, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("유닛종류바", "images/maptool/unittype.bmp", 240, 24, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("지형세트", "images/maptool/terrainset.bmp", 240, 192, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("유닛세트", "images/maptool/unitset.bmp", 240, 192, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("맵툴논", "images/maptool/none.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴엘리우드", "images/maptool/eliwood.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴린", "images/maptool/lyn.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴헥터", "images/maptool/hector.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴레베카", "images/maptool/rebecca.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴세라", "images/maptool/serra.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴플로리나", "images/maptool/florina.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴켄트", "images/maptool/kent.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴브리간드", "images/maptool/brigand.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴솔져", "images/maptool/soldier.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴기병", "images/maptool/cavalier.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴궁수", "images/maptool/archer.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴나이트", "images/maptool/knight.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴와이번", "images/maptool/wyvern.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴워리어", "images/maptool/warrior.bmp", 48, 48, true, RGB(255, 0, 255));

	rc_curSelectedTile = RectMake(0, 0, 240, 48); //1번째렉트줄 0-48
	for (int i = 0; i < STAGEMAX; i++)
	{
		rc_stageSelect[i] = RectMake(0 + (80 * i), 49, 80, 48);
	} //2번째렉트줄 49-97
	rc_stageSave = RectMake(0, 101, 120, 48);
	rc_stageLoad = RectMake(121, 101, 120, 48);
	//3번째렉트줄 101-149
	for (int i = 0; i < TILETYPEMAX; i++)
	{
		rc_tileProperty[i] = RectMake(0 + (TILEX*(i % 5)), 178 + (TILEY*(i / 5)),48,48);
	} //5번째렉트줄 178 - 370
	for (int i = 0; i < UNITTYPEMAX; i++)
	{
		rc_unitProperty[i] = RectMake(0 + (TILEX*(i % 5)), 399 + (TILEY*(i / 5)), 48, 48);
	} //7번째렉트줄 399 - 


	return S_OK;
}

void subMaptool::update()
{
	if (!SUBWIN->GetIsActive()) return;
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		//타일 선택 저장
		for (int i = 0; i < TILETYPEMAX; i++)
		{
			if (PtInRect(&rc_tileProperty[i], SUBWIN->GetMousePos()))
			{
				selectedTerrainType = (TERRAIN_TYPE)i;
				SUBWIN->setTerrainType(selectedTerrainType);
				SUBWIN->setIsTerrainPlace(true); 
				SUBWIN->setIsUnitPlace(false); 
			}
		}
		//유닛 선택 저장
		for (int i = 0; i < UNITTYPEMAX; i++)
		{
			if (PtInRect(&rc_unitProperty[i], SUBWIN->GetMousePos()))
			{
				selectedUnitType = (UNITTYPE)i;
				SUBWIN->setUnitType(selectedUnitType);
				SUBWIN->setIsTerrainPlace(false);
				SUBWIN->setIsUnitPlace(true);
			}
		}
		//스테이지 선택
		for (int i = 0; i < 3; i++)
		{
			if (PtInRect(&rc_stageSelect[i], SUBWIN->GetMousePos()))
			{
				curStage = (STAGE_NUMBER)i;
				SUBWIN->setStageNum(curStage);
				SUBWIN->setStageSelect(true);
			}
		}
		//세이브
		if (PtInRect(&rc_stageSave, SUBWIN->GetMousePos()))
		{
			SUBWIN->setIsSave(true);
		}
		//로드
		if (PtInRect(&rc_stageLoad, SUBWIN->GetMousePos()))
		{
			SUBWIN->setIsLoad(true);
		}

	}
	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		isDebug = !isDebug;
	}

	changeCurSelectedTileStr();
	
}

void subMaptool::render(HDC hdc)
{
	HFONT myFont = CreateFont(25, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "둥근모꼴");
	HFONT oldFont = (HFONT)SelectObject(hdc, myFont);
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);

	DrawText(hdc, curSelectedTileStr, -1, &rc_curSelectedTile, DT_LEFT);

	SelectObject(hdc, oldFont);
	DeleteObject(myFont);
	IMAGEMANAGER->render("스테이지1패널", hdc, rc_stageSelect[0].left, rc_stageSelect[0].top);
	IMAGEMANAGER->render("스테이지2패널", hdc, rc_stageSelect[1].left, rc_stageSelect[1].top);
	IMAGEMANAGER->render("스테이지3패널", hdc, rc_stageSelect[2].left, rc_stageSelect[2].top);
	IMAGEMANAGER->render("스테이지세이브", hdc, rc_stageSave.left, rc_stageSave.top);
	IMAGEMANAGER->render("스테이지로드", hdc, rc_stageLoad.left, rc_stageLoad.top);
	IMAGEMANAGER->render("지형종류바", hdc, 0, 153); //4번째렉트줄 153-177
	IMAGEMANAGER->render("지형세트", hdc, 0, 178); 
	IMAGEMANAGER->render("유닛종류바", hdc, 0, 374); //6번째렉트줄 374-398
	IMAGEMANAGER->render("유닛세트", hdc, 0, 399); 

	/*for (int i = 0; i < TILETYPEMAX; i++)
	{
		RectangleMake(hdc, rc_tileProperty[i]);
	}*/
}

void subMaptool::release()
{
}

void subMaptool::changeCurSelectedTileStr()
{
	if (SUBWIN->getIsTerrainPlace())
	{
		switch (selectedTerrainType)
		{
		case TERRAIN_NONE:
			sprintf_s(curSelectedTileStr, "CurTile : NONE");
			break;
		case TERRAIN_GRASS:
			sprintf_s(curSelectedTileStr, "CurTile : Grass");
			break;
		case TERRAIN_FOREST:
			sprintf_s(curSelectedTileStr, "CurTile : Forest");
			break;
		case TERRAIN_CASTLEWALL:
			sprintf_s(curSelectedTileStr, "CurTile : Castlewall");
			break;
		case TERRAIN_HILL:
			sprintf_s(curSelectedTileStr, "CurTile : Hill");
			break;
		case TERRAIN_MOUNTAIN:
			sprintf_s(curSelectedTileStr, "CurTile : Mountain");
			break;
		case TERRAIN_WATER:
			sprintf_s(curSelectedTileStr, "CurTile : Water");
			break;
		case TERRAIN_CLIFF:
			sprintf_s(curSelectedTileStr, "CurTile : Cliff");
			break;
		case TERRAIN_TOWN:
			sprintf_s(curSelectedTileStr, "CurTile : Town");
			break;
		case TERRAIN_FORT:
			sprintf_s(curSelectedTileStr, "CurTile : Fort");
			break;
		case TERRAIN_SHOP:
			sprintf_s(curSelectedTileStr, "CurTile : Shop");
			break;
		case TERRAIN_FLOOR:
			sprintf_s(curSelectedTileStr, "CurTile : Floor");
			break;
		case TERRAIN_HOUSE:
			sprintf_s(curSelectedTileStr, "CurTile : House");
			break;
		default:
			break;
		}
	}
	else if (SUBWIN->getIsUnitPlace())
	{
		switch (selectedUnitType)
		{
		case TYPE_NONE:
			sprintf_s(curSelectedTileStr, "CurUnit : NONE");
			break;
		case TYPE_ELIWOOD:
			sprintf_s(curSelectedTileStr, "CurUnit : Eliwood");
			break;
		case TYPE_LYN:
			sprintf_s(curSelectedTileStr, "CurUnit : Lyn");
			break;
		case TYPE_HECTOR:
			sprintf_s(curSelectedTileStr, "CurUnit : Hector");
			break;
		case TYPE_GUY:
			sprintf_s(curSelectedTileStr, "CurUnit : NONE");
			break;
		case TYPE_REBECCA:
			sprintf_s(curSelectedTileStr, "CurUnit : Rebecca");
			break;
		case TYPE_SERRA:
			sprintf_s(curSelectedTileStr, "CurUnit : Serra");
			break;
		case TYPE_FLORINA:
			sprintf_s(curSelectedTileStr, "CurUnit : Florina");
			break;
		case TYPE_KENT:
			sprintf_s(curSelectedTileStr, "CurUnit : Kent");
			break;
		case TYPE_BRIGAND:
			sprintf_s(curSelectedTileStr, "CurUnit : Brigand");
			break;
		case TYPE_SOLDIER:
			sprintf_s(curSelectedTileStr, "CurUnit : Soldier");
			break;
		case TYPE_CAVALIER:
			sprintf_s(curSelectedTileStr, "CurUnit : Cavalier");
			break;
		case TYPE_ARCHER:
			sprintf_s(curSelectedTileStr, "CurUnit : Archer");
			break;
		case TYPE_FIGHTER:
			sprintf_s(curSelectedTileStr, "CurUnit : NONE");
			break;
		case TYPE_KNIGHT:
			sprintf_s(curSelectedTileStr, "CurUnit : Knight");
			break;
		case TYPE_PEGASUSKNIGHT:
			sprintf_s(curSelectedTileStr, "CurUnit : NONE");
			break;
		case TYPE_WYVERN:
			sprintf_s(curSelectedTileStr, "CurUnit : Wyvern");
			break;
		case TYPE_WARRIOR:
			sprintf_s(curSelectedTileStr, "CurUnit : Warrior");
			break;
		default:
			break;
		}
	}
	
}
