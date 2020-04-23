#pragma once
#include "singletonBase.h"

#define SUBWINSIZEX 240
#define SUBWINSIZEY 600

enum STAGE_NUMBER;
enum TERRAIN_TYPE;
enum UNITTYPE;
class tile;
class subWin : public singletonBase<subWin>
{
private:

	HWND hWnd;

	static POINT ptMouse;

	image* m_backBuffer;
	gameNode* currentScene;

	bool isActive;
	bool isStageSelect;
	bool isSave;
	bool isLoad;

	bool isTerrainPlace;
	bool isUnitPlace;

	STAGE_NUMBER curStage;
	TERRAIN_TYPE selectedTerrainType;
	UNITTYPE selectedUnitType;
	image* selectedUnitImage;

public:
	

	subWin();
	~subWin();

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	void changeUnitImage();
	void SetScene(gameNode* _scene);

	HWND GetHwnd() { return hWnd; }

	static POINT GetMousePos() { return ptMouse; }

	static LRESULT CALLBACK WndLogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void createSubWindow();

	void SetIsActive(bool isActive) { this->isActive = isActive; }
	bool GetIsActive() { return isActive; }

	TERRAIN_TYPE getTerrainType() { return selectedTerrainType; }
	void setTerrainType(TERRAIN_TYPE _terraintype) { selectedTerrainType = _terraintype; }
	UNITTYPE getUnitType() { return selectedUnitType; }
	void setUnitType(UNITTYPE _unittype) { selectedUnitType = _unittype; }
	image* getSelectedUnitImage() { return selectedUnitImage; }

	STAGE_NUMBER getStageNum() { return curStage; }
	void setStageNum(STAGE_NUMBER num) { curStage = num; }

	bool getStageSelect() { return isStageSelect; }
	void setStageSelect(bool _isstageselect) { isStageSelect = _isstageselect; }
	bool getIsSave() { return isSave; }
	void setIsSave(bool _issave) { isSave = _issave; }
	bool getIsLoad() { return isLoad; }
	void setIsLoad(bool _isload) { isLoad = _isload; }

	bool getIsTerrainPlace() { return isTerrainPlace; }
	void setIsTerrainPlace(bool _isterrainplace) { isTerrainPlace = _isterrainplace; }
	bool getIsUnitPlace() { return isUnitPlace; }
	void setIsUnitPlace(bool _isunitplace) { isUnitPlace = _isunitplace; }

	
};

