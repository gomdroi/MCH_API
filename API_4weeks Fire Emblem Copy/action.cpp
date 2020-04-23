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
		//�̵��� �Ÿ�= ������ǥ���� �� ��ǥ�� �Ÿ�
		_moveRange = getDistance(_startX, _startY, _endX, _endY);

		_angle = getAngle(_startX, _startY, _endX, _endY);

		//��� �޾����� �ð�����
		_worldTimeCount = TIMEMANAGER->getWorldTime();
		//���ʵ��� ���������� ���ڷ� ���� ���� �޾ƿ�
		_time = time;
		//Ʈ��� �ٲ������ν� �������� �����Ҷ� ���� �Լ��� ������� ����.
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
	if (!_isMoving)return;	//���������� �����̰� �� �ʿ� ����.

	float elapsedTime = TIMEMANAGER->getElapsedTime();
	//�̵��� �ð��� �����Ӵ� �ð��� ������ �̵��� �Ÿ��� ���ؼ� �̵��ӵ��� ����.
	float moveSpeed = (elapsedTime / _time) *_moveRange;

	_image->setX(_image->getX() + cosf(_angle)*moveSpeed);
	_image->setY(_image->getY() + (-sinf(_angle))*moveSpeed);

	float time = TIMEMANAGER->getElapsedTime();

	//���� ���� �ð��� �̵��� �ð��� �������ų� ���ٸ�
	if (_time + _worldTimeCount <= TIMEMANAGER->getWorldTime())
	{
		_worldTimeCount = TIMEMANAGER->getWorldTime();

		_image->setX(_endX);
		_image->setY(_endY);

		_isMoving = false;
		//���� moveTo���� �ݹ��Լ��� ���ڷ� ���Դٸ� �ݹ��Լ� ����~
		if (_callBackFunction != nullptr)
		{
			_callBackFunction();
		}
	}
}
