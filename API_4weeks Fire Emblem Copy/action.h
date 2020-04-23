#pragma once
#include"gameNode.h"
/*
 콜백함수 : 사용자가 시스템에 함수의 사용을 알리는 것이 아닌
 (우리가 흔히 알고 있는 함수들)반대되는 개념이다. 즉, 사용자가
 함수를 시스템 상에 등록을 해두고 해당조건에 맞으면 시스템에
 의해 함수를 호출한다.(특정 이벤트 발생시 호출한다.)

 작성은 다음과 같이 한다.
1)함수 포인터 정의
2)사용하고자 하는 함수를 콜백함수에 넣고 특정 이벤트 발생시 정의
해둔 콜백함수를 실행하게 한다.

주의사항 : 함수포인터 기반으로 콜백구현시 static함수만 전달 가능한 단점이 있음.
<functional>
범용적이지 못한 방법이라 콜백을 구현하는 함수 포인터 보다 std::function, std::bind를
이용하여 만들면 위와 같은 단점을 없앨수 있고 편리함.
*/

/*
함수 포인터 : 함수에도 주소를 가지고 있음.
함수의 기계어 코드가 저장되어 있는 메모리 공간 안에서의 시작주소가 함수의 시작주소임.
딱히 함수에 대하여 주소를 알 필요는 없지만 함수의 주소를 인자로 받는 녀석은 필요함.(콜백함수가 해당)
*/
//이녀석이 함수 포인터
//괄호는 연산자 우선순위 때문에 붙여주어야 함.
typedef void(*CALLBACK_FUNCTION)(void);

class action : public gameNode
{
private:

	image * _image;
	
	float _startX, _startY;
	float _endX, _endY;

	float _angle;
	float _moveRange;			//이동거리
	float _worldTimeCount;		//경과시간
	float _time;


	bool _isMoving;

	CALLBACK_FUNCTION _callBackFunction; //원형을 선언

public:
	action();
	~action();

	HRESULT init();
	void release();
	void update();
	void render();


	void moveTo(image* image, float endX, float endY, float time);
	void moveTo(image* image, float endX, float endY, float time, CALLBACK_FUNCTION cbFunction);


	void moving();
	bool getIsMoving() { return _isMoving; }

};

