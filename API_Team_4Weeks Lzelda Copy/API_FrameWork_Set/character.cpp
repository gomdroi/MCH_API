#include "stdafx.h"
#include "character.h"


character::character()
{
}


character::~character()
{
}

HRESULT character::init(const char * imageName, POINT position)
{
	//프레임 초기화
	_currentFrameX = 0;
	_currentFrameY = 0;
	_isFrameImage = false;

	//이미지, 렉트
	_positionX = position.x;
	_positionY = position.y;
	_img = IMAGEMANAGER->findImage(imageName);
	_Rc = RectMakeCenter(position.x, position.y, _img->getFrameWidth(), _img->getFrameHeight());

	//스피드 각도 초기화
	_angle = 0;
	_speed = 0;
	return S_OK;
}

void character::release()
{

}

void character::update()
{
}

void character::render(HDC hdc, bool isRectRend)
{

}

void character::setImage(string imgName)
{
	_img = IMAGEMANAGER->findImage(imgName);
}
