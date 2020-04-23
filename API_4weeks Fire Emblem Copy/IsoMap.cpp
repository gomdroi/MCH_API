#include "stdafx.h"
#include "IsoMap.h"


IsoMap::IsoMap()
{
}


IsoMap::~IsoMap()
{
}

HRESULT IsoMap::init()
{
	_isoX = 0;
	_isoY = 0;
	_center = 0;
	memset(_tileMap, 0, sizeof(_tileMap));

	_startX = INIT_X;
	_startY = INIT_Y;

	_isDebug = true;
	_currentCTRL = CTRL_DRAW;

	MapToolSetup();

	return S_OK;
}

void IsoMap::release()
{
}

void IsoMap::update()
{
	if (KEYMANAGER->isStayKeyDown('W')) { _startY += 5; }
	if (KEYMANAGER->isStayKeyDown('S')) { _startY -= 5; }
	if (KEYMANAGER->isStayKeyDown('A')) { _startX += 5; }
	if (KEYMANAGER->isStayKeyDown('D')) { _startX -= 5; }

	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		_isDebug = !_isDebug;
	}

	if (!SUBWIN->GetIsActive() && KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		float cellX = (float)(m_ptMouse.x - _startX);

		if (cellX < 0)
		{
			cellX = (cellX - CELL_WIDTH) / (float)CELL_WIDTH;
		}
		else
		{
			//얼만큼 떨어져서 클릭했는가를 수치화(가로지름 기준)
			cellX = cellX / (float)CELL_WIDTH;
		}

		//cellY는 -개념이 없기 때문에(기준점이 가장 위)절대값으로 구하자
		int cellY = abs(m_ptMouse.y - _startY) / CELL_HEIGHT;
		//만약 -시작점 자ㅏ체가 -이면 -1을 곱해주자
		cellY = (m_ptMouse.y < _startY) ? cellY * -1 : cellY;
		//x는 y가 기준점에 가까워 질수록 작아져야 한다.
		//마름모 이기 때문에 x축은 y의기준값으로부터 멀리 떨어진수록 x값은 커진다.
		int isoX = (int)cellX + (int)cellY;
		//y축은 x기준값으로 부터 멀리 떨어질수록 y는 늘어나게 된다.
		int isoY = (int)cellY - (int)cellX;
		/*
		//만약 x좌표가 0보다 크고 최대타일수 보다 작고
		//Y좌표 0보다 크고, 최대타일수보다 작으면 검출대상에 집어넣자
		*/
		if (isoX >= 0 && isoX < TILE_COUNT_X &&isoY >= 0 && isoY < TILE_COUNT_Y)
		{

			//1분면이면[x - 1][y]
			//2분면이면[x][y - 1]
			//3분면이면[x1][y + 1]
			//4분면이면[x + 1][y]

			//코너 검출(어느 분면에 찍혔는지)
			int corner = GetCornerIndex(isoX, isoY);
			// 검출되었는데, 그게 마름모안에 있는지 확인
			/*
			//만약 들어가 있다면 지금의 isoX, isoY는 건들 필요 없다
			//마름모 안에 들어가 있기 때문에 예외처리를 할 필요가 없다
			//만약 이 값이 false가 되었다면 마롬므가 아닌 그 밖에 찍혔다는 뜻이 되므로
			//다른 타일을 눌렀다는 것이 되고 거기에 따른 예외처리는 밑에서 써주면 된다.

			*/
			if (IsInRhombus(corner, isoX, isoY))corner = 0;

			//만약 1/4분면에서 마름모 안에 찍힌게 아니라면 그 위의 마름모가 찍히게 된다.
			switch (corner)
			{
			case 1:
				isoX = isoX - 1;//마름모의 4변중에 좌측 윗변과 맞닿아 있는 마름모는 x축으로 한단계 나ㅏㅈ은 좌표임.
				break;
			case 2:
				isoY = isoY - 1;
				break;
			case 3:
				isoY = isoY + 1;
				break;
			case 4:
				isoX = isoX + 1;
				break;
			}

			setMap(isoX, isoY, false);

			_center = corner;
			_isoX = isoX;
			_isoY = isoY;
		}
	}

	
}

void IsoMap::render()
{
	sprintf_s(str, "isoX : %d, isoY : %d, corner : %d",
		_isoX, _isoY, _center);
	TextOut(getMemDC(), 700, 20, str, strlen(str));

	DrawTileMap();
}

