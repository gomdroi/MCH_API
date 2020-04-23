#include "stdafx.h"
#include "soundScene.h"


soundScene::soundScene()
{
}


soundScene::~soundScene()
{
	release();
}

HRESULT soundScene::init()
{
	_sceneName = SCENE_NAME_SOUND_SCENE;

	_backGround = IMAGEMANAGER->findImage("À½¾ÇÈ­¸é");
	IMAGEMANAGER->findImage("À½¾Çoff");
	IMAGEMANAGER->findImage("À½¾Çon");
	_backText = IMAGEMANAGER->findImage("¹è°æÀ½¾Ç");
	_effectText = IMAGEMANAGER->findImage("È¿°úÀ½¾Ç");

	_soundBackgruondBarBack = IMAGEMANAGER->findImage("µÚ À½¾Ç°ÔÀÌÁö");
	_soundBackgruondBarFront = IMAGEMANAGER->findImage("¾Õ À½¾Ç°ÔÀÌÁö");

	_soundEffectBarFront = IMAGEMANAGER->findImage("µÚ À½¾Ç°ÔÀÌÁö");
	_soundEffectBarBack = IMAGEMANAGER->findImage("¾Õ À½¾Ç°ÔÀÌÁö");
	_volume = IMAGEMANAGER->findImage("º¼·ýÁ¶Àý");

	_backVolumeXY.x = 50;
	_backVolumeXY.y = 420;
	_effectVolumeXY.x = 50 + WINSIZEX / 2;
	_effectVolumeXY.y = 420;

	backVolumeRc = RectMakeCenter(_backVolumeXY.x, _backVolumeXY.y, _volume->getWidth(), _volume->getHeight());
	effectVolumeRc = RectMakeCenter(_effectVolumeXY.x, _effectVolumeXY.y, _volume->getWidth(), _volume->getHeight());
	backRc = RectMakeCenter(WINSIZEX / 2, WINSIZEY - 40, 94, 34);

	backVolumeOff = RectMake(150, 160, 132, 116);
	effectVolumeOff = RectMake(150 + WINSIZEX / 2, 160, 132, 116);

	backVolume = false;
	effectVolume = false;
	
	_sound.soundOff();
	_sound.soundPlay("¼±ÅÃÃ¢");

	_sound.effSoundOff();
	_sound.effSoundPlay("ÆøÅº");

	return S_OK;
}

void soundScene::release()
{
}

void soundScene::update()
{
	_backVolumeXY.x = 50 + _soundEffectBarFront->getWidth()*_sound.getBgmVolume();
	_effectVolumeXY.x = WINSIZEX/2+50 + _soundEffectBarFront->getWidth()*_sound.getEffVolume();

	backVolumeRc = RectMakeCenter(_backVolumeXY.x, _backVolumeXY.y, _volume->getWidth(), _volume->getHeight());
	effectVolumeRc = RectMakeCenter(_effectVolumeXY.x, _effectVolumeXY.y, _volume->getWidth(), _volume->getHeight());


	if (PtInRect(&backRc, m_ptMouse))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SCENEMANAGER->changeScene("startScene");
		}
	}

	volumeControl();

}

