#pragma once
#include "gameNode.h"
#include "tileMap.h"
#include "shop.h"
#include "item.h"

struct imageButton
{
	image *_img;
	RECT _rc;
	string _text;
	int _kind;

	bool _isFrame;
	bool _ischeck;

	int _currentX;
};

enum SCENE_NAME
{
	SCENE_NAME_NULL_SCENE,
	SCENE_NAME_LOADDING_SCENE,
	SCENE_NAME_START_SCENE,
	SCENE_NAME_MAPTOOL_SCENE,
	SCENE_NAME_ENDING_SCENE,
	SCENE_NAME_SOUND_SCENE,
	SCENE_NAME_INGAME_SCENE,
	SCENE_NAME_INGAME_SHOP_SCENE,
	SCENE_NAME_SCENE_END
};

class scene: public gameNode
{
protected:
	//이곳에 씬들에 필요한 데이터를 입력합니다. 
	//씬들은 이 클래스를 상속받아 사용합니다

	vector<string> _tileSampleImageNameVector;					//샘플이미지의 이름을 가지고 있는 백터 선언
	vector<string> _objSampleImageNameVector;
	vector<string> _interactionObjectSampleImageNameVector;

	SCENE_NAME _sceneName;

	tileMap _firstFloorMap;
	tileMap _secondFloorMap;

	image *_backmap;

	vector<RECT> _moveRcVector;

	image *_img;

	image *_mapImg;

	//상점쓸때만 이용한다
	shop _shop;

	int _currentMapX;
	int _currentMapY;
	int _nowFloor;

public:
	scene();
	~scene();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();

	virtual tileMap getTileMap();

	void imageButtonRender(imageButton button);
	void imageButtonPoint(imageButton* button);

	void sceneLoading(const char *fileName);
	;
	//샘플이미지의 이미지 이름을 추가한다
	void addSampleImageName();

	//타일맵을 그려준다
	void drawTileMap(bool mapSetAll, int floor, int cameraX, int cameraY);

	//맵을 이동할 수 있는 렉트를 그려줍니다
	void addMoveRc(int x, int y, int width, int height);

	//렉트 충돌을 판단합니다
	int collisionRc(RECT rc);

	void clearMoveRc();

	//현재 층 정보를 얻어 오거나 수정합니다
	void setNowFloor(int floor) { _nowFloor = floor; };
	int getNowFloor() { return _nowFloor; }

	void reDrawPosition(bool isPlayer, int floor, int positionX, int positionY, int width, int height, int cameraX, int cameraY);

};