void IsoMap::DrawTileMap()
{
	for (int i = 0; i < TILE_COUNT_X; i++)
	{
		for (int j = 0; j < TILE_COUNT_Y; j++)
		{
			int left = _startX + (i * RADIUS_WIDTH) - (j * RADIUS_WIDTH);
			int top = _startY + (i * RADIUS_HEIGHT) + (j * RADIUS_HEIGHT);

			_tileMap[i][j].left = left;
			_tileMap[i][j].top = top;

			for (int z = 0; z <= _tileMap[i][j].index; z++)
			{
				if (_tileMap[i][j].tileKind[z] != TILEKIND_NONE)
				{
					switch (_tileMap[i][j].tileNum[z])
					{
					case 0:
						IMAGEMANAGER->frameRender("tile", getMemDC(),
							_tileMap[i][j].left,
							_tileMap[i][j].top - _tileMap[i][j].height*z,
							_tileMap[i][j].tilePos[z].x,
							_tileMap[i][j].tilePos[z].y);
						break;
					case 1:
					case 2:
						IMAGEMANAGER->frameRender("tile2", getMemDC(),
							_tileMap[i][j].left,
							_tileMap[i][j].top - _tileMap[i][j].height * z,
							_tileMap[i][j].tilePos[z].x,
							_tileMap[i][j].tilePos[z].y);
						break;
					}
				}
			}

			if (_isDebug)
			{
				DrawRhombus(left, top);
				SetTextColor(getMemDC(), RGB(0, 0, 0));
				sprintf_s(str, "(%d,%d)", i, j);
				TextOut(getMemDC(),
					left + RADIUS_WIDTH / 2 + 8,
					top + RADIUS_HEIGHT / 2 + 5, str, strlen(str));
			}
		}
	}
}

void IsoMap::DrawRhombus(int left, int top)
{
	int centerX = left + RADIUS_WIDTH;
	int centerY = top + RADIUS_HEIGHT;

	POINT p[5];
	p[0].x = centerX;
	p[0].y = centerY - RADIUS_HEIGHT;

	p[1].x = centerX + RADIUS_WIDTH;
	p[1].y = centerY;

	p[2].x = centerX;
	p[2].y = centerY + RADIUS_HEIGHT;

	p[3].x = centerX - RADIUS_WIDTH;
	p[3].y = centerY;

	p[4].x = centerX;
	p[4].y = centerY - RADIUS_HEIGHT;

	for (int i = 1; i < 5; i++) 
	{
		LineMake(getMemDC(), p[i - 1], p[i]);
	}
}

int IsoMap::GetCornerIndex(int isoX, int isoY)
{
	//클릭한 좌표점을 통해서 공식을 통해 렉트의 left,top을 구한다.
	int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
	int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);


	//left에서 마우스값을 빼면 레프트에서 이동한 값만 남는데,
	//이걸 전체길이로 나누면 0.0~ 1.0 (float으로 나눴고 , 변수가 float이니까)의 값이 나온다
	//비율값으로 마름모를 포함한 렉트를 1~4분으로 나웠을때
	//찍은 위치를 알 수 있다.
	float dx = (m_ptMouse.x - left) / (float)CELL_WIDTH;
	float dy = (m_ptMouse.y - top) / (float)CELL_HEIGHT;

	//막약 둘다 중점 보다 작으면 1/4분면
	if (dx < 0.5f && dy < 0.5f) return 1;
	//만약 dx 중점(0.5)보다 크면 2/4분면
	if (dx >= 0.5f && dy < 0.5f) return 2;
	//만약 dx 중점(0.5)보다 작고 dy 중점보다 크면 3/4분면
	if (dx < 0.5f && dy >= 0.5f) return 3;
	//만약 둘다 중점(0.5)보다 크면 4/4
	if (dx >= 0.5f && dy >= 0.5f) return 4;

	return 0;
}

bool IsoMap::IsInRhombus(int corner, int isoX, int isoY)
{
	// lefttop
	if (corner == 1) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

		//left 에서 마우스값을 뺀값(left에서 이동거리)를
		//left와 중점폭으로 top과 중점을 길이로 하는 작은 사각형으로 
		//비례하기 위해 dx와 dy를 구한다.
		float dx = (float)(m_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(m_ptMouse.y - top) / RADIUS_HEIGHT;

		//만약 전체 비례즁에서 정삼각형 안에 들어가야 하기 때문에
		//무조건 dy가 dx보다 같거나 작을 수 밖에 없다.
		//정삼각형 안에 들어가 잇는 점은 무조건 y가 x보다 작거나 
		//같을 수 밖에 없다.

		//1뺴주는 이유는 top에서부터 차이를 계산 했기 떄무에
		//바텀에서부터의 값을 구하기 위함
		if ((1.0f - dy) <= dx) return true;
		//정삼각형 밖에 있다면 
		else return false;
	}
	// righttop
	else if (corner == 2)
	{
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		//레프트의 중점의 절반을 더하는 이유는
		//검사하는 렉트가 실질적으로 마름모의 시작점인 left가
		//아닌 중점에서의 작은 렉트(사각형)이기 때문에
		//가로 반지름을 더한다
		left += RADIUS_WIDTH;
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

		float dx = (float)(m_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(m_ptMouse.y - top) / RADIUS_HEIGHT;

		if (dy >= dx) return true;
		else return false;
	}
	// leftdown
	else if (corner == 3) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);
		top += RADIUS_HEIGHT;

		float dx = (float)(m_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(m_ptMouse.y - top) / RADIUS_HEIGHT;

		if (dy < dx) return true;
		else return false;
	}
	// rightdown
	else if (corner == 4)
	{
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		left += RADIUS_WIDTH;
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);
		top += RADIUS_HEIGHT;

		float dx = (float)(m_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(m_ptMouse.y - top) / RADIUS_HEIGHT;

		if ((1.0f - dy) > dx) return true;
		else return false;
	}

	return false;
}

