#pragma once
#include"singletonBase.h"
#include "tileMap.h"

class gameNode;

class sceneManager : public singletonBase<sceneManager>
{
private:

	typedef map<string, gameNode*> mSceneList;
	typedef map<string, gameNode*>::iterator miSceneList;

private:
	//현재씬
	static gameNode* _currentScene;
	//씬목록
	mSceneList _mSceneList;

public:
	sceneManager();
	~sceneManager();

	HRESULT init();
	void release();
	void update();
	void render();
	//씬추가하기
	gameNode* addScene(string sceneName, gameNode* scene);

	//씬변경(현재씬으로)
	HRESULT changeScene(string sceneName);

	tileMap getCurrentTileMap();

	//현재 층 정보를 얻어 오거나 수정합니다
	void setNowFloor(int floor);
	int getNowFloor();

	void reDrawPosition(bool isPlayer, int floor, int positionX, int positionY, int width, int height, int cameraX, int cameraY);
};

