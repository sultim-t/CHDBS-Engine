#pragma once

#include <Engine/Memory/Memory.h>
#include <string> // for memcpy_s

// An array with dynamic allocation
template<class T>
class StaticArray
{
protected:
	T *arr;
	UINT amount;
	
public:
	// Default constructor, everything is set to null
	inline StaticArray();

	// Allocates memory for "amount" elements of type T
	inline void Init(UINT amount);
	
	// Copies pointer and amount
	// Note: doesn't copy elements from source
	inline void operator=(const StaticArray<T> &source);
	inline const T &operator[](UINT index) const;
	inline T &operator[](UINT index);

	// Returns amount of elements for which memory were allocated
	inline UINT GetSize() const;
	inline const T *GetArray() const;
	inline bool IsEmpty() const;

	// Frees allocated memory
	inline void Delete();

	// Copy all elements to this array from "source"
	// Note: memory is not allocated, use Init()
	inline void CopyFrom(const StaticArray<T> &source);
	// Copy "n" elements to this array from "source"
	// Note: memory is not allocated, use Init()
	inline void CopyFrom(const StaticArray<T> &source, UINT n);

	// Allocates memory for copy, and returns it
	inline StaticArray<T> GetCopy() const;
};


template<class T>
inline StaticArray<T>::StaticArray() : amount(0), arr(nullptr)
{ }

template<class T>
inline void StaticArray<T>::Init(UINT amount)
{
	// this must be empty
	ASSERT(this->GetSize() == 0 && arr == nullptr);

	this->amount = amount;
	arr = (T*)SYSALLOCATOR.Allocate(sizeof(T) * amount);
}

template<class T>
inline void StaticArray<T>::operator=(const StaticArray<T> &source)
{
	// this array must be empty
	ASSERT(this->GetSize() == 0 && this->arr == nullptr);
	
	// source array must be not empty
	ASSERT(source.GetSize() != 0 && source.arr != nullptr);

	this->arr = source.arr;
	this->amount = source.amount;
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
inline const T * StaticArray<T>::GetArray() const
{
	return arr;
}

template<class T>
inline bool StaticArray<T>::IsEmpty() const
{
	return amount == 0 || arr == nullptr;
}

template<class T>
inline void StaticArray<T>::Delete()
{
	ASSERT(this->arr != nullptr);
	amount = 0;
	SYSALLOCATOR.Free(arr);
}

template<class T>
inline void StaticArray<T>::CopyFrom(const StaticArray<T>& source)
{
	CopyFrom(source, source.GetSize());
}

template<class T>
inline void StaticArray<T>::CopyFrom(const StaticArray<T>& source, UINT n)
{
	ASSERT(source.arr != this->arr);
	ASSERT(source.GetSize() <= n && source.arr != nullptr);
	ASSERT(this->GetSize() >= n && this->arr != nullptr);

	int bytesToCopy = n * sizeof(T);

	memcpy_s(this->arr, bytesToCopy, source.arr, bytesToCopy);
}

template<class T>
inline StaticArray<T> StaticArray<T>::GetCopy() const
{
	StaticArray<T> copy = StaticArray<T>();
	copy.Init(this->amount);

	copy.CopyFrom(*this);

	return copy;
}
