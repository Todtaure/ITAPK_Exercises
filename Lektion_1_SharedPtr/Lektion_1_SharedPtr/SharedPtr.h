#pragma once
#include <iostream>

template<typename T>
class Holder
{
public:
	virtual ~Holder() { }
	virtual void operator()(T* t) = 0;
};

template<typename T, typename D>
class HolderImp : public Holder<T>
{
public:
	HolderImp(D d) : d_(d)
	{
	}

	~HolderImp() {}
	void operator()(T* t) override
	{
		std::cout << "HolderImp Functor called..." << std::endl;
		d_(t);
	}

private:
	D d_;
};

namespace CustomPtr
{

	template<typename T>
	class SharedPtr
	{
	public:
		SharedPtr(T* t) : body(new T(*t)), counter(new std::size_t(1))
		{
		}

		SharedPtr(const SharedPtr& ptr)
		{
			body = ptr.body;
			counter = ptr.counter;
			helper = ptr.helper;

			(*counter)++;
		}

		template<typename D>
		SharedPtr(T* t, D d)
			:body(new T(*t)), counter(new std::size_t(1)), helper(new HolderImp<T, D>(d))
		{
		}

		~SharedPtr()
		{
			std::cout << "Destruction SharedPtr" << *body << std::endl;

			if (--(*counter) <= 0)
			{
				if (helper)
				{
					(*helper)(body);
				}
				else
				{
					delete body;
				}
				delete helper;
				delete counter;
			}
		}

		SharedPtr& operator=(const SharedPtr& ptr)
		{
			if (this != &ptr)
			{
				if (--(*(this->counter)) <= 0)
				{
					delete this->body;
				}

				this->body = ptr->body;
				this->counter = ptr->counter;
				(*counter)++;
			}
			return *this;
		}

		T& operator*() const
		{
			return *body;
		}

		T* operator->() const
		{
			return body;
		}

		template<typename T>
		friend bool operator==(const SharedPtr<T>& lhPtr, const SharedPtr<T>& rhPtr);

		std::size_t count() const
		{
			return *counter;
		}

	private:
		T* body;
		std::size_t* counter;
		Holder<T>* helper;
	};

	template<typename T>
	bool operator==(const SharedPtr<T>& lhPtr, const SharedPtr<T>& rhPtr)
	{
		return lhPtr.body == rhPtr.body;
	}

	template<typename T>
	struct HelperFunctor
	{
		void operator()(T* ptrToDelete)
		{
			std::cout << "HelperFunctor deleting ptr..." << std::endl;
			delete ptrToDelete;
		}
	};

}