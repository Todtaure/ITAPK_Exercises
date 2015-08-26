#pragma once
#include <iostream>

template<typename T>
class SharedPtr
{
public:
	SharedPtr(T* t) : _body(new T(*t))
	{
		std::size_t* init = new std::size_t(1);
		_count = init;
		_hasHelper = new bool(false);

	}

	SharedPtr(const SharedPtr& ptr)
	{
		_body = ptr._body;
		_count = ptr._count;
		_helper = ptr._helper;
		_hasHelper = ptr._hasHelper;

		(*_count)++;
	}

	template<typename T, typename D>
	SharedPtr(T* t, D helper)
	{
		std::size_t* init = new std::size_t(1);
		_count = init;
		_helper = &helper;
		_hasHelper = new bool(true);
	}

	~SharedPtr()
	{
		std::cout << "Destruction SharedPtr" << *_body << endl;
			
		if (--(*_count) <= 0)
		{
			if (_hasHelper)
			{
				*_helper(_body);
			}
			else{
				delete _body;
			}
			delete _count;
		}
	}

	SharedPtr& operator=(const SharedPtr& ptr)
	{
		if (this != &ptr)
		{
			if (this->--(*_count) <= 0)
			{
				delete this->_body;
			}

			this->_body = ptr->_body;
			this->_count = ptr->_count;
			(*_count)++;
		}
	}

	T& operator*()
	{
		return *_body;
	}

	T* operator->()
	{
		return _body;
	}

	template<typename T> 
	friend bool operator==(const SharedPtr<T>& lhPtr, const SharedPtr<T>& rhPtr);

	std::size_t count() const
	{
		return *_count;
	}

private:
	T* _body;
	std::size_t* _count;
	void* _helper;
	bool* _hasHelper;
};

template<typename T>
bool operator==(const SharedPtr<T>& lhPtr, const SharedPtr<T>& rhPtr)
{
	return lhPtr._body == rhPtr._body;
}

template<typename T>
class HelperFunctor
{
public:
	void operator()()
	{

	}
};

