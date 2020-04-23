#include "stdafx.h"
#include "SubIsoMap.h"

SubIsoMap::SubIsoMap()
{
}


SubIsoMap::~SubIsoMap()
{
}

HRESULT SubIsoMap::init()
{
	for (int i = 0; i < TILE_SIZE_X; i++)
	{
		for (int j = 0; j < TILE_SIZE_Y; j++)
		{
			rc[i][j] = RectMake(
				SUBWINSIZEX / TILE_SIZE_X * i,
				300 / TILE_SIZE_Y * j + 200,
				SUBWINSIZEX / TILE_SIZE_X, 300 / TILE_SIZE_Y);
		}
	}

	int currentTile = 0;

	isDebug = false;

	return S_OK;
}

void SubIsoMap::release()
{
}

void SubIsoMap::update()
{
	if (SUBWIN->GetIsActive() && KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < TILE_SIZE_Y; i++)
		{
			for (int j = 0; j < TILE_SIZE_X; j++)
			{
				if (PtInRect(&rc[j][i], SUBWIN->GetMousePos()))
				{
					SUBWIN->SetFramePoint(PointMake(j, i));
				}
			}
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_SHIFT))
		isDebug = !isDebug;
}

void SubIsoMap::render(HDC hdc)
{

	switch (SUBWIN->GetFrameIndex())
	{
	case 0:
		IMAGEMANAGER->findImage("tile")->render(hdc, 0, 200, SUBWINSIZEX, 210);
		break;
	case 1:
		frameX = frameY = 0;

		for (int i = 0; i < TILE_SIZE_Y; i++)
		{
			for (int j = 0; j < TILE_SIZE_X; j++)
			{
				IMAGEMANAGER->findImage("tile2")->frameRender(hdc,
					0 + j * 30, 200 + i * 30,
					frameX, frameY++, 30, 30);

				if (frameY > 7)
				{
					frameY = 0;
					frameX++;
				}
			}
		}//end of for
		break;         
	case 2 : 
		frameX = 10;
		frameY = 0;

		for (int i = 0; i < TILE_SIZE_Y; i++)
		{
			for (int j = 0; j < TILE_SIZE_X; j++)
			{
				if (i < 5 && j>5)break;
				else if (i >= 5 && j > 4)break;

				IMAGEMANAGER->findImage("tile2")->frameRender(hdc,
					0 + j * 30, 200 + i * 30, 
					frameX++, frameY, 30, 30);

				if (i < 5 && frameX >15)
				{
					frameX = 10;
					frameY++;
				}
				else if (i <= 5 && frameX > 20)
				{
					frameX = 16;
					frameY++;
				}
				if (frameY >= 5)
				{
					frameY = 0;
					frameX = 16;
				}
			}
		}//end of for
		break;
	}//end of switch

	if (isDebug)
	{
		for (int i = 0; i < TILE_SIZE_X; i++)
		{
			for (int j = 0; j < TILE_SIZE_Y; j++)
			{
				RectangleMake(hdc, rc[i][j]);
			}
		}
	}
}
