#pragma once
#include "singletonBase.h"
#include "playerUi.h"

enum UI_KIND
{
	UI_KIND_PLAYERINFO,
	UI_KIND_END
};

class uiManager : public singletonBase<uiManager>
{
private:
	playerUi* _playerUi;


public:
	uiManager();
	~uiManager();

	HRESULT init();
	void update();
	void release();
	void render(HDC hdc, UI_KIND kind);
};