void soundScene::render()
{
	IMAGEMANAGER->render("À½¾ÇÈ­¸é", getMemDC());
	IMAGEMANAGER->render("back", getMemDC(), backRc.left, backRc.top);
	//¹è°æÀ½¾Ç
	IMAGEMANAGER->render("¹è°æÀ½¾Ç", getMemDC(), 70, 500);
	if (_backVolumeXY.x <= 50)
	{
		IMAGEMANAGER->render("À½¾Çoff", getMemDC(), 150, 160);
	}
	if (_backVolumeXY.x > 50)
	{
		IMAGEMANAGER->render("À½¾Çon", getMemDC(), 150, 160);
	}
	IMAGEMANAGER->render("µÚ À½¾Ç°ÔÀÌÁö", getMemDC(), 50, 420);
	IMAGEMANAGER->render("¾Õ À½¾Ç°ÔÀÌÁö", getMemDC(), 50, 420, 0, 0,
		(_backVolumeXY.x - 50), _soundBackgruondBarFront->getHeight());
	IMAGEMANAGER->render("º¼·ýÁ¶Àý", getMemDC(), backVolumeRc.left, backVolumeRc.top);


	//////////////////////////////////////////////////////////////////////////////////
	//È¿°úÀ½¾Ç
	IMAGEMANAGER->render("È¿°úÀ½¾Ç", getMemDC(), 70 + WINSIZEX / 2, 500);
	if (_effectVolumeXY.x <= 50 + WINSIZEX / 2)
	{
		IMAGEMANAGER->render("À½¾Çoff", getMemDC(), 150 + WINSIZEX / 2, 160);
	}
	if (_effectVolumeXY.x > 50 + WINSIZEX / 2)
	{
		IMAGEMANAGER->render("À½¾Çon", getMemDC(), 150 + WINSIZEX / 2, 160);
	}
	IMAGEMANAGER->render("µÚ À½¾Ç°ÔÀÌÁö", getMemDC(), 50 + WINSIZEX / 2, 420);
	IMAGEMANAGER->render("¾Õ À½¾Ç°ÔÀÌÁö", getMemDC(), 50 + WINSIZEX / 2, 420, 0, 0,
		_effectVolumeXY.x - (50 + WINSIZEX / 2), _soundEffectBarFront->getHeight());
	IMAGEMANAGER->render("º¼·ýÁ¶Àý", getMemDC(), effectVolumeRc.left, effectVolumeRc.top);


}

void soundScene::volumeControl()
{
	//¹è°æÀ½¾Ç À½¼Ò°Å ¹öÆ°
	if (PtInRect(&backVolumeOff, m_ptMouse))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			_backVolumeXY.x = 50;
			_sound.setBgmVolume((((float)_backVolumeXY.x - 50) / _soundEffectBarFront->getWidth()) * 100.0 / 100.0);

			_sound.soundOff("¼±ÅÃÃ¢");
			_sound.soundPlay("¼±ÅÃÃ¢");
		}
	}
	//È¿°úÀ½ À½¼Ò°Å ¹öÆ°
	if (PtInRect(&effectVolumeOff, m_ptMouse))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			_effectVolumeXY.x = 50 + WINSIZEX / 2;
			_sound.setEffVolume((((float)_effectVolumeXY.x - (WINSIZEX / 2 + 50)) / _soundEffectBarFront->getWidth()) * 100.0 / 100.0);
		
			_sound.effSoundOff("ÆøÅº");
			_sound.effSoundPlay("ÆøÅº");
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&backVolumeRc, m_ptMouse))
		{
			backVolume = true;
		}
		if (PtInRect(&effectVolumeRc, m_ptMouse))
		{
			effectVolume = true;
		}
	}
	if (backVolume)
	{
		if (_backVolumeXY.x >= 50 && _backVolumeXY.x <= 50 + _soundBackgruondBarBack->getWidth())
		{
			_backVolumeXY.x = m_ptMouse.x;
		}
		if (_backVolumeXY.x < 50) { _backVolumeXY.x = 50; }
		if (_backVolumeXY.x > 50 + _soundBackgruondBarBack->getWidth()) { _backVolumeXY.x = 50 + _soundBackgruondBarBack->getWidth(); }

		_sound.setBgmVolume((((float)_backVolumeXY.x - 50) / _soundEffectBarFront->getWidth()) * 100.0 / 100.0);

		_sound.soundOff("¼±ÅÃÃ¢");
		_sound.soundPlay("¼±ÅÃÃ¢");

	}
	if (effectVolume)
	{
		if (_effectVolumeXY.x >= 50 + WINSIZEX / 2 && _effectVolumeXY.x <= 50 + WINSIZEX / 2 + _soundBackgruondBarBack->getWidth())
		{
			_effectVolumeXY.x = m_ptMouse.x;
		}
		if (_effectVolumeXY.x < 50 + WINSIZEX / 2) { _effectVolumeXY.x = 50 + WINSIZEX / 2; }
		if (_effectVolumeXY.x > 50 + WINSIZEX / 2 + _soundBackgruondBarBack->getWidth()) { _effectVolumeXY.x = 50 + WINSIZEX / 2 + _soundBackgruondBarBack->getWidth(); }

		_sound.setEffVolume((((float)_effectVolumeXY.x - (WINSIZEX / 2 + 50)) / _soundEffectBarFront->getWidth()) * 100.0 / 100.0);
		
		_sound.effSoundOff("ÆøÅº");
		_sound.effSoundPlay("ÆøÅº");
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		backVolume = false;
		effectVolume = false;
	}
}

