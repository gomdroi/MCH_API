#include "stdafx.h"
#include "tile.h"
#include "unitSet.h"


tile::tile()
{
}


tile::~tile()
{
}

HRESULT tile::init()
{
	blueTile = new image;
	blueTile->init("images/UI/blue_square.bmp", 816, 51, 16, 1, true, RGB(255, 0, 255));
	redTile = new image;
	redTile->init("images/UI/red_square.bmp", 816, 51, 16, 1, true, RGB(255, 0, 255));
	terrainType = TERRAIN_NONE;
	tileType = TILE_MOVABLE;
	onTileUnit = nullptr;
	onTileUnitType = DEF_NONE;
	showBlueTile = false;
	showRedTile = false;

	settingUnitType = TYPE_NONE;
	m_image = nullptr;
	//¿¡ÀÌ½ºÅ¸¿ë ÃÊ±âÈ­
	listOn = false;
	F = BIGNUM;
	H = 0;
	parent = NULL;

	def = 0;
	avoid = 0;
	return S_OK;
}

void tile::update()
{
	animation();

	if (onTileUnit != nullptr)
	{
		onTileUnit->setOccupiedTile(this);
		onTileUnitType = onTileUnit->getUnitDef();
	}
	else if (onTileUnit == nullptr)
	{
		onTileUnitType = DEF_NONE;
	}
	
}

void tile::animation()
{
	tileFrameCount++;
	blueTile->setFrameY(0);
	if (tileFrameCount % 4 == 0)
	{
		tileFrameCount = 0;
		if (tileFrameIndex > blueTile->getMaxFrameX()) tileFrameIndex = 0;
		blueTile->setFrameX(tileFrameIndex++);
	}
}

void tile::render()
{	
	// ¹éÆÛÆÛ (Ä«¸Ş¶ó)
	int iCameraX;
	int iCameraY;
	int iCameraWidth;
	int iCameraHeight;
	iCameraX = CAMERA->m_cameraPos.x;
	iCameraY = CAMERA->m_cameraPos.y;
	// ¹Ø¿¡²« º¸·ù
	iCameraWidth = getMemDC_Width();
	iCameraHeight = getMemDC_Height();

	if (tileType == TILE_MOVABLE)
	{
		sprintf_s(moveStr, "TRUE");
	}
	else if (tileType == TILE_BLOCKED)
	{
		sprintf_s(moveStr, "FALSE");
	}
	
	if (isDebug)
	{
		RectangleMakeCenter(getMemDC(), m_position.x - iCameraX, m_position.y - iCameraY, TILEX, TILEY);

		//ÁöÇü¼Ó¼º
		TextOut(getMemDC(), m_rc.left + 5 - iCameraX, m_rc.top + 1 - iCameraY, moveStr, strlen(moveStr));
		sprintf_s(tileDefStr, "D: %d", def);
		TextOut(getMemDC(), m_rc.left + 5 - iCameraX, m_rc.top + 16 - iCameraY, tileDefStr, strlen(tileDefStr));
		sprintf_s(tileAvoStr, "A: %d", avoid);
		TextOut(getMemDC(), m_rc.left + 5 - iCameraX, m_rc.top + 31 - iCameraY, tileAvoStr, strlen(tileAvoStr));
	}
	if (showBlueTile)
	{
		//_positionX - _img->getFrameWidth() / 2, _positionY - _img->getFrameHeight() / 2, _img->getFrameX() * 150, _img->getFrameY() * 150, 150, 150, _rcAlpha
		blueTile->alphaRender(getMemDC(), m_position.x - blueTile->getFrameWidth() / 2 - CAMERA->m_cameraPos.x, m_position.y - blueTile->getFrameHeight() / 2 - CAMERA->m_cameraPos.y, blueTile->getFrameX() * 51, blueTile->getFrameY() * 51, 51, 51, 100);
	}
	else if (showRedTile)
	{
		redTile->alphaRender(getMemDC(), m_position.x - blueTile->getFrameWidth() / 2 - CAMERA->m_cameraPos.x, m_position.y - blueTile->getFrameHeight() / 2 - CAMERA->m_cameraPos.y, blueTile->getFrameX() * 51, blueTile->getFrameY() * 51, 51, 51, 100);
	}
	if (m_image != nullptr)
	{
		m_image->render(getMemDC(), m_position.x - TILEX / 2 - CAMERA->m_cameraPos.x, m_position.y - TILEY / 2 - CAMERA->m_cameraPos.y);
	}	

	//¸ÊÅø¿ë
	maptoolSavedSettingUnit();
}

void tile::release()
{
	SAFE_DELETE(blueTile);
}

