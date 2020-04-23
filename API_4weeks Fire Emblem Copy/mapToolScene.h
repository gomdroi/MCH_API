#pragma once
#include "scene.h"


class tile;
class MainMaptool;
class subMaptool;
class mapToolScene :
	public scene
{
private:
	MainMaptool* m_mainMaptool;
	subMaptool* m_subMaptool;
	

public:
	mapToolScene();
	~mapToolScene();

	virtual HRESULT init();
	void mapInit();
	virtual void update();
	virtual void render();
	virtual void release();
};

