#pragma once

#include "StaticArray.h"

template <class T>
class StaticStack : public StaticArray<T>
{
private:
	int top;

public:
	inline StaticStack();

	// Add element to the top
	inline void Push(const T &data);
	// Remove one element from the top
	inline T Pop();

	// Get current top
	inline int GetTop() const;

	// Reset stack top to zero
	inline void Clear();
};

template<class T>
inline StaticStack<T>::StaticStack() : StaticArray(), top(0)
{ }

template<class T>
inline void StaticStack<T>::Push(const T &data)
{
	ASSERT(top < (int)amount);
	ASSERT(ptr);

	ptr.get()[top] = data;
	top++;
}

template<class T>
inline T StaticStack<T>::Pop()
{
	ASSERT(top > 0);
	ASSERT(ptr);

	return ptr.get()[top--];
}

template<class T>
inline int StaticStack<T>::GetTop() const
{
	return top;
}

template<class T>
inline void StaticStack<T>::Clear()
{
	top = 0;
}
