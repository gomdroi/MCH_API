#include "stdafx.h"
#include "actionTest.h"


actionTest::actionTest()
{
}


actionTest::~actionTest()
{
}

HRESULT actionTest::init()
{
	_image = IMAGEMANAGER->addImage("rocket", "images/rocket.bmp", WINSIZEX / 2, WINSIZEY / 2, 52, 64, true, RGB(255, 0, 255));

	_action = new action;
	_action->init();

	//_action->moveTo(_image, RND->getFromIntTo(50, WINSIZEX - 50), RND->getFromIntTo(50, WINSIZEY - 50), RND->getFromIntTo(2, 6));
	//ฤÝน้
	_action->moveTo(_image, RND->getFromIntTo(50, WINSIZEX - 50), RND->getFromIntTo(50, WINSIZEY - 50), RND->getFromIntTo(2, 6), callback);
	testImage = _image;
	testAction = _action;
	return S_OK;
}

void actionTest::update()
{
	_action->update();
}

void actionTest::release()
{
	_action->release();
}

void actionTest::render()
{
	_image->render(getMemDC(), _image->getX(), _image->getY());
}

void actionTest::callback()
{
	testAction->moveTo(testImage, RND->getFromIntTo(50, WINSIZEX - 50), RND->getFromIntTo(50, WINSIZEY - 50), RND->getFromIntTo(2, 6), callback);
}
