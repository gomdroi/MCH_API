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
	IMAGEMANAGER->addImage("��Ʈ��", "images/intro.bmp", 720, 480, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("���θ޴�", "images/mainmenu.bmp", 720, 480, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("������", "images/UI/missA.bmp", 1292, 32, 17, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("������", "images/UI/hitA.bmp", 308, 40, 7, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("����ũ��", "images/UI/critA.bmp", 322, 36, 7, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("�Ʊ���", "images/UI/playerturn.bmp", 534, 60, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("������", "images/UI/enemyturn.bmp", 522, 60, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("��������1", "images/stage1.bmp", 720, 480, true, RGB(255, 0, 255)); //15,10
	IMAGEMANAGER->addImage("��������2", "images/stage2.bmp", 720, 672, true, RGB(255, 0, 255)); //15,14
	IMAGEMANAGER->addImage("��������3", "images/stage3.bmp", 816, 768, true, RGB(255, 0, 255)); //17,16

	IMAGEMANAGER->addImage("����������ũ1_1", "images/story/s1_1.bmp", 720, 480);
	IMAGEMANAGER->addImage("����������ũ1_2", "images/story/s1_2.bmp", 720, 480);
	IMAGEMANAGER->addImage("����������ũ1_3", "images/story/s1_3.bmp", 720, 480);
	IMAGEMANAGER->addImage("����������ũ1_4", "images/story/s1_4.bmp", 720, 480);
	IMAGEMANAGER->addImage("����������ũ2_1", "images/story/s2_1.bmp", 720, 480);
	IMAGEMANAGER->addImage("����������ũ2_2", "images/story/s2_2.bmp", 720, 480);
	IMAGEMANAGER->addImage("����������ũ2_3", "images/story/s2_3.bmp", 720, 480);
	IMAGEMANAGER->addImage("����������ũ3_1", "images/story/s3_1.bmp", 720, 480);
	IMAGEMANAGER->addImage("����������ũ3_2", "images/story/s3_2.bmp", 720, 480);
	IMAGEMANAGER->addImage("����������ũ3_3", "images/story/s3_3.bmp", 720, 480);
	IMAGEMANAGER->addImage("����������ũ3_4", "images/story/s3_4.bmp", 720, 480);
	IMAGEMANAGER->addImage("����1", "images/story/end1.bmp", 720, 480);
	IMAGEMANAGER->addImage("����2", "images/story/end2.bmp", 720, 480);
	return S_OK;
}

void imageManager::release()
{
	deleteAll();
}

image * imageManager::addImage(string strKey, int width, int height)
{

	//�߰��Ϸ��� Ű������ �̹����� �����ϴ��� Ȯ��
	image* img = findImage(strKey);

	//�߰��Ϸ��� �̹����� �̹� ������  ����
	if (img) return img;

	img = new image;
	//�̹����� �ʱ�ȭ ���� ������ 
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
	//������ �̹����� ������ ���� �̹��� ��Ͽ� �߰��Ѵ�.
	_mImageList.insert(make_pair(strKey, img));

	return img;

}

image * imageManager::addFrameImage(string strKey, const char * fileName, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor)
{
	//�߰� �Ϸ��� Ű������ �̹����� �����ϴ��� Ȯ��
	image* img = findImage(strKey);

	//�߰��Ϸ��� �̹����� �̹� ������ ����
	if (img) return img;

	img = new image;
	//�̹����� ����� �ʱ�ȭ ���� ������
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
	//�ش� Ű �˻�
	mapImageListIter key = _mImageList.find(strKey);

	//Ű�� ã����
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

	//����ü�� ���鼭 ����
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
	//�̹����� ã�� �� ����
	image* img = findImage(strKey);
	if (img) img->render(hdc);
}

void imageManager::render(string strKey, HDC hdc, int destX, int destY)
{
	//�̹����� ã�� �� ����
	image* img = findImage(strKey);
	if (img) img->render(hdc, destX, destY);
}

void imageManager::render(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	//�̹����� ã�� �� ����
	image* img = findImage(strKey);
	if (img) img->render(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight);
}

void imageManager::frameRender(string strKey,HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	//�̹����� ã�� �� ����
	image* img = findImage(strKey);

	if (img)img->frameRender(hdc, destX, destY, currentFrameX, currentFrameY);
}
