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
//공용으로 사용하는 녀석
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

	//총알발사
	void fire(float x, float y, float angle, float speed);
	//총알 무브
	void move();

	//삭제
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
	
	//총알발쏴
	void fire(float x, float y);
	//총알 움직여라
	void move();

	missile(){}
	~missile() {}

};
//쏠때 생성하는 미쏼(퐈이어 할때마다 생성)
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

	//발사
	void fire(float x, float y);
	//움직임
	void move(void);


	void removeSonicBoom(int arrNum);

	vector<tagBullet> getVBullet(void) { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet(void) { return _viBullet; }

	sonicBoom();
	~sonicBoom();
};