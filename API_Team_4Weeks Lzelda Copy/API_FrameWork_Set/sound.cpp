#include"stdafx.h"
#include "sound.h"

sound::sound()
{
}


sound::~sound()
{
}

void sound::init()
{
	//FMOD사용 초기화
	FMOD_System_Create(&g_pSystem);
	FMOD_System_Init(g_pSystem, TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	FMOD_System_Create(&effg_pSystem);
	FMOD_System_Init(effg_pSystem, TOTALEFFSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//사운드 채널 할당
	g_pSound = new FMOD_SOUND *[TOTALSOUNDBUFFER];
	g_pChannel = new FMOD_CHANNEL *[TOTALSOUNDBUFFER];
	effg_pSound = new FMOD_SOUND *[TOTALEFFSOUNDBUFFER];
	effg_pChannel = new FMOD_CHANNEL *[TOTALEFFSOUNDBUFFER];

	memset(g_pSound, 0, sizeof(FMOD_SOUND*)*(TOTALSOUNDBUFFER));
	memset(g_pChannel, 0, sizeof(FMOD_CHANNEL*)*(TOTALSOUNDBUFFER));
	memset(effg_pSound, 0, sizeof(FMOD_SOUND*)*(TOTALEFFSOUNDBUFFER));
	memset(effg_pChannel, 0, sizeof(FMOD_CHANNEL*)*(TOTALEFFSOUNDBUFFER));

	//볼륨 초기화
	_bgmVolume = 0.8f;
	_effVolume = 1.0f;

	addSound();
}

void sound::update()
{
	FMOD_System_Update(g_pSystem);
	FMOD_System_Update(effg_pSystem);
}

void sound::release()
{
	soundOff();
	FMOD_Sound_Release(*g_pSound);
	FMOD_Channel_Stop(*g_pChannel);
	FMOD_System_Release(g_pSystem);
	FMOD_System_Close(g_pSystem);
	
	effSoundOff();
	FMOD_Sound_Release(*effg_pSound);
	FMOD_Channel_Stop(*effg_pChannel);
	FMOD_System_Release(effg_pSystem);
	FMOD_System_Close(effg_pSystem);
	
	/*
	fmod에서 3d쓰고 싶으면 
	3d음향 거리값 설정 가능
	set3DminMaxDistance
	*/
}

void sound::addBgmSound(string keyName, string soundName, bool loop = FALSE)
{	
	if (loop)
	{
		FMOD_System_CreateStream(g_pSystem, soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &g_pSound[m_totalBgmSounds.size()]);
	}
	else
	{
		FMOD_System_CreateStream(g_pSystem, soundName.c_str(), FMOD_DEFAULT, nullptr, &g_pSound[m_totalBgmSounds.size()]);
	}

	m_totalBgmSounds.insert(make_pair(keyName, &g_pSound[m_totalBgmSounds.size()]));
}

void sound::soundPlay(string keyName)
{
	if (!_isSound) return;

	int count = 0;
	arrSoundIter iter = m_totalBgmSounds.begin();
	//채널에서 플레이 해야하니 카운트 돌리자.
	for (iter; iter != m_totalBgmSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//배경음 재생
			FMOD_System_PlaySound(g_pSystem, *iter->second, 0, 0, &g_pChannel[count]);
			FMOD_Channel_SetVolume(g_pChannel[count], _bgmVolume);
		}
	}
	//m_totalBgmSounds.insert(make_pair(keyName, &g_pSound[m_totalBgmSounds.size()]));
}

void sound::soundOff()
{
	int count = 0;
	arrSoundIter iter = m_totalBgmSounds.begin();
	for (iter; iter != m_totalBgmSounds.end(); ++iter, count++)
	{
		FMOD_Channel_Stop(g_pChannel[count]);
	}
}

void sound::soundOff(string keyName)
{
	int count = 0;
	arrSoundIter iter = m_totalBgmSounds.begin();
	for (iter; iter != m_totalBgmSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			FMOD_Channel_Stop(g_pChannel[count]);
		}
	}
}

void sound::pause(string keyName)
{
	int count = 0;
	arrSoundIter iter = m_totalBgmSounds.begin();

	for (iter; iter != m_totalBgmSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			FMOD_Channel_SetPaused(g_pChannel[count],true);
		}
	}
}

void sound::resume(string keyName)
{
	int count = 0;
	arrSoundIter iter = m_totalBgmSounds.begin();

	for (iter; iter != m_totalBgmSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			FMOD_Channel_SetPaused(g_pChannel[count], false);
		}
	}
}

bool sound::isPlaySound(string keyName)
{
	int count = 0;
	FMOD_BOOL isPlay;
	arrSoundIter iter = m_totalBgmSounds.begin();

	for (iter; iter != m_totalBgmSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			FMOD_Channel_IsPlaying(g_pChannel[count], &isPlay);
		}
	}

	return isPlay;
}

