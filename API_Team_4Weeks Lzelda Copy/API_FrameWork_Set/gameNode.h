#pragma once
#include"stdafx.h"
#include"image.h"
#include"tileMap.h"
//백버퍼 이미지를 스태틱으로 변경

class gameNode
{
private:
	HDC _hdc;
	static image* _backBuffer;
	bool _managerInit;

public:
	gameNode();
	~gameNode();

	bool _isRectRend;
	bool _start;

	virtual HRESULT init();
	virtual HRESULT init(bool managerInit, bool imageLoad);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual tileMap getTileMap() { return tileMap{}; };
	virtual void setNowFloor(int floor) { return; };
	virtual int getNowFloor() { return 0; };
	virtual void reDrawPosition(bool isPlayer, int floor, int positionX, int positionY, int width, int height, int cameraX, int cameraY) { return; };

	//백버퍼 얻어오기
	image* getBackBuffer() { return _backBuffer; }

	//추가(메모리 DC)
	HDC getMemDC() { return _backBuffer->getMemDC(); }

	//추가 HDC얻기
	HDC getHDC() { return _hdc; }

	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};