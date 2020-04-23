#pragma once

//fmode
#include "inc/fmod.h"
#pragma  comment( lib, "fmodL64_vc.lib" )

//채널버퍼와 사운드 버퍼
#define EXTRACHANNELBUFFER 5
#define SOUNDBUFFER 100
#define EFFEXTRACHANNELBUFFER 5
#define EFFSOUNDBUFFER 100

//버퍼가 작아서 씹히는 문제를 해셜하기 위함
#define TOTALSOUNDBUFFER EXTRACHANNELBUFFER+SOUNDBUFFER
#define TOTALEFFSOUNDBUFFER EFFEXTRACHANNELBUFFER+EFFSOUNDBUFFER

/*
fmod
사운드를 플레이하기 위해 도와주는 라이브러리
간단하게 사용가능하고 지원하는 확장자가 많아서 자주 사용함
*/

//===============================================
//FOMD 전역변수
//===============================================
static FMOD_SYSTEM *g_pSystem;				//fmod의 시스템
static FMOD_SOUND **g_pSound;				//사운드클래스
static FMOD_CHANNEL **g_pChannel;			//채널 클래스

static FMOD_SYSTEM *effg_pSystem;
static FMOD_SOUND **effg_pSound;
static FMOD_CHANNEL **effg_pChannel;

class sound
{
private:
	typedef map<string, FMOD_SOUND**> arrSounds;
	typedef map<string, FMOD_SOUND**>::iterator arrSoundIter;

	arrSounds m_totalBgmSounds; //맵에 담긴 배경 사운드 목록
	arrSounds m_totalEffSounds; //맵에 담긴 효과음 사운드 목록

	float _bgmVolume;
	float _effVolume;

public:
	sound();
	~sound();

	void init();
	void update();
	void release();
	
	//배경 사운드 추가
	void addBgmSound(string keyName, string soundName, bool loop);
	//사운드 플레이
	void soundPlay(string keyName);
	//정지
	void soundOff();
	//선택 정지
	void soundOff(string keyName);
	//일시정지
	void pause(string keyName);
	//다시재생
	void resume(string keyName);
	//플레이 중인지
	bool isPlaySound(string keyName);
	//일시정지 중인지
	bool isPauseSound(string keyName);

	//효과음 추가
	void addEffSound(string keyName, string soundName, bool loop);
	//효과음 다 정지
	void effSoundOff();
	//효과음 선택 정지
	void effSoundOff(string keyName);
	//효과음 재생
	void effSoundPlay(string keyName);
	//일시정지
	void effPause(string keyName);
	//다시재생
	void effResume(string keyName);
	//플레이 중인지
	bool effIsPlaySound(string keyName);
	//일시정지 중인지
	bool effIsPauseSound(string keyName);

	void addSound();

	float getBgmVolume() { return _bgmVolume; };
	float getEffVolume() { return _effVolume; };
	void setBgmVolume(float bgmVolume) { _bgmVolume = bgmVolume; };
	void setEffVolume(float effVolume) { _effVolume = effVolume; };
};