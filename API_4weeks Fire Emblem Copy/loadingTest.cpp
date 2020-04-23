#include "stdafx.h"
#include "loadingTest.h"

loadingTest::loadingTest()
{
}


loadingTest::~loadingTest()
{
}

HRESULT loadingTest::init()
{

	_loading = new loading;
	_loading->init();


	this->totalLoding();

	return S_OK;
}

void loadingTest::release()
{
	SAFE_DELETE(_loading);
}
void loadingTest::update()
{
	_loading->update();

	//로딩이 끝나면 체인지
	if (_loading->loadingDone())
	{
		SCENEMANAGER->changeScene("scene1");
	}
}

void loadingTest::render()
{
	_loading->render();
}
void loadingTest::totalLoding()
{
	/*for (int i = 0; i < 1000; i++)
	{
		_loading->loadImage("1", WINSIZEX, WINSIZEY);
	}*/


}
