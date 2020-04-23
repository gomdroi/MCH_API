#pragma once
#include "scene.h"
#include "tileMap.h"

#define MAPTOOL_WINSIZEX 1200
#define MAPTOOL_WINSIZEY 750

#define TILESIZE 24  //맵 타일 사이즈
#define MAPTOOL_TILESIZE 24  //맵 타일 사이즈
#define MAPTOOL_SIDEBAR_STARTX 700	//맵툴 사이드 바 x시작 위치
#define MAPTOOL_SIDEBAR_STARTY 0	//맵툴 사이드 바 y시작 위치

#define MAPTOOL_VIEWSIZEX 600
#define MAPTOOL_VIEWSIZEY 500

enum MAPTOOL_BUTTONKIND
{
	MAPTOOL_BUTTONKIND_SAVE,
	MAPTOOL_BUTTONKIND_LOAD,
	MAPTOOL_BUTTONKIND_TERRAINDRAW,
	MAPTOOL_BUTTONKIND_OBJDRAW,
	MAPTOOL_BUTTONKIND_INTERACTIONOBJDRAW,
	MAPTOOL_BUTTONKIND_ERASER,
	MAPTOOL_BUTTONKIND_RESIZE,
	MAPTOOL_BUTTONKIND_FIRSTFLOOR,
	MAPTOOL_BUTTONKIND_SECONDFLOOR,
	MAPTOOL_BUTTONKIND_LEFTBUTTON,
	MAPTOOL_BUTTONKIND_RIGHTBUTTON,
	MAPTOOLCTRL_END
};

struct tagSampleTile
{
	image *_img;
	RECT _rcTile;
	int _frameX;
	int _frameY;
};
//현재 타일 구조체
struct tagCurrentTile
{
	image *_img;
	int _x;
	int _y;
};

class mapToolScene :public scene
{
private:
	tileMap _firstFloorTileMap, _secondFloorTileMap;			//맵정보를 받는다
	vector<tagCurrentTile> _currentTileVector;					//현재 선택된 타일의 정보를 저장
	tagInteractionObject _currentInteractionObject;				//현재 선택된 상호작용 오브젝트의 정보 저장
	tagSampleTile _sampleTiles[SAMPLETILEX* SAMPLETILEY];		//오른쪽 샘플타일

	image* _nowImg;												//현재 샘플타일 이미지
	int _nowImgNum;												//샘플타일을 알기위한 변수

	HWND hEdit1, hEdit2, hEdit3;								//에디트버튼 3개
	vector<imageButton> imageButtonVector;						//버튼

	image* _mapBackBuffer;										//맵을 미리 그려놓는 백버퍼

	int _nowFloor;												//현재 층수 저장

	int _currentMapX;											//입력된 맵 x좌표 저장
	int _currentMapY;											//입력된 맵 y좌표 저장

	int _currentMapXPos;										//입력된 맵 사이즈 저장
	int _currentMapYPos;										//입력된 맵 사이즈 저장

	HANDLE file;												//세이브, 로드을 위한 변수
	DWORD write;
	DWORD read;

	int _ctrSelect;												//어떤 버튼을 클릭했는지 저장

	int _nowCarmeraX;											//카메라 x,y좌표
	int _nowCarmeraY;

	int _startMousePointX;										//마우스가 눌려져 있을때 시작 점 저장한다
	int _startMousePointY;
	int _selectTileX, _selectTileY;								//마우스 드래그시 선택된 크기 저장
	RECT _nowSelectSampleTileRect;								//선택된 타일를 나타내는 렉트

	bool _isMinimapView;										//미니맵을 볼지 여부를 판단하는 함수

public:
	mapToolScene();
	~mapToolScene();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void setMap();														//타일 선택 및 행동을 처리해주는 함수

	void save();
	void load();

	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objSelect(int frameX, int frameY);
	
	//backmap의 dc를 반환
	HDC getMapBackBufferDc() { return _mapBackBuffer->getMemDC(); };

	//버튼 추가
	void buttonAdd(string imageName, MAPTOOL_BUTTONKIND buttonKind, int x, int y, int width, int height, bool isFrame);
	//버튼 판단
	void selectButton();

	//맵 사이즈를 변경해 주는 함수
	void reMapSize(bool isLoad);

	//타일 맵을 백맵에 그려주는 함수
	void drawBackTileMap(int set, int floor);

	//다음이나 전 샘플이미지로 넘어간다
	void moveNowSampleImage(bool isleft);

	//현재 이미지넘버에 맞는 오브젝트를 선택된 것에 설정해준다.
	void setNowInteractionObject(int num);
};

