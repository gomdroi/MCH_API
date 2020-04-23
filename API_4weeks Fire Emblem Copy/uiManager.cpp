#include "stdafx.h"
#include "uiManager.h"
#include "UI_Set.h"
#include "UI_tileInfo.h"
#include "UI_command.h"
#include "UI_battleinfo.h"
#include "UI_turn.h"

uiManager::uiManager()
{
}


uiManager::~uiManager()
{
}

HRESULT uiManager::init()
{
	tileInfo = new UI_tileInfo;
	tileInfo->init();

	command = new UI_command;
	command->init();

	battleinfo = new UI_battleinfo;
	battleinfo->init();

	turnAni = new UI_turn;
	turnAni->init();

	return S_OK;
}

void uiManager::update()
{
	tileInfo->update();
	command->update();
	battleinfo->update();
	turnAni->update();
}

void uiManager::render()
{
	tileInfo->render();
	command->render();
	battleinfo->render();
	turnAni->render();
}

void uiManager::release()
{	
	//tileInfo->release();
	//command->release();
	SAFE_DELETE(tileInfo);
	SAFE_DELETE(command);
	SAFE_DELETE(battleinfo);
}

