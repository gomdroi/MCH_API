#include "stdafx.h"
#include "scene.h"


scene::scene()
{
}


scene::~scene()
{
}

HRESULT scene::init()
{
	return S_OK;
}

void scene::update()
{
	g_cursor.update();
	TILEMANAGER->update();
	UNITMANAGER->update();			
	ASTAR->update();
	UIMANAGER->update();
	ANIMATIONMANAGER->update();
	EFFECTMANAGER->update();
}


void scene::render()
{
	TILEMANAGER->render();
	UNITMANAGER->render();
	UIMANAGER->render();
	g_cursor.render();
 	EFFECTMANAGER->render();
}

void scene::release()
{
}