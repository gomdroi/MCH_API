#include "stdafx.h"
#include "action.h"


action::action()
{
}


action::~action()
{
}

HRESULT action::init()
{

	_callBackFunction = nullptr;
	_worldTimeCount = 0.0f;
	_isMoving = false;

	return S_OK;
}

void action::release()
{
}

void action::update()
{
	moving();
}

void action::render()
{
}

void action::moveTo(image * image, float endX, float endY, float time)
{
	if (!_isMoving)
	{
		_callBackFunction = nullptr;
		_image = image;
		_startX = image->getX();
		_startY = image->getY();

		_endX = endX;
		_endY = endY;
		//이동할 거리= 시작좌표값과 끝 좌표값 거리
		_moveRange = getDistance(_startX, _startY, _endX, _endY);

		_angle = getAngle(_startX, _startY, _endX, _endY);

		//명령 받았을때 시간저장
		_worldTimeCount = TIMEMANAGER->getWorldTime();
		//몇초동안 움직일지는 인자로 들어온 값을 받아옴
		_time = time;
		//트루로 바꿔줌으로써 목적지에 도달할때 까지 함수가 실행될일 없음.
		_isMoving = true;
	}
}

void action::moveTo(image * image, float endX, float endY, float time, CALLBACK_FUNCTION cbFunction)
{
	if (!_isMoving)
	{
		_callBackFunction = cbFunction;

		_image = image;

		_startX = image->getX();
		_startY = image->getY();

		_endX = endX;
		_endY = endY;

		_moveRange = getDistance(_startX, _startY, _endX, _endY);
		_angle = getAngle(_startX, _startY, _endX, _endY);
		_worldTimeCount = TIMEMANAGER->getWorldTime();
		_time = time;
		_isMoving = true;

	}
}

void action::moving()
{
	if (!_isMoving)return;	//멈춰있으면 움직이게 할 필요 없음.

	float elapsedTime = TIMEMANAGER->getElapsedTime();
	//이동할 시간과 프레임당 시간을 나누도 이동할 거리를 곱해서 이동속도를 정함.
	float moveSpeed = (elapsedTime / _time) *_moveRange;

	_image->setX(_image->getX() + cosf(_angle)*moveSpeed);
	_image->setY(_image->getY() + (-sinf(_angle))*moveSpeed);

	float time = TIMEMANAGER->getElapsedTime();

	//만약 현재 시간이 이동할 시간을 지나가거나 같다면
	if (_time + _worldTimeCount <= TIMEMANAGER->getWorldTime())
	{
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		_image->setX(_endX);
		_image->setY(_endY);

		_isMoving = false;
		//만약 moveTo에서 콜백함수가 인자로 들어왔다면 콜백함수 실행~
		if (_callBackFunction != nullptr)
		{
			_callBackFunction();
		}
	}
}
