#pragma once
class camera
{
private:
	int _cameraX,_cameraY;

public:
	camera();
	~camera();

	int getCameraX() { return _cameraX; };
	int getCameraY() { return _cameraY; };
	void setCameraX(int cameraX) { _cameraX = cameraX; };
	void setCameraY(int cameraY) { _cameraY = cameraY; };

	HRESULT init();
};