void tile::setTile(TERRAIN_TYPE _terraintype)
{
	switch (_terraintype)
	{
	case TERRAIN_NONE:
		break;
	case TERRAIN_GRASS:
		terrainType = TERRAIN_GRASS;
		tileType = TILE_MOVABLE;
		def = 0;		
		avoid = 0;
		sprintf_s(tileNameStr, "Grass");
		sprintf_s(tileDefStr, "0");
		sprintf_s(tileAvoStr, "0");
		break;
	case TERRAIN_FOREST:
		terrainType = TERRAIN_FOREST;
		tileType = TILE_MOVABLE;
		def = 1;
		avoid = 10;
		sprintf_s(tileNameStr, "Forest");
		sprintf_s(tileDefStr, "1");
		sprintf_s(tileAvoStr, "10");
		break;
	case TERRAIN_CASTLEWALL:
		terrainType = TERRAIN_CASTLEWALL;
		tileType = TILE_BLOCKED;
		def = 0;
		avoid = 0;
		sprintf_s(tileNameStr, "Catlewall");
		sprintf_s(tileDefStr, "0");
		sprintf_s(tileAvoStr, "0");
		break;
	case TERRAIN_HILL:
		terrainType = TERRAIN_HILL;
		tileType = TILE_MOVABLE;
		def = 2;
		avoid = 20;
		sprintf_s(tileNameStr, "Hill");
		sprintf_s(tileDefStr, "2");
		sprintf_s(tileAvoStr, "20");
		break;
	case TERRAIN_MOUNTAIN:
		terrainType = TERRAIN_MOUNTAIN;
		tileType = TILE_BLOCKED;
		def = 0;
		avoid = 0;
		sprintf_s(tileNameStr, "Mountain");
		sprintf_s(tileDefStr, "0");
		sprintf_s(tileAvoStr, "0");
		break;
	case TERRAIN_WATER:
		terrainType = TERRAIN_WATER;
		tileType = TILE_BLOCKED;
		def = 0;
		avoid = 0;
		sprintf_s(tileNameStr, "Water");
		sprintf_s(tileDefStr, "0");
		sprintf_s(tileAvoStr, "0");
		break;
	case TERRAIN_CLIFF:
		terrainType = TERRAIN_CLIFF;
		tileType = TILE_BLOCKED;
		def = 0;
		avoid = 0;
		sprintf_s(tileNameStr, "Cliff");
		sprintf_s(tileDefStr, "0");
		sprintf_s(tileAvoStr, "0");
		break;
	case TERRAIN_TOWN:
		terrainType = TERRAIN_TOWN;
		tileType = TILE_BLOCKED;
		def = 0;
		avoid = 0;
		sprintf_s(tileNameStr, "Town");
		sprintf_s(tileDefStr, "0");
		sprintf_s(tileAvoStr, "0");
		break;
	case TERRAIN_FORT:
		terrainType = TERRAIN_FORT;
		tileType = TILE_MOVABLE;
		def = 2;
		avoid = 15;
		sprintf_s(tileNameStr, "Fort");
		sprintf_s(tileDefStr, "2");
		sprintf_s(tileAvoStr, "15");
		break;
	case TERRAIN_SHOP:
		terrainType = TERRAIN_SHOP;
		tileType = TILE_MOVABLE;
		def = 0;
		avoid = 5;
		sprintf_s(tileNameStr, "Shop");
		sprintf_s(tileDefStr, "0");
		sprintf_s(tileAvoStr, "5");
		break;
	case TERRAIN_FLOOR:
		terrainType = TERRAIN_FLOOR;
		tileType = TILE_MOVABLE;
		def = 0;
		avoid = 0;
		sprintf_s(tileNameStr, "Floor");
		sprintf_s(tileDefStr, "0");
		sprintf_s(tileAvoStr, "0");
		break;
	case TERRAIN_HOUSE:
		terrainType = TERRAIN_HOUSE;
		tileType = TILE_MOVABLE;
		def = 1;
		avoid = 5;
		sprintf_s(tileNameStr, "House");
		sprintf_s(tileDefStr, "1");
		sprintf_s(tileAvoStr, "5");
		break;
	default:
		break;
	}
}

void tile::maptoolSavedSettingUnit()
{
	switch (settingUnitType)
	{
	case TYPE_NONE:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø³í");
		break;
	case TYPE_ELIWOOD:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø¿¤¸®¿ìµå");
		break;
	case TYPE_LYN:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø¸°");
		break;
	case TYPE_HECTOR:
		m_image = IMAGEMANAGER->findImage("¸ÊÅøÇíÅÍ");
		break;
	case TYPE_GUY:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø³í");
		break;
	case TYPE_REBECCA:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø·¹º£Ä«");
		break;
	case TYPE_SERRA:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø¼¼¶ó");
		break;
	case TYPE_FLORINA:
		m_image = IMAGEMANAGER->findImage("¸ÊÅøÇÃ·Î¸®³ª");
		break;
	case TYPE_KENT:
		m_image = IMAGEMANAGER->findImage("¸ÊÅøÄËÆ®");
		break;
	case TYPE_BRIGAND:
		m_image = IMAGEMANAGER->findImage("¸ÊÅøºê¸®°£µå");
		break;
	case TYPE_SOLDIER:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø¼ÖÁ®");
		break;
	case TYPE_CAVALIER:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø±âº´");
		break;
	case TYPE_ARCHER:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø±Ã¼ö");
		break;
	case TYPE_FIGHTER:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø³í");
		break;
	case TYPE_KNIGHT:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø³ªÀÌÆ®");
		break;
	case TYPE_PEGASUSKNIGHT:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø³í");
		break;
	case TYPE_WYVERN:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø¿ÍÀÌ¹ø");
		break;
	case TYPE_WARRIOR:
		m_image = IMAGEMANAGER->findImage("¸ÊÅø¿ö¸®¾î");
		break;
	default:
		break;
	}
}