bool sound::isPauseSound(string keyName)
{
	int count = 0;
	FMOD_BOOL isPause;

	arrSoundIter iter = m_totalBgmSounds.begin();

	for (iter; iter != m_totalBgmSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			FMOD_Channel_GetPaused(g_pChannel[count], &isPause);
		}
	}

	return isPause;
}

void sound::addEffSound(string keyName, string soundName, bool loop)
{
	if (loop)
	{
		FMOD_System_CreateStream(effg_pSystem, soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &effg_pSound[m_totalEffSounds.size()]);
	}
	else
	{
		FMOD_System_CreateStream(effg_pSystem, soundName.c_str(), FMOD_DEFAULT, nullptr, &effg_pSound[m_totalEffSounds.size()]);
	}

	m_totalEffSounds.insert(make_pair(keyName, &effg_pSound[m_totalEffSounds.size()]));
}

void sound::effSoundOff()
{
	int count = 0;
	arrSoundIter iter = m_totalEffSounds.begin();
	for (iter; iter != m_totalEffSounds.end(); ++iter, count++)
	{
		FMOD_Channel_Stop(effg_pChannel[count]);
	}
}

void sound::effSoundOff(string keyName)
{
	int count = 0;
	arrSoundIter iter = m_totalEffSounds.begin();
	for (iter; iter != m_totalEffSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			FMOD_Channel_Stop(effg_pChannel[count]);
		}
	}
}

void sound::effSoundPlay(string keyName)
{
	if (!_isSound) return;

	int count = 0;
	arrSoundIter iter = m_totalEffSounds.begin();
	//채널에서 플레이 해야하니 카운트 돌리자.
	for (iter; iter != m_totalEffSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//배경음 재생
			FMOD_System_PlaySound(effg_pSystem, *iter->second, 0, 0, &effg_pChannel[count]);
			FMOD_Channel_SetVolume(effg_pChannel[count], _effVolume);
		}
	}
}

void sound::effPause(string keyName)
{
	int count = 0;
	arrSoundIter iter = m_totalEffSounds.begin();

	for (iter; iter != m_totalEffSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			FMOD_Channel_SetPaused(effg_pChannel[count], true);
		}
	}
}

void sound::effResume(string keyName)
{
	int count = 0;
	arrSoundIter iter = m_totalEffSounds.begin();

	for (iter; iter != m_totalEffSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			FMOD_Channel_SetPaused(effg_pChannel[count], false);
		}
	}
}

bool sound::effIsPlaySound(string keyName)
{
	int count = 0;
	FMOD_BOOL isPlay;
	arrSoundIter iter = m_totalEffSounds.begin();

	for (iter; iter != m_totalEffSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			FMOD_Channel_IsPlaying(effg_pChannel[count], &isPlay);
		}
	}

	return isPlay;
}

bool sound::effIsPauseSound(string keyName)
{
	int count = 0;
	FMOD_BOOL isPause;

	arrSoundIter iter = m_totalEffSounds.begin();

	for (iter; iter != m_totalEffSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			FMOD_Channel_GetPaused(effg_pChannel[count], &isPause);
		}
	}

	return isPause;
}

void sound::addSound()
{
	addBgmSound("타이틀", "sound/title.mp3", false);
	addBgmSound("선택창", "sound/select.mp3", true);
	addBgmSound("오버월드", "sound/overworld.mp3", true);
	addBgmSound("던전", "sound/dungeon.mp3", true);
	addBgmSound("보스", "sound/boss_bgm.mp3", true);
	addBgmSound("집","sound/home.mp3",true);
	addBgmSound("엔딩", "sound/ending.mp3", true);


	addEffSound("검","sound/sword.wav",false);
	addEffSound("화살", "sound/arrow.wav", false);
	addEffSound("폭탄", "sound/bomb.wav", false);
	addEffSound("부메랑", "sound/boomerang.wav", false);
	addEffSound("들기", "sound/lift.wav", false);
	addEffSound("죽음", "sound/link_dies.wav", false);
	addEffSound("체력없음", "sound/low_hp.wav", true);
	addEffSound("깨짐", "sound/break.wav", false);
	addEffSound("풀제거", "sound/grass_destroyed.wav", false);
	addEffSound("하트", "sound/heart.wav", false);
	addEffSound("하트조각", "sound/heart_piece.wav", false);
	addEffSound("던지기", "sound/throw.wav", false);
	addEffSound("메뉴열기", "sound/menu_open.wav", false);
	addEffSound("메뉴닫기", "sound/menu_close.wav", false);
	addEffSound("보스히트", "sound/boss_hit.wav", false);
	addEffSound("보스죽음", "sound/boss_dies.wav", false);
	addEffSound("선택", "sound/click.wav", false);

}

