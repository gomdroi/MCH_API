#pragma once
#include "character.h"
#include "camera.h"
#include "Arrow.h"
#include "Boom.h"
#include "Boomerang.h"
#include "tileMap.h"

class player : public character
{
private:
	camera *_camera;				//플레이어를 따라다니는 카메라
	animation *_ani;				//플레이어 애니메이션
	equipment* _equipment;			//장비

	arrow * _arrow;
	boom * _boom;
	boomerang * _boomerang;

	bool _isLadder;					//사다리에 있는 상태변수 입니다.
	bool _isLadderDirectionUp;

	bool _isHpLow;					//hp가 부족함을 나타내는 변수입니다
	bool _isLift;
	int _knockbackStartTime;		//넉백시작시간을 나타냅니다.			

public:
	player();
	~player();

	HRESULT init(const char* imageName, POINT position);
	void release();
	void update(int mapSizeX, int mapSizeY, tileMap *nowTileMap);
	void render(HDC hdc, bool isRectRend);

	void playerAnimAdd();
	void playerAnimCheck();
	void playerAttack();
	void playerMove(int mapSizeX, int mapSiseY, tileMap nowTileMap);
	void playerAnimMove();
	void playerEquipment();
	void playerInteactionObject(tileMap *nowTileMap);
	void Collision();

	//void playerHit();
	bool obstacle(POINT movePosition, int direction, int mapSizeX, int mapSizeY, tileMap nowTileMap);

	void ending();

public:

	int getCameraX() { return _camera->getCameraX(); };
	void setCameraX(int cameraX) { _camera->setCameraX(cameraX); };

	int getCameraY() { return _camera->getCameraY(); };
	void setCameraY(int cameraY) { _camera->setCameraY(cameraY); };

	//플레이어의 실제 이동을 처리한다
	void setPlayerMove(int mapStartX, int mapStartY, int mapSizeX, int mapSizeY, float moveX, float moveY);
	//씬 이동시 카메라와 플레이어 좌표를 조정한다
	void setPlayerSceneMove(int x, int y, int mapWidth, int mapHeight);

	bool getIsLadder() { return _isLadder; };
	void setIsLadder(bool isLadder) { _isLadder = isLadder; };

	void aniStop() { if(_ani->isPlay()) _ani->stop(); };
	void setIsLift(bool isLift) { _isLift = isLift; };

};

