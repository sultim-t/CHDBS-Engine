#pragma once

#include <Engine/Memory/Memory.h>

// An array with dynamic allocation
template<class T>
class StaticArray
{
private:
	T *arr;
	UINT amount;
	
public:
	// Empty constructor
	inline StaticArray();

	// Allocates memory for "amount" elements of type T
	inline void Init(UINT amount);
	
	// Copies elements from "source"
	inline void operator=(const StaticArray<T> &source);
	inline const T &operator[](UINT index) const;
	inline T &operator[](UINT index);

	// Returns amount of elements for which memory were allocated
	inline UINT GetSize() const;

	// Frees allocated memory
	inline void Delete();
	// Copy "n" elements to this array from "source"
	inline void CopyFrom(const StaticArray<T> &source, UINT n);
};


template<class T>
inline StaticArray<T>::StaticArray()
{ 
	amount = 0;
	arr = nullptr;
}

template<class T>
inline void StaticArray<T>::Init(UINT amount)
{
	ASSERT(amount != 0);

	this->amount = amount;
	arr = SYSALLOCATOR.Allocate(sizeof(T) * amount);
}

template<class T>
inline void StaticArray<T>::operator=(const StaticArray<T> &source)
{
	// this array must be empty
	ASSERT(this->GetSize() == 0 && this->arr == nullptr);
	
	// source array must be not empty
	ASSERT(source.GetSize() != 0 && source.arr != nullptr);

	Init(source.amount);
	CopyFrom(source);
}

template<class T>
inline const T &StaticArray<T>::operator[](UINT index) const
{
	ASSERT(index >= 0 && index < amount);

	return arr[index];
}

template<class T>
inline T &StaticArray<T>::operator[](UINT index)
{
	ASSERT(index >= 0 && index < amount);

	return arr[index];
}

template<class T>
inline UINT StaticArray<T>::GetSize() const
{
	return amount;
}

template<class T>
inline void StaticArray<T>::Delete()
{
	ASSERT(this->arr != nullptr);
	SYSALLOCATOR.Free(arr);
}

template<class T>
inline void StaticArray<T>::CopyFrom(const StaticArray<T>& source, UINT n)
{
	ASSERT(source.GetSize() <= n && source.arr != nullptr);
	ASSERT(this->GetSize() >= n && this->arr != nullptr);

	for (UINT i = 0; i < n; i++)
	{
		arr[i] = source.arr[i];
	}
}