#pragma once
#include"gameNode.h"
struct tagBullet
{
	image* bulletImage;			
	RECT rc;			
	float x, y;			
	float fireX, fireY;
	float radius;
	float speed;
	float angle;
	bool fire;
	int count;
};
//�������� ����ϴ� �༮
class bullet  : public gameNode
{
private: 

	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;
	float		_range;
	int			_bulletMax;

public:
	bullet();
	~bullet();

public:

	HRESULT init(char* imageName, int bulletMax, float range);
	void release();
	void update();
	void render();

	//�Ѿ˹߻�
	void fire(float x, float y, float angle, float speed);
	//�Ѿ� ����
	void move();

	//����
	void removeMissile(int arrNum);

	vector <tagBullet> getVBullet(){ return _vBullet; }
	vector<tagBullet>::iterator getVIBullet() { return _viBullet; }


};
class missile : public gameNode
{
private : 

	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;

public : 
	HRESULT init(int bulletMax, float range);
	void release();
	void update();
	void render();
	
	//�Ѿ˹߽�
	void fire(float x, float y);
	//�Ѿ� ��������
	void move();

	missile(){}
	~missile() {}

};
//�� �����ϴ� �̜X(ǡ�̾� �Ҷ����� ����)
class sonicBoom : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	int _missileMax;
	float _range;

public:
	HRESULT init(int bulletMax, float range);
	void release(void);
	void update(void);
	void render(void);

	//�߻�
	void fire(float x, float y);
	//������
	void move(void);


	void removeSonicBoom(int arrNum);

	vector<tagBullet> getVBullet(void) { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet(void) { return _viBullet; }

	sonicBoom();
	~sonicBoom();
};