#pragma once
#include"image.h"

//백버퍼 이미지를 스태틱으로 변경

//static image* _backBuffer = IMAGEMANAGER->addImage("backbuffer", WINSIZEX, WINSIZEY);
class gameNode
{

private:
	 static image * _backBuffer;		//백버퍼
	
	//void setBackBuffer();		//백버퍼 셋팅

	HDC _hdc;
	bool _managerInit;

public:
	gameNode();
	~gameNode();


	virtual HRESULT init();
	virtual HRESULT init(bool managerInit);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void render();


	static image* setBackBuffer();

	//백버퍼 얻어오기
	image* getBackBuffer() { return _backBuffer; }

	//추가(메모리 DC)
	HDC getMemDC() { return _backBuffer->getMemDC();}

	// 2019.09.26
	// 카메라 좌표 (DC 좌표)
	int getMemDC_X() { return _backBuffer->getX(); }
	int getMemDC_Y() { return _backBuffer->getY(); }
	// 가로 세로 길이 가져오기
	int getMemDC_Width() { return _backBuffer->getWidth(); }
	int getMemDC_Height() { return _backBuffer->getHeight(); }

	//추가 HDC얻기
	HDC getHDC() { return _hdc; }


	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);





};

