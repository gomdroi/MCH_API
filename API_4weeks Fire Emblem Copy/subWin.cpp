#include "stdafx.h"
#include "subWin.h"


POINT subWin::ptMouse = POINT{ 0,0 };

subWin::subWin()
{
	m_backBuffer = new image();
	m_backBuffer->init(SUBWINSIZEX, SUBWINSIZEY);
}


subWin::~subWin()
{
	SAFE_DELETE(m_backBuffer);
}

HRESULT subWin::init()
{
	createSubWindow();
	isStageSelect = false;
	isActive = false;
	isTerrainPlace = false;
	isUnitPlace = false;
	return S_OK;
}

void subWin::update()
{
	if (currentScene != NULL)
	{
		currentScene->update();
	}

	changeUnitImage();
}

void subWin::render()
{
	HDC hdc = GetDC(hWnd);
	PatBlt(m_backBuffer->getMemDC(), 0, 0, SUBWINSIZEX, SUBWINSIZEY, WHITENESS);

	if (currentScene != NULL)
	{
		currentScene->render(m_backBuffer->getMemDC());
	}
	m_backBuffer->render(hdc);
	ReleaseDC(hWnd, hdc);
}

void subWin::release()
{
}

void subWin::changeUnitImage()
{
	switch (selectedUnitType)
	{
	case TYPE_NONE:
		selectedUnitImage = IMAGEMANAGER->findImage("������");
		break;
	case TYPE_ELIWOOD:
		selectedUnitImage = IMAGEMANAGER->findImage("�����������");
		break;
	case TYPE_LYN:
		selectedUnitImage = IMAGEMANAGER->findImage("������");
		break;
	case TYPE_HECTOR:
		selectedUnitImage = IMAGEMANAGER->findImage("��������");
		break;
	case TYPE_GUY:
		selectedUnitImage = IMAGEMANAGER->findImage("������");
		break;
	case TYPE_REBECCA:
		selectedUnitImage = IMAGEMANAGER->findImage("��������ī");
		break;
	case TYPE_SERRA:
		selectedUnitImage = IMAGEMANAGER->findImage("��������");
		break;
	case TYPE_FLORINA:
		selectedUnitImage = IMAGEMANAGER->findImage("�����÷θ���");
		break;
	case TYPE_KENT:
		selectedUnitImage = IMAGEMANAGER->findImage("������Ʈ");
		break;
	case TYPE_BRIGAND:
		selectedUnitImage = IMAGEMANAGER->findImage("�����긮����");
		break;
	case TYPE_SOLDIER:
		selectedUnitImage = IMAGEMANAGER->findImage("��������");
		break;
	case TYPE_CAVALIER:
		selectedUnitImage = IMAGEMANAGER->findImage("�����⺴");
		break;
	case TYPE_ARCHER:
		selectedUnitImage = IMAGEMANAGER->findImage("�����ü�");
		break;
	case TYPE_FIGHTER:
		selectedUnitImage = IMAGEMANAGER->findImage("������");
		break;
	case TYPE_KNIGHT:
		selectedUnitImage = IMAGEMANAGER->findImage("��������Ʈ");
		break;
	case TYPE_PEGASUSKNIGHT:
		selectedUnitImage = IMAGEMANAGER->findImage("������");
		break;
	case TYPE_WYVERN:
		selectedUnitImage = IMAGEMANAGER->findImage("�������̹�");
		break;
	case TYPE_WARRIOR:
		selectedUnitImage = IMAGEMANAGER->findImage("����������");
		break;
	default:
		break;
	}
}

void subWin::SetScene(gameNode * _scene)
{
	currentScene = _scene;
	currentScene->init();
}

LRESULT subWin::WndLogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:

		SUBWIN->SetIsActive(true);
		ptMouse.x = LOWORD(lParam);
		ptMouse.y = HIWORD(lParam);
		break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void subWin::createSubWindow()
{
	//�α� ������ ����
	int x, y, cx, cy;
	WNDCLASS wc;
	RECT rc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)subWin::WndLogProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "subMaptool";
	RegisterClass(&wc);
	//�θ� ������ �����ʿ� ��ġ ����.
	RECT rcWin;
	GetWindowRect(m_hWnd, &rcWin);

	cx = SUBWINSIZEX;
	cy = SUBWINSIZEY;
	x = rcWin.right;
	y = rcWin.top;

	rc.left = 0;
	rc.top = 0;
	rc.right = cx;
	rc.bottom = cy;

	HWND hParenthWnd = NULL;
	HINSTANCE hInst = NULL;

	hParenthWnd = m_hWnd;
	hInst = GetModuleHandle(NULL);

	hWnd = CreateWindow("subMaptool", "subMaptool",
		WS_POPUP | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		x, y, cx, cy, hParenthWnd, NULL, hInst, NULL);

	AdjustWindowRect(&rc, WINSTYLE, FALSE);

	SetWindowPos(hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top), SWP_NOZORDER);

	ShowWindow(hWnd, SW_SHOW);
}


