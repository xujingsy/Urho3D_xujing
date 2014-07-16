#pragma once

template <typename T>
class Singleton
{
public:
	static T* Instance()
	{
		if(_instance == 0){	_instance = new T;}	return _instance;
	}

	static void Release()
	{
		if(_instance != 0)
		{
			delete _instance;
			_instance = 0;
		}
	}

protected:
	Singleton(void){}   

	virtual ~Singleton(void){}

	static T* _instance;
	
};
template <typename T>
T* Singleton<T>::_instance = 0;

