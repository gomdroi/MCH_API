#pragma once
#include"singletonBase.h"
#include<bitset>

using namespace std;

#define KEYMAX 256

class keyManager : public singletonBase<keyManager>
{
private:
	bitset<KEYMAX> _keyUP;
	bitset<KEYMAX> _keyDown;

public:
	keyManager();
	~keyManager();

	HRESULT init();
	void release();

	//키를 한번만 누름
	bool isOnceKeyDown(int key);
	//누르다가 입력종료시
	bool isOnceKeyUp(int key);
	//계속 누름
	bool isStayKeyDown(int key);
	//토글
	bool isToggleKey(int key);
};