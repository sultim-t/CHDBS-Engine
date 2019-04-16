#pragma once

#include <Engine/Memory/Memory.h>
#include <memory> // for shared ptr

#define DYNARRAY_INCMULT 2

template <class T>
class DynamicArray
{
private:
	std::shared_ptr<T> ptr;
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

	void operator=(const DynamicArray<T> &source);
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
	// Clear array
	// Note: doesn't free memory, doesn't destroy elements
	void Clear();
	// Frees allocated memory
	void Delete();
};

template<class T>
inline void DynamicArray<T>::Expand()
{
	int oldSize = sizeof(T) * allocated;

	allocated *= DYNARRAY_INCMULT;
	int newSize = sizeof(T) * allocated;

	ptr.reset((T*)SystemAllocator::Reallocate(ptr.get(), oldSize, newSize));
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
	ASSERT(!ptr);
	ASSERT(initSize > 0);

	top = 0;
	allocated = initSize;
	ptr.reset((T*)SystemAllocator::Allocate(sizeof(T) * initSize));
}

template<class T>
inline void DynamicArray<T>::operator=(const DynamicArray<T>& source)
{
	// source array must be not empty
	ASSERT(source.GetSize() != 0 && source.ptr);

	this->ptr = source.ptr;
	this->allocated = source.allocated;
	this->top = source.top;
}

template<class T>
inline const T &DynamicArray<T>::operator[](int index) const
{
	ASSERT(index < top);
	return ptr.get()[index];
}

template<class T>
inline T &DynamicArray<T>::operator[](int index)
{
	ASSERT(index < top);
	return ptr.get()[index];
}

template<class T>
inline void DynamicArray<T>::Push(const T &elem)
{
	// must exist
	ASSERT(ptr);

	if (top >= allocated)
	{
		Expand();
	}

	ptr.get()[top] = elem;
	top++;
}

template<class T>
inline T DynamicArray<T>::Pop()
{
	ASSERT(top > 0);
	ASSERT(ptr);

	return ptr.get()[top--];
}

template<class T>
inline T * DynamicArray<T>::GetArray()
{
	return ptr.get();
}

template<class T>
inline const T * DynamicArray<T>::GetArray() const
{
	return ptr.get();
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
	// if not initialized
	if (!ptr)
	{
		return;
	}

	// delete
	ptr.reset();

	top = 0;
	allocated = 0;
}