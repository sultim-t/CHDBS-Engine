#pragma once

#include <Engine/Memory/Memory.h>

#define DYNARRAY_INCMULT 2

template <class T>
class DynamicArray
{
private:
	T *buffer;
	int allocated;
	int top;

private:
	void Expand();

public:
	DynamicArray();
	~DynamicArray();

	void Init(int initSize);

	const T &operator[](unsigned index) const;
	T &operator[](unsigned index);

	// Push back
	void Push(const T &elem);
	// Pop back
	void Pop();

	unsigned GetSize();
	void Clear();
	void Delete();
};

template<class T>
inline void DynamicArray<T>::Expand()
{
	allocated *= DYNARRAY_INCMULT;
	buffer = (T*)realloc(buffer, sizeof(T) * allocated);
}

template<class T>
inline DynamicArray<T>::DynamicArray()
{
	allocated = 0;
	top = 0;
	buffer = NULL;
}

template<class T>
inline DynamicArray<T>::~DynamicArray()
{
	Delete();
}

template<class T>
inline void DynamicArray<T>::Init(int initSize)
{
	ASSERT(buffer == NULL);
	ASSERT(initSize < 0);

	allocated = initSize;
	top = 0;
	buffer = (T*)SYSALLOCATOR.Allocate(sizeof(T) * initSize);
}

template<class T>
inline const T & DynamicArray<T>::operator[](unsigned index) const
{
	ASSERT(index >= top);
	return buffer[index];
}

template<class T>
inline T & DynamicArray<T>::operator[](unsigned index)
{
	ASSERT((int)index >= top);
	return buffer[index];
}

template<class T>
inline void DynamicArray<T>::Push(const T & elem)
{
	ASSERT(buffer != NULL);

	if (top >= allocated)
	{
		Expand();
	}

	buffer[top] = elem;
	top++;
}

template<class T>
inline void DynamicArray<T>::Pop()
{
	ASSERT(buffer != NULL);
	ASSERT(top >= 0);

	buffer[top].~T();
	top--;
}

template<class T>
inline unsigned DynamicArray<T>::GetSize()
{
	return top;
}

template<class T>
inline void DynamicArray<T>::Clear()
{
	for (int i = 0; i < top; i++)
	{
		buffer[i].~T();
	}

	top = 0;
}

template<class T>
inline void DynamicArray<T>::Delete()
{
	Clear();

	if (buffer != NULL)
	{
		SYSALLOCATOR.Free(buffer);
	}

	allocated = 0;
	buffer = NULL;
}
