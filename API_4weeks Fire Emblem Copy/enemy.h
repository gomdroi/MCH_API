#pragma once
#include"gameNode.h"
// ���ʹ̸� �̳༮�� ��ӹ޾� ������ ����� ���� ��.
class enemy :  public gameNode
{
private : 

	image* _image;
	RECT _rc;

	//�������� ���� �༮��
	int _count;
	int _currentFrameX;
	int _currentFrameY;

	//������ �Ѿ��� �߻��� �༮��
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

	//��� �޾� ���鲨�� virtual �ƴϸ� �׳�
	//virtual void move();
	virtual void draw();
	virtual void animation();

	bool bulletCountFire();

	RECT getRect() { return _rc; }

};

