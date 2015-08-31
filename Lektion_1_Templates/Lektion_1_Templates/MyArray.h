#pragma once

template<typename T, size_t SIZE = 10>
class MyArray
{
public:
	MyArray()
	{
		arr_start_ = &arr[0];
		arr_end_ = &arr[SIZE];
	}

	~MyArray()
	{
	}

	void fill(const T& obj)
	{
		for (auto i = 0; i < SIZE; i++)
		{
			arr[i] = obj;
		}
	}

	T* begin()
	{
		return arr_start_;
	}

	T* end()
	{
		return arr_end_;
	}

	T& operator[](int i)
	{
		return arr[i];
	}

	size_t size()
	{
		return SIZE;
	}

private:
	T arr[SIZE];
	T* arr_start_;
	T* arr_end_;
};

template<typename T>
T* myFind(T* begin, T* end, const T& val)
{
	auto iter = begin;

	while (iter != end)
	{
		if (*iter == val)
		{
			return iter;
		}
		++iter;
	}

	return end;
}

