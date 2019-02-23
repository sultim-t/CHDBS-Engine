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
	// Reallocate memory
	void Expand();

public:
	DynamicArray();
	~DynamicArray();

	// Allocate memory
	void Init(int initSize);

	const T &operator[](int index) const;
	T &operator[](int index);

	// Push back
	void Push(const T &elem);
	// Pop back
	T Pop();

	// Returns elements count
	int GetSize() const;
	// Clear array
	// Note: doesn't frees memory, doesn't destroy elements
	void Clear();
	// Frees allocated memory
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
	buffer = nullptr;
}

template<class T>
inline DynamicArray<T>::~DynamicArray()
{
	Delete();
}

template<class T>
inline void DynamicArray<T>::Init(int initSize)
{
	ASSERT(buffer == nullptr);
	ASSERT(initSize > 0);

	allocated = initSize;
	top = 0;
	buffer = (T*)SYSALLOCATOR.Allocate(sizeof(T) * initSize);
}

template<class T>
inline const T &DynamicArray<T>::operator[](int index) const
{
	ASSERT(index < top);
	return buffer[index];
}

template<class T>
inline T &DynamicArray<T>::operator[](int index)
{
	ASSERT(index < top);
	return buffer[index];
}

template<class T>
inline void DynamicArray<T>::Push(const T &elem)
{
	ASSERT(buffer != nullptr);

	if (top >= allocated)
	{
		Expand();
	}

	buffer[top] = elem;
	top++;
}

template<class T>
inline T DynamicArray<T>::Pop()
{
	ASSERT(buffer != nullptr);
	ASSERT(top >= 0);

	return buffer[top--];
}

template<class T>
inline int DynamicArray<T>::GetSize() const
{
	return top;
}

template<class T>
inline void DynamicArray<T>::Clear()
{
	top = 0;
}

template<class T>
inline void DynamicArray<T>::Delete()
{
	Clear();

	if (buffer != nullptr)
	{
		SYSALLOCATOR.Free(buffer);
	}

	allocated = 0;
	buffer = nullptr;
}
