#pragma once
#include"gameNode.h"
// 에너미를 이녀석을 상속받아 보스나 잡몸을 만들어도 됨.
class enemy :  public gameNode
{
private : 

	image* _image;
	RECT _rc;

	//프레임을 돌릴 녀석들
	int _count;
	int _currentFrameX;
	int _currentFrameY;

	//랜덤을 총알을 발사할 녀석들
	int _fireCount;
	int _rndFireCount;
public:
	enemy();
	~enemy();

	HRESULT init();
	HRESULT init(const char* imageName, POINT position);
    void release();
	void update();
	void render();

	//상속 받아 만들꺼면 virtual 아니면 그냥
	//virtual void move();
	virtual void draw();
	virtual void animation();

	bool bulletCountFire();

	RECT getRect() { return _rc; }

};

