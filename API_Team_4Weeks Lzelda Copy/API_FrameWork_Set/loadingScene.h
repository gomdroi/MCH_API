#pragma once
#pragma warning(disable:4996)
#include "scene.h"
#include "loading.h"

class loadingScene:public scene
{
	/*
	로딩화면을 위한 씬입니다
	로딩 이미지는 아래 loading 클래스에 정의되어 있습니다
	로딩을 하려는 이미지는 아래 total loading 함수에 추가합니다
	*/
private:
	loading *_loading;

public:
	loadingScene();
	~loadingScene();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();

	void totalLoding();
};