void IsoMap::MapToolSetup()
{
	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			_tileMap[i][j].index = -1;
			_tileMap[i][j].height = 50;
		}
	}
}

void IsoMap::setMap(int isoX, int isoY, bool isAdd)
{
	int index = SUBWIN->GetFrameIndex();

	switch (SUBWIN->GetFrameIndex())
	{
	case 0:
		imageFrame = SUBWIN->GetFramePoint();
		break;
	case 1:
		imageFrame.x = SUBWIN->GetFramePoint().y;
		imageFrame.y = SUBWIN->GetFramePoint().x;
		break;
	case 2:
		if (SUBWIN->GetFramePoint().y < 5)
		{
			imageFrame.x = SUBWIN->GetFramePoint().x + 10;
			imageFrame.y = SUBWIN->GetFramePoint().y;
		}
		else
		{
			imageFrame.x = SUBWIN->GetFramePoint().x + 16;
			imageFrame.y = SUBWIN->GetFramePoint().y - 5;
		}
		break;
	}

	_currentCTRL = SUBWIN->GetCTRL();

	switch (_currentCTRL)
	{
	case CTRL_DRAW:
		if (isAdd)
		{
			if (kindSelect(imageFrame.x, imageFrame.y) == TILEKIND_OBJECT
				&& _tileMap[isoX][isoY].index == -1)
				break;

			_tileMap[isoX][isoY].index++;
			if (_tileMap[isoX][isoY].index >= TILE_MAX)
				_tileMap[isoX][isoY].index = TILE_MAX - 1;
			_tileMap[isoX][isoY].tileNum[_tileMap[isoX][isoY].index] = index;
			_tileMap[isoX][isoY].tileKind[_tileMap[isoX][isoY].index] =
				kindSelect(imageFrame.x, imageFrame.y);
			_tileMap[isoX][isoY].tilePos[_tileMap[isoX][isoY].index]
				= imageFrame;
		}
		else
		{
			if (kindSelect(imageFrame.x, imageFrame.y) == TILEKIND_OBJECT)
				break;

			if (_tileMap[isoX][isoY].index == -1)
			{
				_tileMap[isoX][isoY].index++;
				if (_tileMap[isoX][isoY].index >= TILE_MAX)
				{
					_tileMap[isoX][isoY].index = TILE_MAX - 1;
				}
				_tileMap[isoX][isoY].tileNum[_tileMap[isoX][isoY].index] = index;
				_tileMap[isoX][isoY].tileKind[_tileMap[isoX][isoY].index] = kindSelect(imageFrame.x, imageFrame.y);
				_tileMap[isoX][isoY].tilePos[_tileMap[isoX][isoY].index] = imageFrame;
			}
			else
			{
				_tileMap[isoX][isoY].tileNum[_tileMap[isoX][isoY].index] = index;
				_tileMap[isoX][isoY].tileKind[_tileMap[isoX][isoY].index] = kindSelect(imageFrame.x, imageFrame.y);
				_tileMap[isoX][isoY].tilePos[_tileMap[isoX][isoY].index] = imageFrame;
			}
		}
		break;
	case CTRL_ERASER:
		if (_tileMap[isoX][isoY].index > -1)
		{
			_tileMap[isoX][isoY].index--;
		}
		break;
	}
}

TILEKIND IsoMap::kindSelect(int frameX, int frameY)
{
	if (frameX == -1 && frameY == -1) return TILEKIND_NONE;

	if (SUBWIN->GetFrameIndex() == 0)
	{
		if (frameY >= 7) return TILEKIND_NONE;
		if (frameY <= 4) return TILEKIND_TERRAIN;
		else return TILEKIND_OBJECT;
	}
	if (SUBWIN->GetFrameIndex() == 2)
	{
		if (frameX >= 21) return TILEKIND_NONE;
	}
	return TILEKIND_TERRAIN;
}

void IsoMap::TlieInit()
{
	for (int i = 0; i < TILE_COUNT_X; i++) {
		for (int j = 0; j < TILE_COUNT_Y; j++) {
			_tileMap[i][j].index = -1;
		}
	}
}

void IsoMap::Load()
{

}

void IsoMap::Save()
{
	
}
