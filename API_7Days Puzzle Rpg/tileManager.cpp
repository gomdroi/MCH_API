#include "stdafx.h"
#include "tileManager.h"
#include "tile.h"
#include "monsters.h"
#include "events.h"

tileManager::tileManager()
{
}


tileManager::~tileManager()
{
}

HRESULT tileManager::init()
{
	IMAGEMANAGER->addImage("빈공간", "images/emptytile.bmp", 110, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("막힌공간", "images/emptytilex.bmp", 110, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지1", "images/stage1tile.bmp", 110, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지2", "images/stage2tile.bmp", 110, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지3", "images/stage3tile.bmp", 110, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지4", "images/stage4tile.bmp", 110, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지5", "images/stage5tile.bmp", 110, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지6", "images/stage6tile.bmp", 110, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지7", "images/stage7tile.bmp", 110, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지8", "images/stage8tile.bmp", 110, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지9", "images/stage9tile.bmp", 110, 90, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("스테이지10", "images/stage10tile.bmp", 110, 90, true, RGB(255, 0, 255));
	return S_OK;
}

HRESULT tileManager::stageInit(int _stage)
{
	m_mousePositionPointer = &m_ptMouse;
	switch (_stage)
	{
	case 1:
		//타일인벤토리에 타일 세팅
		for (int i = 0; i < 4; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지1"));
			tileinven->setTileType(TILE_MONSTER);
			tileinven->setTileIndex(i);				//SUM : 4
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 4; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지1"));
			tileinven->setTileType(TILE_EVENT);
			tileinven->setTileIndex(i);				//SUM : 4
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 2; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지1"));
			tileinven->setTileType(TILE_PASSAGE);
			tileinven->setTileIndex(i);				//SUM : 2
			m_tileInvenVector.push_back(tileinven);
		}

		//맵인벤토리에 빈타일공간 세팅
		for (int i = 0; i < MAP_MAX; i++)
		{
			tile* tilemap = new tile;
			tilemap->init();
			tilemap->setImage(IMAGEMANAGER->findImage("빈공간"));
			tilemap->setTileType(TILE_EMPTY);
			tilemap->setTileIndex(i);
			m_tileMapVector.push_back(tilemap);
		}
		//타일 위치 정해주고 정렬
		reArrangeInvenTile();

		m_selectedTile = nullptr;
		//맵 깔기
		for (int i = 0; i < m_tileMapVector.size(); i++)
		{
			m_tileMapVector[i]->setPosition(PointMake(380 + ((m_tileMapVector[i]->getImage()->getWidth() + 20) * (i % 3)), 120 + ((m_tileMapVector[i]->getImage()->getHeight() + 40) * (i / 3))));
			if (i == 1 || i == 2 || i == 4 || i == 5 || i == 7 || i == 8 || i == 10 || i == 11)
			{
				m_tileMapVector[i]->setIsForbid(true);
				m_tileMapVector[i]->setImage(IMAGEMANAGER->findImage("막힌공간"));
			}
		}

		isTouch = false;
		isOccupied = false;
		break;
	case 2:
		for (int i = 0; i < 3; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지2"));
			tileinven->setTileType(TILE_MONSTER);
			tileinven->setTileIndex(4 + i);				//SUM : 7
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 3; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지2"));
			tileinven->setTileType(TILE_EVENT);
			tileinven->setTileIndex(4 + i);				//SUM : 7
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 3; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지2"));
			tileinven->setTileType(TILE_PASSAGE);
			tileinven->setTileIndex(3 + i);				//SUM : 5
			m_tileInvenVector.push_back(tileinven);
		}

		stageRelease();
		//맵에 빈공간 세팅
		for (int i = 0; i < MAP_MAX; i++)
		{
			tile* tilemap = new tile;
			tilemap->init();
			tilemap->setImage(IMAGEMANAGER->findImage("빈공간"));
			tilemap->setTileType(TILE_EMPTY);
			m_tileMapVector.push_back(tilemap);
		}

		//타일 위치 정해주고 정렬
		reArrangeInvenTile();

		m_selectedTile = nullptr;
		//맵 깔기
		for (int i = 0; i < m_tileMapVector.size(); i++)
		{
			m_tileMapVector[i]->setPosition(PointMake(380 + ((m_tileMapVector[i]->getImage()->getWidth() + 20) * (i % 3)), 120 + ((m_tileMapVector[i]->getImage()->getHeight() + 40) * (i / 3))));
			if (i == 3 || i == 6 || i == 9 || i == 10 || i == 12 || i == 13)
			{
				m_tileMapVector[i]->setIsForbid(true);
				m_tileMapVector[i]->setImage(IMAGEMANAGER->findImage("막힌공간"));
			}
		}

		isTouch = false;
		isOccupied = false;
		break;
	case 3:
		for (int i = 0; i < 5; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지3"));
			tileinven->setTileType(TILE_MONSTER);
			tileinven->setTileIndex(7 + i);				//SUM : 12
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 3; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지3"));
			tileinven->setTileType(TILE_EVENT);
			tileinven->setTileIndex(7 + i);				//SUM : 10
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 4; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지3"));
			tileinven->setTileType(TILE_PASSAGE);
			tileinven->setTileIndex(5 + i);				//SUM : 8
			m_tileInvenVector.push_back(tileinven);
		}

		stageRelease();
		//맵에 빈공간 세팅
		for (int i = 0; i < MAP_MAX; i++)
		{
			tile* tilemap = new tile;
			tilemap->init();
			tilemap->setImage(IMAGEMANAGER->findImage("빈공간"));
			tilemap->setTileType(TILE_EMPTY);
			m_tileMapVector.push_back(tilemap);
		}

		//타일 위치 정해주고 정렬
		reArrangeInvenTile();

		m_selectedTile = nullptr;
		//맵 깔기
		for (int i = 0; i < m_tileMapVector.size(); i++)
		{
			m_tileMapVector[i]->setPosition(PointMake(380 + ((m_tileMapVector[i]->getImage()->getWidth() + 20) * (i % 3)), 120 + ((m_tileMapVector[i]->getImage()->getHeight() + 40) * (i / 3))));
			if (i == 1 || i == 2 || i == 4 || i == 5 || i == 9 || i == 10 || i == 12 || i == 13)
			{
				m_tileMapVector[i]->setIsForbid(true);
				m_tileMapVector[i]->setImage(IMAGEMANAGER->findImage("막힌공간"));
			}
		}

		isTouch = false;
		isOccupied = false;
		break;
	case 4:
		for (int i = 0; i < 2; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지4"));
			tileinven->setTileType(TILE_MONSTER);
			tileinven->setTileIndex(12 + i);				//SUM : 14
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 2; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지4"));
			tileinven->setTileType(TILE_EVENT);
			tileinven->setTileIndex(10 + i);				//SUM : 12
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 2; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지4"));
			tileinven->setTileType(TILE_PASSAGE);
			tileinven->setTileIndex(8 + i);				//SUM : 10
			m_tileInvenVector.push_back(tileinven);
		}

		stageRelease();
		//맵에 빈공간 세팅
		for (int i = 0; i < MAP_MAX; i++)
		{
			tile* tilemap = new tile;
			tilemap->init();
			tilemap->setImage(IMAGEMANAGER->findImage("빈공간"));
			tilemap->setTileType(TILE_EMPTY);
			m_tileMapVector.push_back(tilemap);
		}

		//타일 위치 정해주고 정렬
		reArrangeInvenTile();

		m_selectedTile = nullptr;
		//맵 깔기
		for (int i = 0; i < m_tileMapVector.size(); i++)
		{
			m_tileMapVector[i]->setPosition(PointMake(380 + ((m_tileMapVector[i]->getImage()->getWidth() + 20) * (i % 3)), 120 + ((m_tileMapVector[i]->getImage()->getHeight() + 40) * (i / 3))));
			if ( i == 9 || i == 10 || i == 12 || i == 13)
			{
				m_tileMapVector[i]->setIsForbid(true);
				m_tileMapVector[i]->setImage(IMAGEMANAGER->findImage("막힌공간"));
			}
		}

		isTouch = false;
		isOccupied = false;
		break;
	case 5:
		for (int i = 0; i < 4; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지5"));
			tileinven->setTileType(TILE_MONSTER);
			tileinven->setTileIndex(14 + i);				//SUM : 18
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 5; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지5"));
			tileinven->setTileType(TILE_EVENT);
			tileinven->setTileIndex(12 + i);				//SUM : 17
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 4; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지5"));
			tileinven->setTileType(TILE_PASSAGE);
			tileinven->setTileIndex(10 + i);				//SUM : 14
			m_tileInvenVector.push_back(tileinven);
		}

		stageRelease();
		//맵에 빈공간 세팅
		for (int i = 0; i < MAP_MAX; i++)
		{
			tile* tilemap = new tile;
			tilemap->init();
			tilemap->setImage(IMAGEMANAGER->findImage("빈공간"));
			tilemap->setTileType(TILE_EMPTY);
			m_tileMapVector.push_back(tilemap);
		}

		//타일 위치 정해주고 정렬
		reArrangeInvenTile();

		m_selectedTile = nullptr;
		//맵 깔기
		for (int i = 0; i < m_tileMapVector.size(); i++)
		{
			m_tileMapVector[i]->setPosition(PointMake(380 + ((m_tileMapVector[i]->getImage()->getWidth() + 20) * (i % 3)), 120 + ((m_tileMapVector[i]->getImage()->getHeight() + 40) * (i / 3))));
			if (i == 12 || i == 13)
			{
				m_tileMapVector[i]->setIsForbid(true);
				m_tileMapVector[i]->setImage(IMAGEMANAGER->findImage("막힌공간"));
			}
		}

		isTouch = false;
		isOccupied = false;
		break;
	case 6:
		for (int i = 0; i < 6; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지6"));
			tileinven->setTileType(TILE_MONSTER);
			tileinven->setTileIndex(18 + i);				//SUM : 24
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 4; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지6"));
			tileinven->setTileType(TILE_EVENT);
			tileinven->setTileIndex(17 + i);				//SUM : 21
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 3; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지6"));
			tileinven->setTileType(TILE_PASSAGE);
			tileinven->setTileIndex(14 + i);				//SUM : 17
			m_tileInvenVector.push_back(tileinven);
		}

		stageRelease();
		//맵에 빈공간 세팅
		for (int i = 0; i < MAP_MAX; i++)
		{
			tile* tilemap = new tile;
			tilemap->init();
			tilemap->setImage(IMAGEMANAGER->findImage("빈공간"));
			tilemap->setTileType(TILE_EMPTY);
			m_tileMapVector.push_back(tilemap);
		}

		//타일 위치 정해주고 정렬
		reArrangeInvenTile();

		m_selectedTile = nullptr;
		//맵 깔기
		for (int i = 0; i < m_tileMapVector.size(); i++)
		{
			m_tileMapVector[i]->setPosition(PointMake(380 + ((m_tileMapVector[i]->getImage()->getWidth() + 20) * (i % 3)), 120 + ((m_tileMapVector[i]->getImage()->getHeight() + 40) * (i / 3))));
			if (i == 1 || i == 2 || i == 4 || i == 5)
			{
				m_tileMapVector[i]->setIsForbid(true);
				m_tileMapVector[i]->setImage(IMAGEMANAGER->findImage("막힌공간"));
			}
		}

		isTouch = false;
		isOccupied = false;
		break;
	case 7:
		for (int i = 0; i < 4; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지7"));
			tileinven->setTileType(TILE_MONSTER);
			tileinven->setTileIndex(24 + i);				//SUM : 28
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 3; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지7"));
			tileinven->setTileType(TILE_EVENT);
			tileinven->setTileIndex(21 + i);				//SUM : 24
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 3; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지7"));
			tileinven->setTileType(TILE_PASSAGE);
			tileinven->setTileIndex(17 + i);				//SUM : 20
			m_tileInvenVector.push_back(tileinven);
		}

		stageRelease();
		//맵에 빈공간 세팅
		for (int i = 0; i < MAP_MAX; i++)
		{
			tile* tilemap = new tile;
			tilemap->init();
			tilemap->setImage(IMAGEMANAGER->findImage("빈공간"));
			tilemap->setTileType(TILE_EMPTY);
			m_tileMapVector.push_back(tilemap);
		}

		//타일 위치 정해주고 정렬
		reArrangeInvenTile();

		m_selectedTile = nullptr;
		//맵 깔기
		for (int i = 0; i < m_tileMapVector.size(); i++)
		{
			m_tileMapVector[i]->setPosition(PointMake(380 + ((m_tileMapVector[i]->getImage()->getWidth() + 20) * (i % 3)), 120 + ((m_tileMapVector[i]->getImage()->getHeight() + 40) * (i / 3))));
			if (i == 2 || i == 5 || i == 9 || i == 10 || i == 12 || i == 13 )
			{
				m_tileMapVector[i]->setIsForbid(true);
				m_tileMapVector[i]->setImage(IMAGEMANAGER->findImage("막힌공간"));
			}
		}

		isTouch = false;
		isOccupied = false;
		break;
	case 8:
		for (int i = 0; i < 5; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지8"));
			tileinven->setTileType(TILE_MONSTER);
			tileinven->setTileIndex(28 + i);				//SUM : 33
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 2; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지8"));
			tileinven->setTileType(TILE_EVENT);
			tileinven->setTileIndex(24 + i);				//SUM : 26
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 3; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지8"));
			tileinven->setTileType(TILE_PASSAGE);
			tileinven->setTileIndex(20 + i);				//SUM : 23
			m_tileInvenVector.push_back(tileinven);
		}

		stageRelease();
		//맵에 빈공간 세팅
		for (int i = 0; i < MAP_MAX; i++)
		{
			tile* tilemap = new tile;
			tilemap->init();
			tilemap->setImage(IMAGEMANAGER->findImage("빈공간"));
			tilemap->setTileType(TILE_EMPTY);
			m_tileMapVector.push_back(tilemap);
		}

		//타일 위치 정해주고 정렬
		reArrangeInvenTile();

		m_selectedTile = nullptr;
		//맵 깔기
		for (int i = 0; i < m_tileMapVector.size(); i++)
		{
			m_tileMapVector[i]->setPosition(PointMake(380 + ((m_tileMapVector[i]->getImage()->getWidth() + 20) * (i % 3)), 120 + ((m_tileMapVector[i]->getImage()->getHeight() + 40) * (i / 3))));
			if (i == 8 || i == 11)
			{
				m_tileMapVector[i]->setIsForbid(true);
				m_tileMapVector[i]->setImage(IMAGEMANAGER->findImage("막힌공간"));
			}
		}

		isTouch = false;
		isOccupied = false;
		break;
	case 9:
		for (int i = 0; i < 6; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지9"));
			tileinven->setTileType(TILE_MONSTER);
			tileinven->setTileIndex(33 + i);				//SUM : 39
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 3; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지9"));
			tileinven->setTileType(TILE_EVENT);
			tileinven->setTileIndex(26 + i);				//SUM : 29
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 4; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지9"));
			tileinven->setTileType(TILE_PASSAGE);
			tileinven->setTileIndex(23 + i);				//SUM : 27
			m_tileInvenVector.push_back(tileinven);
		}

		stageRelease();
		//맵에 빈공간 세팅
		for (int i = 0; i < MAP_MAX; i++)
		{
			tile* tilemap = new tile;
			tilemap->init();
			tilemap->setImage(IMAGEMANAGER->findImage("빈공간"));
			tilemap->setTileType(TILE_EMPTY);
			m_tileMapVector.push_back(tilemap);
		}

		//타일 위치 정해주고 정렬
		reArrangeInvenTile();

		m_selectedTile = nullptr;
		//맵 깔기
		for (int i = 0; i < m_tileMapVector.size(); i++)
		{
			m_tileMapVector[i]->setPosition(PointMake(380 + ((m_tileMapVector[i]->getImage()->getWidth() + 20) * (i % 3)), 120 + ((m_tileMapVector[i]->getImage()->getHeight() + 40) * (i / 3))));
		}

		isTouch = false;
		isOccupied = false;
		break;
	case 10:
		for (int i = 0; i < 8; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지10"));
			tileinven->setTileType(TILE_MONSTER);
			tileinven->setTileIndex(39 + i);				//SUM : 47
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 4; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지10"));
			tileinven->setTileType(TILE_EVENT);
			tileinven->setTileIndex(29 + i);				//SUM : 33
			m_tileInvenVector.push_back(tileinven);
		}
		for (int i = 0; i < 3; i++)
		{
			tile* tileinven = new tile;
			tileinven->init();
			tileinven->setImage(IMAGEMANAGER->findImage("스테이지10"));
			tileinven->setTileType(TILE_PASSAGE);
			tileinven->setTileIndex(27 + i);				//SUM : 29
			m_tileInvenVector.push_back(tileinven);
		}

		stageRelease();
		//맵에 빈공간 세팅
		for (int i = 0; i < MAP_MAX; i++)
		{
			if (i == 10)
			{
				tile* tilemap = new tile;
				tilemap->init();
				tilemap->setImage(IMAGEMANAGER->findImage("스테이지10"));
				tilemap->setTileType(TILE_MONSTER);
				m_tileMapVector.push_back(tilemap);
			}
			else
			{
				tile* tilemap = new tile;
				tilemap->init();
				tilemap->setImage(IMAGEMANAGER->findImage("빈공간"));
				tilemap->setTileType(TILE_EMPTY);
				m_tileMapVector.push_back(tilemap);
			}
			
		}
		
		

		//타일 위치 정해주고 정렬
		reArrangeInvenTile();

		m_selectedTile = nullptr;
		//맵 깔기
		for (int i = 0; i < m_tileMapVector.size(); i++)
		{
			m_tileMapVector[i]->setPosition(PointMake(380 + ((m_tileMapVector[i]->getImage()->getWidth() + 20) * (i % 3)), 120 + ((m_tileMapVector[i]->getImage()->getHeight() + 40) * (i / 3))));
			if (i == 10)
			{
				m_tileMapVector[i]->setIsForbid(true);
			}
		}

		isTouch = false;
		isOccupied = false;
		break;
	default:
		break;
	}
	
	

	return S_OK;
}

void tileManager::stageRelease()
{
	//맵에 있는 타일 청소
	m_tileMapIter = m_tileMapVector.begin();
	for (m_tileMapIter; m_tileMapIter != m_tileMapVector.end();)
	{
		SAFE_DELETE((*m_tileMapIter));
		m_tileMapIter = m_tileMapVector.erase(m_tileMapIter);
	}
}

void tileManager::release()
{
	
}

void tileManager::update()
{
	//인벤타일 위치 꾸준히 세팅
	/*for (int i = 0; i < m_tileInvenVector.size(); i++)
	{
		m_tileInvenVector[i]->setPosition(PointMake(815 + ((m_tileInvenVector[i]->getImage()->getWidth() + 10) * (i % 2)), 35 + ((m_tileInvenVector[i]->getImage()->getHeight()+5) * (i / 2))+20 ));
		
	}*/
	dragTile();
	//드래그시에 타일이 마우스 포인터 따라다니기
	for (int i = 0; i < m_tileInvenVector.size(); i++)
	{
		if (m_tileInvenVector[i]->getIsSelected())
		{
			m_tileInvenVector[i]->setPosition(PointMake(m_ptMouse.x, m_ptMouse.y));
			break;
		}			
	}
	//몬스터 포지션 설정
	for (int i = 0; i < m_tileInvenVector.size(); i++)
	{
		if (m_tileInvenVector[i]->getTileType() == TILE_MONSTER)
		{
			for (int j = 0; j < MONSTERMANAGER->getMonsterVector().size(); j++)
			{
				if (m_tileInvenVector[i]->getTileIndex() == MONSTERMANAGER->getMonsterVector()[j]->getMonsterNumber())
				{
					/*if (MONSTERMANAGER->getMonsterVector()[j]->getIsMapPlaced() == false)*/
					
						MONSTERMANAGER->getMonsterVector()[j]->setPosition(m_tileInvenVector[i]->getPosition());
					/*	MONSTERMANAGER->getMonsterVector()[j]->setIsMapPlaced(true);*/
						break;
					
				}			
			}			
		}		
	}
	//이벤트 포지션 설정
	for (int i = 0; i < m_tileInvenVector.size(); i++)
	{
		if (m_tileInvenVector[i]->getTileType() == TILE_EVENT)
		{
			for (int j = 0; j < EVENTMANAGER->getEventVector().size(); j++)
			{
				if (m_tileInvenVector[i]->getTileIndex() == EVENTMANAGER->getEventVector()[j]->getEventNumber())
				{
					/*if (MONSTERMANAGER->getMonsterVector()[j]->getIsMapPlaced() == false)*/

					EVENTMANAGER->getEventVector()[j]->setPosition(m_tileInvenVector[i]->getPosition());
					/*EVENTMANAGER->getEventVector()[j]->setIsMapPlaced(true);*/
					break;

				}
			}
		}
	}


	//vector<monsters*> monsterVector = MONSTERMANAGER->getMonsterVector();
	//m_monsterVector = MONSTERMANAGER->MonsterVector();

	//for (int i = 0; i < monsterVector.size(); i++)
	//{
	//	/*monsterVector[i]->setPosition(m_tileInvenVector[i]->getPosition());*/
	//	m_monsterVector[i]->setStickedTilePosition(m_tileInvenVector[i]->getPosition());
	//}

}

void tileManager::render()
{
	for (int i = 0; i < m_tileMapVector.size(); i++)
	{
		m_tileMapVector[i]->render();
	}
	for (int i = 0; i < m_tileInvenVector.size(); i++)
	{
		m_tileInvenVector[i]->render();
	}
	
}

void tileManager::dragTile()
{
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		isTouch = true;

		//마우스 포인터 위치 고정 실패
		/*for (int i = 0; i < m_tileInvenVector.size(); i++)
		{
			mousePositionSave.x = m_ptMouse.x - (m_tileInvenVector[i]->getCollider().left +
				(m_tileInvenVector[i]->getCollider().left - m_tileInvenVector[i]->getCollider().right) / 2);

			mousePositionSave.y = m_ptMouse.y - (m_tileInvenVector[i]->getCollider().top +
				(m_tileInvenVector[i]->getCollider().bottom - m_tileInvenVector[i]->getCollider().top) / 2);
		}	*/			
	}

	//타일 눌렀을 때
	if (isTouch)
	{
		for (int i = 0; i < m_tileInvenVector.size(); i++)
		{
			if (PtInRect(&m_tileInvenVector[i]->getCollider(), m_ptMouse))
			{
				if (m_selectedTile) continue;
				m_tileInvenVector[i]->setIsSelected(true); // 눌렸는가
				m_selectedTile = m_tileInvenVector[i];
				//마우스 포인터 위치 고정 실패
				/*m_tileInvenVector[i]->setPosition(PointMake(m_ptMouse.x - mousePositionSave.x, m_ptMouse.y - mousePositionSave.y));*/
			}
		}
	}
	
	
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		//맵에 넣으려고 하면
		for (int i = 0; i < MAP_MAX; i++)
		{
			if (PtInRect(&m_tileMapVector[i]->getCollider(), m_ptMouse) && m_tileMapVector[i]->getIsForbid() == false)
			{
				vector<tile*>::iterator tiIter = m_tileInvenVector.begin();

				for (tiIter; tiIter != m_tileInvenVector.end(); ++tiIter)
				{
					if ((*tiIter)->getIsSelected())
					{
						//몬스터 벡터 정보
						vector<monsters*> mVector = MONSTERMANAGER->getMonsterVector();
						vector<monsters*>::iterator mIter = mVector.begin();
						//이벤트 벡터 정보
						vector<events*> eVector = EVENTMANAGER->getEventVector();
						vector<events*>::iterator eIter = eVector.begin();

						for (mIter; mIter != mVector.end();mIter++)
						{
							if (PtInRect(&m_tileMapVector[i]->getCollider(), (*mIter)->getPosition()))
							{
								(*mIter)->setPosition(m_tileMapVector[i]->getPosition());
								(*mIter)->setIsMapPlaced(true);
							}
						}

						for (eIter; eIter != eVector.end(); eIter++)
						{
							if (PtInRect(&m_tileMapVector[i]->getCollider(), (*eIter)->getPosition()))
							{
								(*eIter)->setPosition(m_tileMapVector[i]->getPosition());
								(*eIter)->setIsMapPlaced(true);
							}
						}
						/*m_tileMapVector[i] = m_tileInvenVector[j];*/
						m_tileMapVector[i]->setImage((*tiIter)->getImage()); //스왑 예시
						m_tileMapVector[i]->setIsForbid(true);
						(*tiIter)->setIsSelected(false);
						m_selectedTile = nullptr;
						isTouch = false;
						SAFE_DELETE((*tiIter));
						tiIter = m_tileInvenVector.erase(tiIter);						
						reArrangeInvenTile(); //타일 인벤토리 정리
						break;
					}
				}
			}
		}

		// 맵이 아닌 곳에 놓을경우
		isTouch = false;				
				for (int i = 0; i < m_tileInvenVector.size(); i++)
				{
					if (m_tileInvenVector[i]->getIsSelected())
					{
						m_tileInvenVector[i]->setPosition(m_tileInvenVector[i]->getStartPosition());
						m_tileInvenVector[i]->setIsSelected(false);
						m_selectedTile = nullptr;
					}
				}
					
	}

}

void tileManager::reArrangeInvenTile()
{
	for (int i = 0; i < m_tileInvenVector.size(); i++)
	{
		m_tileInvenVector[i]->setPosition(PointMake(815 + ((m_tileInvenVector[i]->getImage()->getWidth() + 10) * (i % 2)), 35 + ((m_tileInvenVector[i]->getImage()->getHeight() + 5) * (i / 2)) + 20));
		m_tileInvenVector[i]->setStartPosition(m_tileInvenVector[i]->getPosition());
	}
}
