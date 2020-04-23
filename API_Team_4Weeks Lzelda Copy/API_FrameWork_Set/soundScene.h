#pragma once
#include "scene.h"

class soundScene : public scene
{
private:

	image* _soundEffectBarFront;	//효과음바 앞 이미지
	image* _soundEffectBarBack;		//효과음바 뒤 이미지

	image* _soundBackgruondBarFront;	//배경음바 앞 이미지
	image* _soundBackgruondBarBack;		//배경음바 뒤 이미지

	image* _volume;

	image* _backGround;

	image* _backText;
	image* _effectText;

	RECT backVolumeRc;
	RECT effectVolumeRc;
	RECT backRc;
	POINT _backVolumeXY;
	POINT _effectVolumeXY;

	RECT backVolumeOff;
	RECT effectVolumeOff;

	bool backVolume;
	bool effectVolume;
public:
	soundScene();
	~soundScene();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void volumeControl();
};


