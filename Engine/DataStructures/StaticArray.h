#pragma once

#include <Engine/Memory/Memory.h>
#include <string> // for memcpy_s
#include <memory> // for shared ptr

// An array with dynamic allocation
template<class T>
class StaticArray
{
protected:
	std::shared_ptr<T> ptr;
	UINT amount;
	
public:
	// Default constructor, everything is set to null
	inline StaticArray();
	// If there are no more references, deallocates memory
	inline ~StaticArray();

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
inline StaticArray<T>::StaticArray() : amount(0)
{ }

template<class T>
inline StaticArray<T>::~StaticArray()
{
	// if not initialized
	if (!ptr)
	{
		return;
	}

	Delete();
}

template<class T>
inline void StaticArray<T>::Init(UINT amount)
{
	// this must be empty
	ASSERT(this->GetSize() == 0 && !ptr);

	this->amount = amount;
	ptr.reset((T*)SYSALLOCATOR.Allocate(sizeof(T) * amount));
}

template<class T>
inline void StaticArray<T>::operator=(const StaticArray<T> &source)
{
	// this array must be empty
	ASSERT(this->GetSize() == 0 && !this->ptr);
	
	// source array must be not empty
	ASSERT(source.GetSize() != 0 && source.ptr);

	this->ptr = source.ptr;
	this->amount = source.amount;
}

template<class T>
inline const T &StaticArray<T>::operator[](UINT index) const
{
	ASSERT(index >= 0 && index < amount);

	return ptr.get()[index];
}

template<class T>
inline T &StaticArray<T>::operator[](UINT index)
{
	ASSERT(index >= 0 && index < amount);

	return ptr.get()[index];
}

template<class T>
inline UINT StaticArray<T>::GetSize() const
{
	return amount;
}

template<class T>
inline const T * StaticArray<T>::GetArray() const
{
	return ptr.get();
}

template<class T>
inline bool StaticArray<T>::IsEmpty() const
{
	return amount == 0 || !ptr;
}

template<class T>
inline void StaticArray<T>::Delete()
{
	ASSERT(this->ptr);

	amount = 0;
	// release shared ptr
	ptr.reset();
}

template<class T>
inline void StaticArray<T>::CopyFrom(const StaticArray<T>& source)
{
	CopyFrom(source, source.GetSize());
}

template<class T>
inline void StaticArray<T>::CopyFrom(const StaticArray<T>& source, UINT n)
{
	// self copying
	ASSERT(source.ptr != this->ptr);
	
	// existense
	ASSERT(source.GetSize() <= n && source.ptr);
	ASSERT(this->GetSize() >= n && this->ptr);

	int bytesToCopy = n * sizeof(T);

	memcpy_s(this->ptr.get(), bytesToCopy, source.ptr.get(), bytesToCopy);
}

template<class T>
inline StaticArray<T> StaticArray<T>::GetCopy() const
{
	StaticArray<T> copy = StaticArray<T>();
	copy.Init(this->amount);

	copy.CopyFrom(*this);

	return copy;
}
