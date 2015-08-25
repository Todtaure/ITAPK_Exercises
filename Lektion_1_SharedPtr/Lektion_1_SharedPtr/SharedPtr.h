#pragma once
template<typename T>
class SharedPtr
{
public:
	SharedPtr(T* t);
	SharedPtr(const SharedPtr&);
	~SharedPtr();	

	SharedPtr& operator=(const SharedPtr&);
	T& operator*();
	T* operator->();

	size_t count() const;
};

