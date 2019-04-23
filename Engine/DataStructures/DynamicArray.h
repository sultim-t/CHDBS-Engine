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

	// Get row data
	T *GetArray();
	// Get row data
	const T *GetArray() const;
	// Returns elements count
	int GetSize() const;
	// Is memory allocated for this array?
	bool IsAllocated() const;

	// Clear array
	// Note: doesn't free memory, doesn't destroy elements
	void Clear();
	// Frees allocated memory
	void Delete();

	// Allocate memory in not initialized memory
	// Note: use it ONLY when using malloc() or similar
	void RawInit(int initSize);
};

template<class T>
inline void DynamicArray<T>::Expand()
{
	int oldSize = sizeof(T) * allocated;

	allocated *= DYNARRAY_INCMULT;
	int newSize = sizeof(T) * allocated;

	buffer = (T*)SystemAllocator::Reallocate(buffer, oldSize, newSize);
}

template<class T>
inline DynamicArray<T>::DynamicArray() : 
	allocated(0), top(0) { }

template<class T>
inline DynamicArray<T>::~DynamicArray()
{
	Delete();
}

template<class T>
inline void DynamicArray<T>::Init(int initSize)
{
	// must be empty
	ASSERT(buffer == nullptr);
	ASSERT(initSize > 0);

	// init
	RawInit(initSize);
}

template<class T>
inline void DynamicArray<T>::RawInit(int initSize)
{
	// init without checking buffer and size
	top = 0;
	allocated = initSize;
	buffer = (T*)SystemAllocator::Allocate(sizeof(T) * initSize);
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
	// must exist
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
	ASSERT(top > 0);
	ASSERT(buffer != nullptr);

	return buffer[top--];
}

template<class T>
inline T * DynamicArray<T>::GetArray()
{
	return buffer;
}

template<class T>
inline const T * DynamicArray<T>::GetArray() const
{
	return buffer;
}

template<class T>
inline int DynamicArray<T>::GetSize() const
{
	return top;
}

template<class T>
inline bool DynamicArray<T>::IsAllocated() const
{
	return allocated > 0;
}

template<class T>
inline void DynamicArray<T>::Clear()
{
	top = 0;
}

template<class T>
inline void DynamicArray<T>::Delete()
{
	// delete
	if (buffer != nullptr)
	{
		SystemAllocator::Free(buffer);
	}

	buffer = nullptr;
	top = 0;
	allocated = 0;
}