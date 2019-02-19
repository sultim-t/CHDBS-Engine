#pragma once

template<class T, UINT Size>
class StaticArray
{
private:
	T arr[Size];
	
public:
	inline StaticArray();
	
	inline void operator=(const StaticArray<T, Size> &source);
	inline const T &operator[](UINT index) const;
	inline T &operator[](UINT index);

	inline UINT GetSize() const;

	// Copy elements to this array from "source"
	inline void CopyFrom(const StaticArray<T, Size> &source);
};


template<class T, UINT Size>
inline StaticArray<T, Size>::StaticArray()
{ }

template<class T, UINT Size>
inline void StaticArray<T, Size>::operator=(const StaticArray<T, Size>& source)
{
	CopyFrom(source);
}

template<class T, UINT Size>
inline const T &StaticArray<T, Size>::operator[](UINT index) const
{
	ASSERT(index >= 0 && index < Size);

	return arr[index];
}

template<class T, UINT Size>
inline T &StaticArray<T, Size>::operator[](UINT index)
{
	ASSERT(index >= 0 && index < Size);

	return arr[index];
}

template<class T, UINT Size>
inline UINT StaticArray<T, Size>::GetSize() const
{
	return Size;
}

template<class T, UINT Size>
inline void StaticArray<T, Size>::CopyFrom(const StaticArray<T, Size>& source)
{
	for (UINT i = 0; i < Size; i++)
	{
		arr[i] = source.arr[i];
	}
}