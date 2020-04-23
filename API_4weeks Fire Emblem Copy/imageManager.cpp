#include "stdafx.h"
#include "imageManager.h"


imageManager::imageManager()
{
}


imageManager::~imageManager()
{
}

HRESULT imageManager::init()
{
	IMAGEMANAGER->addImage("인트로", "images/intro.bmp", 720, 480, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("메인메뉴", "images/mainmenu.bmp", 720, 480, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("빗나감", "images/UI/missA.bmp", 1292, 32, 17, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("명중함", "images/UI/hitA.bmp", 308, 40, 7, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("명중크리", "images/UI/critA.bmp", 322, 36, 7, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("아군턴", "images/UI/playerturn.bmp", 534, 60, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("적군턴", "images/UI/enemyturn.bmp", 522, 60, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지1", "images/stage1.bmp", 720, 480, true, RGB(255, 0, 255)); //15,10
	IMAGEMANAGER->addImage("스테이지2", "images/stage2.bmp", 720, 672, true, RGB(255, 0, 255)); //15,14
	IMAGEMANAGER->addImage("스테이지3", "images/stage3.bmp", 816, 768, true, RGB(255, 0, 255)); //17,16

	IMAGEMANAGER->addImage("스테이지토크1_1", "images/story/s1_1.bmp", 720, 480);
	IMAGEMANAGER->addImage("스테이지토크1_2", "images/story/s1_2.bmp", 720, 480);
	IMAGEMANAGER->addImage("스테이지토크1_3", "images/story/s1_3.bmp", 720, 480);
	IMAGEMANAGER->addImage("스테이지토크1_4", "images/story/s1_4.bmp", 720, 480);
	IMAGEMANAGER->addImage("스테이지토크2_1", "images/story/s2_1.bmp", 720, 480);
	IMAGEMANAGER->addImage("스테이지토크2_2", "images/story/s2_2.bmp", 720, 480);
	IMAGEMANAGER->addImage("스테이지토크2_3", "images/story/s2_3.bmp", 720, 480);
	IMAGEMANAGER->addImage("스테이지토크3_1", "images/story/s3_1.bmp", 720, 480);
	IMAGEMANAGER->addImage("스테이지토크3_2", "images/story/s3_2.bmp", 720, 480);
	IMAGEMANAGER->addImage("스테이지토크3_3", "images/story/s3_3.bmp", 720, 480);
	IMAGEMANAGER->addImage("스테이지토크3_4", "images/story/s3_4.bmp", 720, 480);
	IMAGEMANAGER->addImage("엔딩1", "images/story/end1.bmp", 720, 480);
	IMAGEMANAGER->addImage("엔딩2", "images/story/end2.bmp", 720, 480);
	return S_OK;
}

void imageManager::release()
{
	deleteAll();
}

image * imageManager::addImage(string strKey, int width, int height)
{

	//추가하려는 키값으로 이미지가 존재하는지 확인
	image* img = findImage(strKey);

	//추가하려는 이미지가 이미 있으면  리턴
	if (img) return img;

	img = new image;
	//이미지가 초기화 되지 않으면 
	if (FAILED(img->init(width, height)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image * imageManager::addImage(string strKey, const char * fileName, int width, int height, bool trans, COLORREF transColor)
{
	image* img = findImage(strKey);

	if (img) return img;

	img = new image;

	if (FAILED(img->init(fileName, width, height, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image * imageManager::addImage(string strKey, const char * fileName, float x, float y, int width, int height, bool trans, COLORREF transColor)
{
	image* img = findImage(strKey);

	if (img) return img;

	img = new image;

	if (FAILED(img->init(fileName, x, y, width, height, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}


	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image * imageManager::addFrameImage(string strKey, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor)
{
	image* img = findImage(strKey);

	if (img) return img;

	img = new image;

	if (FAILED(img->init(fileName, x, y, width, height, frameX, frameY, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}
	//생성된 이미지를 맵으로 만든 이미지 목록에 추가한다.
	_mImageList.insert(make_pair(strKey, img));

	return img;

}

image * imageManager::addFrameImage(string strKey, const char * fileName, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor)
{
	//추가 하려는 키값으로 이미지가 존재하는지 확인
	image* img = findImage(strKey);

	//추가하려는 이미지가 이미 있으면 리턴
	if (img) return img;

	img = new image;
	//이미지가 제대로 초기화 되지 않으면
	if (FAILED(img->init(fileName, width, height, frameX, frameY, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image * imageManager::findImage(string strKey)
{
	mapImageListIter key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		return key->second;
	}

	return NULL;
}

bool imageManager::deleteImage(string strKey)
{
	//해당 키 검색
	mapImageListIter key = _mImageList.find(strKey);

	//키를 찾으면
	if (key != _mImageList.end())
	{
		key->second->release();
		SAFE_DELETE(key->second);
		_mImageList.erase(key);

		return true;
	}

	return false;
}

bool imageManager::deleteAll()
{

	//맵전체를 돌면서 삭제
	mapImageListIter iter = _mImageList.begin();

	for (iter; iter != _mImageList.end();)
	{
		if (iter->second != NULL)
		{
			SAFE_DELETE(iter->second);
			iter = _mImageList.erase(iter);
		}
		else ++iter;
	}

	_mImageList.clear();

	return true;
}

void imageManager::render(string strKey, HDC hdc)
{
	//이미지를 찾은 후 렌더
	image* img = findImage(strKey);
	if (img) img->render(hdc);
}

void imageManager::render(string strKey, HDC hdc, int destX, int destY)
{
	//이미지를 찾은 후 렌더
	image* img = findImage(strKey);
	if (img) img->render(hdc, destX, destY);
}

void imageManager::render(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	//이미지를 찾은 후 렌더
	image* img = findImage(strKey);
	if (img) img->render(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight);
}

void imageManager::frameRender(string strKey,HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	//이미지를 찾은 후 렌더
	image* img = findImage(strKey);

	if (img)img->frameRender(hdc, destX, destY, currentFrameX, currentFrameY);
}
