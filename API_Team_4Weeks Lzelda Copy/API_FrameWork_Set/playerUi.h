#pragma once

class playerUi
{
private:
	image* _upUi;
	RECT _upUiRc;

	image* _bowUi;
	image* _boomerangUi;
	image* _bombUi;

	image* _hp0;
	image* _hp1;
	image* _hp2;
	image* _hp3;
	image* _hp4;
	image* _hp5;
	image* _hp6;

public:
	playerUi();
	~playerUi();

	HRESULT init();
	void update();
	void release();
	void render(HDC hdc);
};

