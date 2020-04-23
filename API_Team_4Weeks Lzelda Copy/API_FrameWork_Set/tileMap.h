#pragma once

//지형(EX 밟을때 사운드를 나게 하거나 이동할 수 없는 지역 등등)
enum TERRAIN
{
	TR_NONE, TR_CEMENT, TR_DESERT, TR_GRASS, TR_WATER, TR_END
};

//오브젝트(EX 지형위쪽에 배치, 플레이어의 이동을 방해하는 지형)
enum OBJECT
{
	OBJ_NONE,
	OBJ_BLOCK,
	OBJ_DOOR,
	OBJ_BLOCKS,
	OBJ_END
};
//상호작용 오브젝트(타일의 크기와는 다르게 플레이어와 상호작용하고 움직이는 물체)
enum INTERACTIONOBJECT
{
	INTERACTIONOBJ_NONE,
	INTERACTIONOBJ_GRASS,
	INTERACTIONOBJ_SMALLROCK,
	INTERACTIONOBJ_LADDER,
	INTERACTIONOBJ_BIGTREASUREBOX,
	INTERACTIONOBJ_TREASUREBOX,
	INTERACTIONOBJ_JAR,
	INTERACTIONOBJ_ROCKEDDOOR,
	INTERACTIONOBJ_OPENDOOR,
	INTERACTIONOBJ_END
};
//위치 좌표
enum POS
{
	POS_FLAG1,
	POS_FLAG2,
	POS_TANK1,
	POS_TANK2
};

//타일 구조체
struct tagTile
{
	TERRAIN terrain;
	OBJECT obj;
	RECT rc;
	image *terrainImg, *objImg;

	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;

	int terrainImageNum;
	int objImageNum;
};

struct tagInteractionObject
{
	INTERACTIONOBJECT obj;
	image *_img;
	RECT _rc;

	//맵에서의 타일 좌표값
	int xPos;
	int yPos;

	//기본적으로 가지고 있는 타일크기
	int widthPos;
	int heightPos;

	int imageNum;
};

class tileMap
{
	/*
	1개의 타일맵(속한 오브젝트 벡터)을 갖는 클래스입니다
	벡터에 맵 사이즈의 데이터를 저장하고 사용합니다
	*/
private:
	int _mapSizeX;
	int _mapSizeY;

	vector<vector<tagTile>> _map;
	vector<tagInteractionObject> _interactionObject;

public:
	tileMap();
	~tileMap();

	virtual HRESULT init();
	virtual HRESULT init(int sizeX, int sizeY);
	virtual HRESULT initTileMap(int sizeX, int sizeY,int tileSizeX, int tileSizeY, image* setImg);

	virtual void update();
	virtual void render();
	virtual void release ();

	vector<vector<tagTile>> getTileMap() { return _map; };
	void setTileMap(vector<vector<tagTile>> tilemap) {_map=tilemap; };

	vector<tagInteractionObject> getInteractionObject() { return _interactionObject; };
	void setInteractionObject(vector<tagInteractionObject> interactionObject) { _interactionObject = interactionObject; };


	//맵 사이즈 get, set
	int getMapSizeX() { return _mapSizeX; };
	int getMapSizeY() { return _mapSizeY; };

	void setMapSizeX(int sizeX) { _mapSizeX = sizeX; };
	void setMapSizeY(int sizeY) { _mapSizeY = sizeY; };


	//맵 구조체 get,set
	int getMapTerrain(int x, int y) { return _map[y][x].terrain; };
	int getMapObj(int x, int y);
	image* getMapTerrainImage(int x, int y) { return _map[y][x].terrainImg; };
	image* getMapObjImage(int x, int y) { return _map[y][x].objImg; };
	int getTerrainImageNum(int x, int y) { return _map[y][x].terrainImageNum; };
	int getObjImageNum(int x, int y) { return _map[y][x].objImageNum; };
	
	void setMapTerrain(int x, int y, TERRAIN terrain) { _map[y][x].terrain= terrain; };
	void setMapObj(int x, int y, OBJECT object) { _map[y][x].obj= object; };
	void setTerrainFrameX(int x, int y, int frameX) { _map[y][x].terrainFrameX = frameX; };
	void setTerrainFrameY(int x, int y, int frameY) { _map[y][x].terrainFrameY = frameY; };
	void setObjFrameX(int x, int y, int frameX) { _map[y][x].objFrameX = frameX; };
	void setObjFrameY(int x, int y, int frameY) { _map[y][x].objFrameY = frameY; };
	void setMapTerrainImage(int x, int y, image* terrainImage) { _map[y][x].terrainImg = terrainImage; };
	void setMapObjImage(int x, int y, image* objImage) { _map[y][x].objImg = objImage; };
	void setTerrainImageNum(int x, int y, int num) { _map[y][x].terrainImageNum = num; };
	void setObjImageNum(int x, int y, int num) { _map[y][x].objImageNum = num; };

	void addInteractionObject(int x, int y, int widthXPos, int heightYPos, RECT rc,INTERACTIONOBJECT obj, image* img, int imageNum);
	void deleteInteractionObject(int num);
};

