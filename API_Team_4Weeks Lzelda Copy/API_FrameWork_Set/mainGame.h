#pragma once
#include "gameNode.h"
#include "sceneAll.h"

class mainGame : public gameNode
{
private:
	bool _isKeyExplanation;

public:
	mainGame();
	~mainGame();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};

