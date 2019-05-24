#pragma once

#include <string> // for memcpy_s

// An array with dynamic allocation
template<class T>
class StaticArray
{
protected:
	T *buffer;
	UINT amount;
	
public:
	// Default constructor, everything is set to null
	inline StaticArray();
	// If there are no more references, deallocates memory
	inline ~StaticArray();

	// Allocates memory for "amount" elements of type T
	inline void Init(UINT amount);
	
	// Copies elements from source
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
};


template<class T>
inline StaticArray<T>::StaticArray() : amount(0), buffer(nullptr)
{ }

template<class T>
inline StaticArray<T>::~StaticArray()
{
	Delete();
}

template<class T>
inline void StaticArray<T>::Init(UINT amount)
{
	// this must be empty
	ASSERT(this->GetSize() == 0 && buffer == nullptr);

	this->amount = amount;

	if (amount != 0)
	{
		buffer = new T[amount]();
	}
}

template<class T>
inline void StaticArray<T>::operator=(const StaticArray<T>& source)
{
	// this must be empty
	ASSERT(this->GetSize() == 0 && ptr == nullptr);

	// allocate memory
	Init(source.GetSize());

	// copy data
	CopyFrom(source);
}

template<class T>
inline const T &StaticArray<T>::operator[](UINT index) const
{
	ASSERT(index >= 0 && index < amount);

	return buffer[index];
}

template<class T>
inline T &StaticArray<T>::operator[](UINT index)
{
	ASSERT(index >= 0 && index < amount);

	return buffer[index];
}

template<class T>
inline UINT StaticArray<T>::GetSize() const
{
	return amount;
}

template<class T>
inline const T * StaticArray<T>::GetArray() const
{
	return buffer;
}

template<class T>
inline bool StaticArray<T>::IsEmpty() const
{
	return amount == 0 || buffer == nullptr;
}

template<class T>
inline void StaticArray<T>::Delete()
{
	// if not initialized
	if (buffer == nullptr && amount == 0)
	{
		return;
	}

	// release
	delete[] buffer;

	amount = 0;
	buffer = nullptr;
}

template<class T>
inline void StaticArray<T>::CopyFrom(const StaticArray<T>& source)
{
	CopyFrom(source, source.GetSize());
}

template<class T>
inline void StaticArray<T>::CopyFrom(const StaticArray<T>& source, UINT n)
{
	if (n == 0 || source.GetSize() == 0)
	{
		return;
	}

	// self copying
	ASSERT(source.buffer != this->buffer);
	
	// existense
	ASSERT(source.GetSize() <= n && source.buffer != nullptr);
	ASSERT(this->GetSize() >= n && this->buffer != nullptr);

	int bytesToCopy = n * sizeof(T);

	std::memcpy(this->buffer, source.buffer, bytesToCopy);
}