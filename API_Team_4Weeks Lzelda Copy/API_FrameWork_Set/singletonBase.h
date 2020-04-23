#pragma once

//=================================================
//	## singletonBase ##
//=================================================

template <typename T>

class singletonBase
{

protected:
	//싱글톤 인스턴스 선언

	static T* singleton;

	singletonBase() {};
	~singletonBase() {};
public:

	static T* getSingleton();
	void releaseSingleton();

};

//싱글톤 인스턴스 선언
template <typename T>
T* singletonBase<T>::singleton = nullptr;


//싱글톤 객체 가져오기
template<typename T>
T* singletonBase<T>::getSingleton()
{

	//싱글톤 객체가 없으면 새로 생성해라
	if (!singleton)singleton = new T;

	return singleton;
}

//해제
template<typename T>
void singletonBase<T>::releaseSingleton()
{
	//싱글톤이 있다면
	if (singleton)
	{
		delete singleton;
		singleton = nullptr;
	}
}
