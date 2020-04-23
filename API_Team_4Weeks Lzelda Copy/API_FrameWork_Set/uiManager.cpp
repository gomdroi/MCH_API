#include "stdafx.h"
#include "uiManager.h"


uiManager::uiManager()
{
}


uiManager::~uiManager()
{
}
HRESULT uiManager::init()
{
	_playerUi = new playerUi;
	_playerUi->init();

	return S_OK;
}
void uiManager::update()
{
}
void uiManager::release()
{
}
void uiManager::render(HDC hdc, UI_KIND kind)
{
	if (kind==UI_KIND_PLAYERINFO)
	{
		_playerUi->render(hdc);
	}
}