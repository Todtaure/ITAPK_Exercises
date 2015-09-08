#pragma once
#include <cassert>      // assert
#include <cstddef>      // ptrdiff_t
#include <iterator>     // iterator
#include <type_traits>  // remove_cv
#include <utility>      // swap


template<typename T, size_t SIZE = 10>
class MyArray
{
public:
	MyArray()
	{
		arr_start_ = &arr_[0];
		arr_end_ = &arr_[SIZE];
	}

	template<typename D, size_t SIZE_D = 10>
	MyArray(MyArray<D, SIZE_D> const &copy)
	{
		for (auto i = 0; i < copy.size(); i++)
		{
			arr_[i] = *(copy.begin() + i);
		}

		arr_start_ = &arr_[0];
		arr_end_ = &arr_[SIZE];
	}

	~MyArray()
	{
	}

	void fill(const T &obj)
	{
		for (auto i = 0; i < SIZE; i++)
		{
			arr_[i] = obj;
		}
	}

	T* begin() const
	{
		return arr_start_;
	}

	T* end() const
	{
		return arr_end_;
	}

	T& operator[](int i)
	{
		return arr_[i];
	}

	template<typename D>
	MyArray<T, SIZE>& operator=(MyArray<D, SIZE> const &right)
	{
		for (int i = 0; i < SIZE;i++)
		{
			arr_[i] = *(right.begin() + i);
		}

		return *this;
	}

	typedef T value_type;

	size_t size() const
	{
		return SIZE;
	}

private:
	T arr_[SIZE];
	T* arr_start_;
	T* arr_end_;
};

template<typename T, typename G>
T* myFind(T* begin, T* end, const G& val)
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

template<typename T, typename G>
T** myFind(T** begin, T** end, const G& val)
{
	auto iter = begin;

	while (iter != end)
	{
		if (**iter == val)
		{
			return iter;
		}
		++iter;
	}

	return end;
}

//-------------------------------------------------------------------

template<typename T, size_t SIZE>
class MyArray<T*, SIZE>
{
public:
	MyArray()
	{
		arr_start_ = &arr_[0];
		arr_end_ = &arr_[SIZE];
	}

	template<typename D, size_t SIZE_D = 10>
	MyArray(MyArray<D, SIZE_D> const &copy)
	{
		for (auto i = 0; i < copy.size(); i++)
		{
			arr_[i] = *(copy.begin() + i);
		}

		arr_start_ = &arr_[0];
		arr_end_ = &arr_[SIZE];
	}

	~MyArray()
	{
		for (int i = 0; i < SIZE;i++)
		{
			if (arr_[i] == arr_[i - 1]){ continue; }
			delete arr_[i];
		}
	}

	void fill(T *obj)
	{
		for (auto i = 0; i < SIZE; i++)
		{
			arr_[i] = obj;
		}
	}

	T** begin() const
	{
		return arr_start_;
	}

	T** end() const
	{
		return arr_end_;
	}

	T* operator[](int i)
	{
		return arr_[i];
	}

	template<typename D>
	MyArray<T, SIZE>& operator=(MyArray<D, SIZE> const &right)
	{
		for (int i = 0; i < SIZE; i++)
		{
			arr_[i] = *(right.begin() + i);
		}

		return *this;
	}

	size_t size() const
	{
		return SIZE;
	}

	typedef T value_type;

private:
	T* arr_[SIZE];
	T** arr_start_;
	T** arr_end_;
};

template<typename U>
typename U::value_type myAccumulation(const U& u)
{
	typename U::value_type m = typename U::value_type();

	for (auto first = u.begin(); first != u.end(); ++first)
	{
		m += *first;
	}

	return m;
}


template<typename U, typename P>
typename U::value_type myAccumulation(const U& u, P p)
{
	typename U::value_type m = typename U::value_type();

	for (auto first = u.begin(); first != u.end(); ++first)
	{
		m += p(*first);
	}

	return m;
}


/*
Iterator for MyArray
*/

template<typename Type>
class ForwardIterator : public std::iterator<std::forward_iterator_tag, Type>
{
	ForwardIterator():t_(nullptr){}

	ForwardIterator(Type *t) : t_(t){}
	
	ForwardIterator(const ForwardIterator &fit) : t_(fit.t){}
	
	ForwardIterator& operator++()
	{
		++t_;
		return *this;
	}
	
	ForwardIterator operator++(int)
	{
		ForwardIterator tmp(*this);
		operator++();
		return tmp;
	}

	template<typename other>
	bool operator==(const ForwardIterator<other>& rhs)
	{
		return t_ == rhs.t_;
	}

	template<typename other>
	bool operator!=(const ForwardIterator<other>& rhs)
	{
		return t_ != rhs.t_;
	}
	
	Type& operator*()
	{
		return *t_;
	}
	
	Type& operator->()
	{
		return *t_;
	}

	void swap(ForwardIterator& other) noexcept
	{
		using std::swap;
		swap(t_, other.t_);
	}

private:
	Type* t_;
};