#pragma once

#include "StaticArray.h"

template <class T>
class StaticStack : public StaticArray<T>
{
private:
	int top;

public:
	inline StaticStack();

	inline void Push(const T &data);
	inline T Pop();

	inline int GetTop() const;
};

template<class T>
inline StaticStack<T>::StaticStack() : StaticArray()
{ 
	top = 0;
}

template<class T>
inline void StaticStack<T>::Push(const T &data)
{
	ASSERT(top < (int)amount);
	ASSERT(arr != nullptr);

	arr[top] = data;
	top++;
}

template<class T>
inline T StaticStack<T>::Pop()
{
	ASSERT(top > 0);
	ASSERT(arr != nullptr);

	return arr[top--];
}

template<class T>
inline int StaticStack<T>::GetTop() const
{
	return top;
}
