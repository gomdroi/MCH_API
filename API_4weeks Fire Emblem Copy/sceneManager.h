#pragma once
#include"singletonBase.h"

class gameNode;

class sceneManager : public singletonBase<sceneManager>
{
private:

	typedef map<string, gameNode*> mSceneList;
	typedef map<string, gameNode*>::iterator miSceneList;

private:
	//ÇöÀç¾À
	static gameNode* _currentScene;
	//¾À¸ñ·Ï
	mSceneList _mSceneList;

public:
	sceneManager();
	~sceneManager();


	HRESULT init();
	void release();
	void update();
	void render();
	//¾ÀÃß°¡ÇÏ±â
	gameNode* addScene(string sceneName, gameNode* scene);

	//¾Àº¯°æ(ÇöÀç¾ÀÀ¸·Î)
	HRESULT changeScene(string sceneName);


};

