#pragma once
#include"gameNode.h"
/*
 �ݹ��Լ� : ����ڰ� �ý��ۿ� �Լ��� ����� �˸��� ���� �ƴ�
 (�츮�� ���� �˰� �ִ� �Լ���)�ݴ�Ǵ� �����̴�. ��, ����ڰ�
 �Լ��� �ý��� �� ����� �صΰ� �ش����ǿ� ������ �ý��ۿ�
 ���� �Լ��� ȣ���Ѵ�.(Ư�� �̺�Ʈ �߻��� ȣ���Ѵ�.)

 �ۼ��� ������ ���� �Ѵ�.
1)�Լ� ������ ����
2)����ϰ��� �ϴ� �Լ��� �ݹ��Լ��� �ְ� Ư�� �̺�Ʈ �߻��� ����
�ص� �ݹ��Լ��� �����ϰ� �Ѵ�.

���ǻ��� : �Լ������� ������� �ݹ鱸���� static�Լ��� ���� ������ ������ ����.
<functional>
���������� ���� ����̶� �ݹ��� �����ϴ� �Լ� ������ ���� std::function, std::bind��
�̿��Ͽ� ����� ���� ���� ������ ���ټ� �ְ� ����.
*/

/*
�Լ� ������ : �Լ����� �ּҸ� ������ ����.
�Լ��� ���� �ڵ尡 ����Ǿ� �ִ� �޸� ���� �ȿ����� �����ּҰ� �Լ��� �����ּ���.
���� �Լ��� ���Ͽ� �ּҸ� �� �ʿ�� ������ �Լ��� �ּҸ� ���ڷ� �޴� �༮�� �ʿ���.(�ݹ��Լ��� �ش�)
*/
//�̳༮�� �Լ� ������
//��ȣ�� ������ �켱���� ������ �ٿ��־�� ��.
typedef void(*CALLBACK_FUNCTION)(void);

class action : public gameNode
{
private:

	image * _image;
	
	float _startX, _startY;
	float _endX, _endY;

	float _angle;
	float _moveRange;			//�̵��Ÿ�
	float _worldTimeCount;		//����ð�
	float _time;


	bool _isMoving;

	CALLBACK_FUNCTION _callBackFunction; //������ ����

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

