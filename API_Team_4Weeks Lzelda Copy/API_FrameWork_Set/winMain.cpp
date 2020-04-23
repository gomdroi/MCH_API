#include"stdafx.h"
#include"mainGame.h"

//---------------------------------------
//이 곳에 stdafx에 선언했던 전역변수를 선언합니다

//인스턴스->메모리상에 할당되어 실행중인 프로그램
//OS 확인하기 위한 값
HINSTANCE m_hInstance;

//윈도우 핸들(문법적으로는 32비트 or 64비트의 정수값)
//사용 목적 -> 대상끼리 구분하기 위해, 문자열보다 정수가 속도가 빠르고 간편

//---------------------------------------
//전역변수 선언
HWND m_hWnd;
POINT m_ptMouse = { 0,0 };

player _player;
sound _sound;
bool _isDebug;
bool _isSound;

//---------------------------------------

mainGame mg;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setWindowSize(int x, int y, int width, int height);

/*
WndProc는 윈도우에서 이루어 지는 동작, 메시지를 처리
*/

//윈도우 메인함수
//hInstance : 인스턴스 핸들
//hPrevInstance : 이전에 실행된 인스턴스 핸들(지금은 사용하지 않음)
//lpszCmdParam : 명령행으로 입력된 프로그램 인수
//nCmdShow : 프로그램이 시작될 형태(최소화,보통크기 등등)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//인스터스를 m_hInstance(전역변수) 담아준다
	//프로그램 자체의 실체화된 주소, 프로그램 자체의 핸들
	m_hInstance = hInstance;

	//윈메인의 역할
	//1. 윈도우창 세팅 후 화면에 띄우기
	//2. 메세지 루프

	//WNDCLASS : 윈도우 창의 정보를 저장하기 위한 구조체
	//윈도우창 구조체 선언 후 초기화
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;	//윈도우 클래스에서 사용하고자 하는 여분의 메모리양을 바이트 단위로 지정
	wndClass.cbWndExtra = 0;	//cbClsExtra 유사하되 개별 윈도우에서 사용하고자 하는 여분의 메모리양을 지정
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//윈도우 작업영역에 칠할 배경 브러시를 지정
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//클래스 커서를 지정
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//타이틀바의 좌상단에 보여줄 아이콘과 윈도우가 최소화 되었을대 보여줄 아이콘을 지정
	wndClass.hInstance = hInstance;	//윈도우 클래스를 등록한 응용프로그램의 인스턴스 핸들
	wndClass.lpfnWndProc = (WNDPROC)WndProc;	//메세지를 처리하는 함수를 지정(윈도우 프로시져)
	wndClass.lpszClassName = WINNAME;	//등록하고자 하는 윈도우 클래스 이름
	wndClass.lpszMenuName = NULL;			//클래스로부터 만들어진 윈도우가 사용할 메뉴를 지정
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//윈도우 클래스의 스타일을 지정

												//윈도우 클래스 등록
												//등록할 윈도우 클래스의 특성을 지정하는 WNDCLASS 구조체의 번지를 전달
	RegisterClass(&wndClass);

	//윈도우 창 생성
	m_hWnd = CreateWindow(
		WINNAME,		//윈도우 클래스의 이름
		WINNAME,		//윈도우 타이틀바 이름
		WS_OVERLAPPEDWINDOW,//윈도우 스타일
		0,				//윈도우 화면의 x좌표
		0,				//윈도우 화면의 y좌표
		WINSIZEX,				//윈도우 화면의 가로크기
		WINSIZEY,				//윈도우 화면의 세로크기
		NULL,				//부모윈도우
		(HMENU)NULL,		//메뉴핸들
		hInstance,			//인스턴스 지정
		NULL);				//사용하지 않음

							//화면에 윈도우 보여주기


	//화면 작업 사이즈 영역 계산
	setWindowSize(0, 0, WINSIZEX, WINSIZEY);
	ShowWindow(m_hWnd, nCmdShow);

	//메인게임 클래스가 초기화가 실패하면 종료
	if (FAILED(mg.init()))
	{
		return 0;
	}

	//MSG : 운영체제에서 발생하는 메세지 정보를 저장하기 위한 구조체
	MSG message;

	//TranslateMessage : 키보드 입력메세지를 처리 담당
	//WM_CHAR 메세지를 발생시킨다.

	//DispatchMessage : 윈도우 프로시져에 전달된 메세지를 실제 윈도우로 전달해준다.

	//GetMessage : 대기상태
	/*메시지 루프
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);//키보드 입력되는 순간 메시지 처리
		DispatchMessage(&message);//실제로 윈도우로 메시지를 전달해주는 역할
	}
	//메시지 루프를 다 돌았다면 해제
	*/

	/*
		getMessage와 PeekMessage 두 함수 모두 winMain에서 메세지큐로 부터
		메세지를 추출하는 함수.
		getMessage함수는 메세지큐에 메세지가 없으면 그냥 그대로 반환값을 반환하지
		않고 그대로 대기상태로 있다가 메세지가 생기면 해당 메세지를 추출하고 반환
		값으로 1를 리턴함.
		물론 메세지가 있을때 항상 1만 리턴하는 것은 아니고 WM_QUIT 메세지가 발생
		했을경우 0을 리턴한다.


		그에 비해 PeekMessage함수는  getMessage와 다르게
		매프레임마다 함수가 실행되면 메세지 큐를 검사하고 항상 반환값으로 1을 반환한다.
		즉 대기 상태가 없음.
		매 프레임마다 함수가 실행되고 메세지 큐를 검사하고 메세지가 있든 없든 항상 1을
		반환한다는 뜻.
	*/

	//게임용 루프
	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;

			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{	//초당 60프레임으로 업데이트 해라
			TIMEMANAGER->update(60.0f);
			mg.update();
			mg.render();
		}
	}

	mg.release();
	return message.wParam;

}

//윈도우 프로시져 : 메세지를 운영체제에 전달 강제로 운영체제가 호출해준다.
//hWnd : 어느 윈도우에서 발생한 메세지인지 구분
//iMessage : 메세지 구분 번호
//wParam : unsigned int 마우스 버튼의 상태, 키보드 조합키의 상태를 전달한다.
//lParam : unsigned long 마우스 클릭 좌표를 전달
//CALLBACK 함수는 어떤 이벤트가 발생 했을 때 나에게 알려달라고 하는 것!(즉 호출 되는 것이라고 봐야함)
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return mg.MainProc(hWnd, iMessage, wParam, lParam);

}

//=================================================================
//	## 윈도우 크기 조정 ## (클라이언트 영역의 사이즈를 정확히 잡아준다)
//=================================================================
void setWindowSize(int x, int y, int width, int height)
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;

	AdjustWindowRect(&rc, WINSTYLE, false);

	//위 RECT 정보로 윈도우 사이즈 셋팅
	SetWindowPos(m_hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top), SWP_NOZORDER | SWP_NOMOVE);
}
