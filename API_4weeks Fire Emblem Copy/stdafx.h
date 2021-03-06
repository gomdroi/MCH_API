// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

#include <fstream>
#include<iostream>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include<commdlg.h> //오빨넴용(OpenFileName)

//STL
#include<string>
#include<map>
#include<vector>
#include<stack>
#include<algorithm>


//====================================
//	## 내가 만든 헤더파일 이곳에 추가 ## 
//====================================
#include"commonMacroFunction.h"
#include"keyManager.h"
#include"imageManager.h"
#include"timeManager.h"
#include"sceneManager.h"
#include"soundManager.h"
#include"animationManager.h"
#include"effectManager.h"
#include"randomFunction.h"
#include"utils.h"
#include"txtData.h"
#include"iniData.h"

#include"camera.h"
#include"cursor.h"
#include"itemManager.h"
#include"unitManager.h"
#include"tileManager.h"
#include"uiManager.h"
#include"customAstar.h"
#include"subWin.h"

using namespace UTIL;
using namespace std;
//====================================
//	## 싱글톤 추가 ## 
//====================================

#define RND						randomFunction::getSingleton()
#define KEYMANAGER				keyManager::getSingleton()
#define IMAGEMANAGER			imageManager::getSingleton()
#define TIMEMANAGER             timeManager::getSingleton()
#define SCENEMANAGER			sceneManager::getSingleton()
#define SOUNDMANAGER			soundManager::getSingleton()
#define ANIMATIONMANAGER		animationManager::getSingleton()
#define EFFECTMANAGER			effectManager::getSingleton()
#define TXTDATA					txtData::getSingleton()
#define INIDATA					iniData::getSingleton()

#define ITEMMANAGER				itemManager::getSingleton()
#define UNITMANAGER				unitManager::getSingleton()
#define TILEMANAGER				tileManager::getSingleton()
#define UIMANAGER				uiManager::getSingleton()
#define ASTAR					customAstar::getSingleton()	
#define CAMERA					camera::getSingleton()

#define SUBWIN					subWin::getSingleton()
//====================================
//	## 디파인문 ## (윈도우 초기화 셋팅) 
//====================================

#define WINNAME (LPTSTR)(TEXT("경일 프로그래밍 14기"))
#define WINSTARTX 100
#define WINSTARTY 100
#define WINSIZEX 720	//1024 //720
#define WINSIZEY 480	//768  //480
#define WINSTYLE WS_CAPTION | WS_SYSMENU

//타일 사이즈
#define TILEX 48
#define TILEY 48

#define TILETYPEMAX 12
#define UNITTYPEMAX 18
#define STAGEMAX 3
//==================================================
//	## 매크로 함수 ## (메인게임의 릴리즈 부분에서 사용) 
//==================================================

#define SAFE_DELETE(p)	{if(p) {delete(p); (p) = nullptr;}}
#define SAFE_RELEASE(p)	{if(p) {(p)->release(); (p) = nullptr;}}

//==================================================
//	## 전역변수 ## (인스턴스, 윈도우 핸들, 마우스좌표) 
//==================================================
extern HINSTANCE m_hInstance;
extern HWND m_hWnd;
extern POINT m_ptMouse;
extern bool mainMaptoolLbuttonDown;
extern cursor g_cursor;
extern bool isDebug;

//커맨드UI
extern bool isCommandStart;
extern bool isBattleStart;
//턴
extern bool isPlayerTurn;
extern bool isEnemyTurn;
extern bool isTurning;

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
enum UNITTYPE
{
	TYPE_NONE,
	TYPE_ELIWOOD,
	TYPE_LYN,
	TYPE_HECTOR,
	TYPE_GUY,
	TYPE_REBECCA,
	TYPE_SERRA,
	TYPE_FLORINA,
	TYPE_KENT,
	TYPE_BRIGAND,
	TYPE_SOLDIER,
	TYPE_CAVALIER,
	TYPE_ARCHER,
	TYPE_FIGHTER,
	TYPE_KNIGHT,
	TYPE_PEGASUSKNIGHT,
	TYPE_WYVERN,
	TYPE_WARRIOR
};

enum UNITDEF
{
	DEF_NONE,
	DEF_ALLY,
	DEF_ENEMY
};
enum ITEM_TYPE
{
	ITEMTYPE_NONE,
	ITEM_CONSUMABLE,
	ITEM_WEAPON
};
enum CONSUMABLE_KIND
{
	CONSUMABLE_NONE,
	VULNERARY
};
enum WEAPON_GRADE
{
	WEAPON_GRADE_NONE,
	WEAPON_GRADE_E,
	WEAPON_GRADE_D,
	WEAPON_GRADE_C,
	WEAPON_GRADE_B,
	WEAPON_GRADE_A
};

enum WEAPON_KIND
{
	WEAPONKIND_NONE,
	IRON_SWORD,
	STEEL_SWORD,
	SACRED_SWORD,
	IRON_AXE,
	STEEL_AXE,
	THROWING_AXE,
	IRON_SPEAR,
	STEEL_SPEAR,
	JAVELIN_SPEAR,
	IRON_BOW,
	STEEL_BOW
	
};

enum WEAPON_TYPE
{
	WEAPONTYPE_NONE,
	SWORD,
	AXE,
	SPEAR,
	BOW
};

enum UNIT_DIRECTION
{
	UNIT_IDLE,
	UNIT_UP,
	UNIT_DOWN,
	UNIT_LEFT,
	UNIT_RIGHT,
	UNIT_TARGETED
};

enum TERRAIN_TYPE
{
	TERRAIN_NONE=-1,
	TERRAIN_GRASS,
	TERRAIN_FOREST,
	TERRAIN_CASTLEWALL,
	TERRAIN_HILL,
	TERRAIN_MOUNTAIN,
	TERRAIN_WATER,
	TERRAIN_CLIFF,
	TERRAIN_TOWN,
	TERRAIN_FORT,
	TERRAIN_SHOP,
	TERRAIN_FLOOR,
	TERRAIN_HOUSE
};

enum TILE_TYPE
{
	TILE_START,
	TILE_END,
	TILE_MOVABLE,
	TILE_BLOCKED
};

enum COMMAND_TYPE
{
	COMMAND_ATK,
	COMMAND_TRADE,
	COMMAND_VENDOR,
	COMMAND_ITEM,
	COMMAND_WAIT
};

enum COMMAND_STATE
{
	COMMAND_MOVECONFIRM,
	COMMAND_ATKITEMCONFIRM
};

enum STAGE_NUMBER
{
	STAGE_ONE,
	STAGE_TWO,
	STAGE_THREE
};

enum UNIT_STATE
{

};

//구조체 부분
struct commandMenu
{
	RECT rc;
	image* img;
	POINTFLOAT position;
	COMMAND_TYPE commandType;
};