// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.

#include <fstream>

//STL
#include<string>
#include<map>
#include<vector>

//====================================
//	## 내가 만든 헤더파일## 
//====================================

#include "drawShapesFunction.h"
#include "randomFunction.h"
#include "keyManager.h"
#include "imageManager.h"
#include "TimeManager.h"
#include "sceneManager.h"
#include "iniData.h"
#include "player.h"
#include "sound.h"
#include "animationmanager.h"
#include "inventoryManager.h"
#include "uiManager.h"
#include "enemyManager.h"
#include "equipmentManager.h"
#include "itemManager.h"
#include "effectManager.h"
#include "utils.h"

using namespace std;
using namespace UTIL;

//====================================
//	## 싱글톤 추가 ## 
//====================================
#define RND						randomFunction::getSingleton()
#define KEYMANAGER				keyManager::getSingleton()
#define IMAGEMANAGER			imageManager::getSingleton()
#define TIMEMANAGER				timeManager::getSingleton()
#define INIDATA					iniData::getSingleton()
#define SCENEMANAGER			sceneManager::getSingleton()
#define ANIMATIONMANAGER		animationManager::getSingleton()
#define INVENTORYMANAGER		inventoryManager::getSingleton()
#define UIMANAGER				uiManager::getSingleton()
#define ENEMYMANAGER			enemyManager::getSingleton()
#define EQUIPMENTMANAGER		equipmentManager::getSingleton()
#define ITEMMANAGER				itemManager::getSingleton()
#define EFFECTMANAGER			effectManager::getSingleton()

//====================================
//	## 디파인문 ## (윈도우 초기화 셋팅)
//====================================

#define WINNAME (LPTSTR)(TEXT("Api Project Name"))


#define TOTALSIZE 3

//전체 화면 사이즈
#define WINSIZEX 256*TOTALSIZE
#define WINSIZEY 224*TOTALSIZE

#define MAPSTARTVIEWX 0
#define	MAPSTARTVIEWY 0
#define	MAPENDVIEWX 2048
#define	MAPENDVIEWY 1536

#define TILESIZE 24		//타일 사이즈

#define TILEX 64	//기본 맵 타일 사이즈 X
#define TILEY 64	//기본 맵 타일 사이즈 Y

#define TILESIZEX TILEX * TILESIZE
#define TILESIZEY TILEY * TILESIZE


//화면 오른쪽 이미지 타일 갯수
#define SAMPLETILEX 20
#define SAMPLETILEY 10

#define WINSTYLE WS_CAPTION | WS_SYSMENU

//==================================================
//	## 매크로 함수 ## (메인게임의 릴리즈 부분에서 사용) 
//==================================================

#define SAFE_DELETE(p)	{if(p) {delete(p); (p) = nullptr;}}
#define SAFE_RELEASE(p)	{if(p) {(p)->release(); (p) = nullptr;}}

//==================================================
//	## 전역변수 ## (인스턴스, 윈도우 핸들, 마우스좌표) 
//	winmain에 선언되어있습니다
//==================================================
extern HINSTANCE m_hInstance;
extern HWND m_hWnd;
extern POINT m_ptMouse;
extern player _player;
extern sound _sound;
extern bool _isDebug;
extern bool _isSound;


//==================================================
//	## 전역 상태,타입 enum 
//==================================================
enum OBJECT_TYPE { EMPTY_TYPE, PLAYER, ENEMY, NPC, BOOMWALL, WEED };// 사물의 상호작용 시 타입 나누기용
enum DIRECTION_STATE { PLAYER_LEFT, PLAYER_RIGHT, PLAYER_UP, PLAYER_DOWN, NULLDIRECTION };//플레이어방향
enum ANIMATION_STATE { PLAYER_IDLE, PLAYER_MOVE, PLAYER_ATTACK, PLAYER_BOW }; //현재사용안함
enum USED_EQUIPMENT { USED_BOW, USED_BOOMERANG, USED_BOOM }; //플레이어가 현재 착용하고 있는 장비 (장비가 수시로 바뀌기때문에 나눔)
enum EQUIPMENT_KIND { EQUIPMENT_KIND_NULL, EQUIPMENT_KIND_ARROW, EQUIPMENT_KIND_BOOMERANG, EQUIPMENT_KIND_BOOM, EQUIPMENT_KIND_INTERACTIONOBJECT, 
	EQUIPMENT_KIND_AGAHNIMBOLT, EQUIPMENT_KIND_AGAHNIMBUBBLE, EQUIPMENT_KIND_AGAHNIMSMALLBUBBLE, EQUIPMENT_KIND_END };
enum SHOPITEM {
	BOW, BOOMERANG,
	NOLL,
	BOMB,
	NOLL1,
	NOLL2,
	NOLL3,
	NOLL4,
	NOLL5,
	NOLL6,
	HPMAX,
	NOLL7,
	NOLL8,
	NOLL9,
	NOLL10,
	NOLL11,
	NOLL12,
	NOLL13,
	NOLL14,
	NOLL15,
	SHOPARROW10,
	ITEMNULL
	//EMPTY = 19
};

enum ITEM { MONEY, ARROW10, LIFE, HPMAXUP, GREENPENDENT, BLUEPENDENT, REDPENDENT, KEY};
//enum ITEM {	MONEY, ARROW10, LIFE, HPMAX};