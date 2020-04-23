#include "stdafx.h"
#include "tile.h"




HRESULT tile::init()
{
	m_image = IMAGEMANAGER->findImage("ºó°ø°£");
	m_position = { 0,0 };
	m_collider = RectMakeCenter(m_position.x, m_position.y, 110,90);
	tileStartPosition = { 0,0 };
	tileType = TILE_EMPTY;
	isSelected = false;
	isForbid = false;


	return S_OK;
}

void tile::release()
{
}

void tile::update()
{
}

void tile::render()
{
	m_collider = RectMakeCenter(m_position.x, m_position.y, 110, 90);
	
	m_image->render(getMemDC(), m_collider.left, m_collider.top);

}

tile::tile()
{
}


tile::~tile()
{
}
