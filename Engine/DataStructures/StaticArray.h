#pragma once

#include <Engine/Memory/SystemAllocator.h>

template<class T>
class StaticArray
{
private:
	T *arr;
	unsigned size;
	
public:
	inline StaticArray();
	inline ~StaticArray();
	
	// Allocates memory for array
	inline void Init(unsigned count);
	
	inline void operator=(const StaticArray<T> &source);
	inline const T &operator[](unsigned index) const;
	inline T &operator[](unsigned index);

	inline unsigned GetSize() const;

	// Copy elements to this array from "source"
	inline void CopyFrom(const StaticArray<T> &source);

	// Reset to default, destroys allocated data
	inline void Delete();
};

template<class T>
inline StaticArray<T>::StaticArray()
{
	this->arr = NULL;
	this->size = 0;
}

template<class T>
inline StaticArray<T>::~StaticArray()
{
	Delete();
}

template<class T>
inline void StaticArray<T>::Init(unsigned count)
{
	ASSERT(this != NULL && count != 0);

	this->size = count;
	arr = SYSALLOCATOR.Allocate(sizeof(T) * count);
}

template<class T>
inline void StaticArray<T>::operator=(const StaticArray<T>& source)
{
	CopyFrom(source);
}

template<class T>
inline const T & StaticArray<T>::operator[](unsigned index) const
{
	ASSERT(this-> != NULL);
	ASSERT(index >= 0 && i < size);

	return arr[index];
}

template<class T>
inline T & StaticArray<T>::operator[](unsigned index)
{
	ASSERT(this-> != NULL);
	ASSERT(index >= 0 && i < size);

	return arr[index];
}

template<class T>
inline unsigned StaticArray<T>::GetSize() const
{
	return size;
}

template<class T>
inline void StaticArray<T>::CopyFrom(const StaticArray<T> &source)
{
	ASSERT(this != NULL);
	ASSERT(&source != NULL);
	ASSERT(this != &source);

	Clear();

	unsigned sourceSize = source.size;
	if (sourceSize == 0)
	{
		return;
	}

	Init(sourceSize);
	for (unsigned i = 0; i < sourceSize; i++)
	{
		arr[i] = source[i];
	}
}

template<class T>
inline void StaticArray<T>::Delete()
{
	ASSERT(this-> != NULL);
	
	SYSALLOCATOR.Free(arr);
	arr = NULL;
	size = 0;
}
