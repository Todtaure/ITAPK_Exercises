#pragma once
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <String>

/*
------------------------------------------------------------
Snippet 2
------------------------------------------------------------
Strong guarantee makes sense, because otherwise eg. capacity will be larger than the vectors actual size
this will make our full() method return a wrong result from then on and potentially we go over the bounds of our vector.
*/

class Test { /* Some code */ };

template <typename T>
class MyVector
{
public:
	MyVector(size_t capacity)
		: capacity_(capacity), count_(0), data_(new T[capacity])
	{}

	bool full() const { return count_ == capacity_; }

	void push_back(const T& oneMore)
	{
		auto tmp = this;
		try
		{
			if (full())
			{
				capacity_ *= 2;

				//Added boost shared_ptr for basic guarantee of no memory leak.
				boost::shared_ptr<T> newData = new T[capacity_];

				std::copy(data_, data_ + count_, newData);
				std::swap(data_, newData);
			}

			data_[count_] = oneMore;
			++count_;
		}
		catch (const std::exception&)
		{
			//Added Roll-back for strong guarantee.
			std::swap(this, tmp);
		}
	}

	~MyVector()
	{
		delete data_;
	}

private:
	size_t capacity_;
	size_t count_;
	T* data_;
};

/* Using */
void f()
{
	MyVector <Test > my(20);

	Test t;

	my.push_back(t);
}

/*
------------------------------------------------------------
	Snippet 3
------------------------------------------------------------
Strong guarantee not really necessary, since the intention is to change the object anyways and
if the assignment fails, the object will simply be an emtpy string (no harm done).
*/

class String
{
public:
	String() : s_(nullptr) {}

	String(const char* s) : s_(new char[strlen(s) + 1])
	{
		strcpy_s(s_, 10000, s);
	}

	String(const String& other)
		: s_(new char[strlen(other.s_) + 1])
	{
		strcpy_s(s_, 10000, other.s_);
	}

	String& operator =(const String& other)
	{
		char* tmp = s_;
		try
		{
			delete[] s_;
			s_ = new char[strlen(other.s_) + 1];
			strcpy_s(s_, 100, other.s_);
			return *this;
		}
		catch (const std::exception&)
		{
			s_ = tmp;
			return *this;
		}
	}

	~String()
	{
		delete[] s_;
	}
private:
	char* s_;
};

/* Using */
void g()
{
	String s("Hello world");

	String aCopy(s);

	s = "Hello girls";
}

/*
------------------------------------------------------------
Snippet 4
------------------------------------------------------------
For this snippet, strong guarantee is not strictly necessary, since again we intend to change the entire object
and thus losing it, simply requires us to recreate a new instance, ergo we do not lose any data.
*/

struct Blob { Blob(); Blob(Blob const *blob); };
struct Key { Key(); Key(Key const *key); bool isValid(); int id(); };

int InvalidKey(int id);

class DataSet
{
public:
	DataSet(Key* key, Blob* blob)
		: key_(key), blob_(blob)
	{
		if (!key->isValid())
		{
			//Save tmp copy of member and delete both pointers. No more memory leak on throw.
			int throwVar = key->id();
			delete key_;
			delete blob_;
			throw InvalidKey(throwVar);
		}
	}

	void overWrite(const Key* key, const Blob* blob)
	{
		Key tmpKey(key);
		Blob tmpBlob(blob);

		try
		{
			*key_ = *key;
			*blob_ = *blob;
		}
		catch (const std::exception&)
		{
			//Added strong guarantee, state is now always okay.
			std::swap(*blob_, tmpBlob);
			std::swap(*key_, tmpKey);
		}
	}

	~DataSet()
	{
		delete key_;
		delete blob_;
	}
private:
	Key* key_;
	Blob* blob_;
};

Key getKeyValue();

Blob fetchDBBlobByKey(Key key);

/* Using */
void h()
{
	//Instantiate objects before method call.
	auto var1 = new Key;
	auto var2 = new Blob;
	DataSet ds(var1, var2);

	{
		Key k(getKeyValue());
		Blob b(fetchDBBlobByKey(k));

		ds.overWrite(&k, &b);
	}
}

/*
------------------------------------------------------------
Exercise 1.3
------------------------------------------------------------
*/
namespace exerciseonethree
{
	template <typename T>
	class MyVector
	{
	public:
		//Free strong guarantee.
		explicit MyVector(int capacity = 10) : capacity_(capacity), data_(new T[capacity])
		{		
		}

		//Free strong guarantee.
		MyVector(const MyVector& other) : capacity_(other.size()), data_(new T[other.size])
		{
			try
			{
				std::copy(other.begin(), other.end(), *data_);
			}
			catch (const std::exception&)
			{
				delete data_;
			}
		}

		//Expensive strong guarantee.
		MyVector& operator =(const MyVector& other) 
		{
			auto tmpData = *data_;
			auto tmpCap = capacity_;

			try
			{
				delete data_;
				capacity_ = other.size();
				data_ = new T[capacity_];
				std::copy(other.begin(), other.end(), *data_);
			}
			catch (const std::exception&)
			{
				delete data_;
				data_ = new T[tmpData.size()];
				std::copy(tmpData.begin(), tmpData.end(), *data_);
				capacity_ = tmpCap;
			}
		}

		//Free strong guarantee.
		~MyVector()
		{
			delete data_;
		}

		//Free strong guarantee.
		int size() const
		{
			return std::count(this - begin(), this->end(), true);
		}

		//Free strong guarantee.
		T& back()
		{
			return data_[this->size() - 1];
		}

		//Expensive strong guarantee.
		void push_back(const T& t)
		{
			auto tmp = this;
			try
			{
				if (this->size() == capacity_)
				{
					capacity_ *= 2;

					boost::shared_ptr<T> newData = new T[capacity_];

					std::copy(data_, data_ + this->size(), newData);
					data_ = new T[capacity_];
					std::swap(data_, newData);
				}

				data_[this->size()] = t;
				++count_;
			}
			catch (const std::exception&)
			{
				//Added Roll-back for strong guarantee.
				std::swap(this, tmp);
			}
		}

		//Free strong guarantee.
		void pop_back()
		{
			--count_;
		}

		//Expensive strong guarantee.
		void insert(const T& t)
		{
			auto tmp = data_;
			try
			{
				std::copy(this->begin(), this->end(), data_ + 1);
				data_[0] = t;
			}
			catch (const std::exception&)
			{
				std::swap(data_, tmp);
			}
		}

		//Free strong guarantee.
		T* begin()
		{
		return data_;
		}

		//Free strong guarantee.
		T* end()
		{
			return data_ + capacity_;
		}

		//Free strong guarantee.
		T& operator [](size_t i)
		{
			return (*data_)[i];
		}

	private:
		int capacity_;
		int count_;
		T* data_; /* Contains the actual elements - data on the heap */
	};
}