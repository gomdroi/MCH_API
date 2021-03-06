// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//STL
#include<string>
#include<map>
#include<vector>


//====================================
//	## 내가 만든 헤더파일 이곳에 추가 ## 
//====================================
#include"commonMacroFunction.h"
#include"keyManager.h"
#include"imageManager.h"
#include"randomFunction.h"
#include"utils.h"
#include"tileManager.h"
#include"itemManager.h"
#include"monsterManager.h"
#include"eventManager.h"


using namespace UTIL;
//====================================
//	## 싱글톤 추가 ## 
//====================================

#define RND						randomFunction::getSingleton()
#define KEYMANAGER				keyManager::getSingleton()
#define IMAGEMANAGER			imageManager::getSingleton()	
#define ITEMMANAGER				itemManager::getSingleton()
#define MONSTERMANAGER			monsterManager::getSingleton()
#define TILEMANAGER				tileManager::getSingleton()
#define EVENTMANAGER			eventManager::getSingleton()

//====================================
//	## 디파인문 ## (윈도우 초기화 셋팅) 
//====================================

#define WINNAME (LPTSTR)(TEXT("경일 프로그래밍 14기"))
#define WINSTARTX 100
#define WINSTARTY 100
#define WINSIZEX 1024
#define WINSIZEY 768
#define WINSTYLE WS_CAPTION | WS_SYSMENU

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

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
