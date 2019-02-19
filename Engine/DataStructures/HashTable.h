#pragma once
#include "StaticArray.h"
#include "LinkedList.h"

#include <Engine/Memory/Memory.h>

#define HASH_TABLE_INC_MULT 2

template <class K, class T>
class HashTable
{
	typedef UINT(*HashFunction)(K);

private:

	template <class K, class T>
	struct HTElement
	{
		K Key;
		T Value;

		HTElement(K key, T value)
		{
			this->Key = key;
			this->Value = value;
		}

		// default
		~HTElement() { }

		void operator=(HTElement &elem)
		{
			this->Key = elem.Key;
			this->Value = elem.Value;
		}

		bool operator==(HTElement &elem)
		{
			return Key == elem.Key;
		}
	};

private:
	HTElement<K, T> **chains;
	HashFunction hashFunc;

	UINT chainCount;
	UINT maxChainSize;
	UINT *chainSizes;

private:
	// Very expensive
	// To avoid use bigger chainCount
	void Resize();

public:
	HashTable();
	~HashTable();

	void Init(UINT chainCount, UINT maxChainSize);

	void DeclareHashFunction(HashFunction hashFunc);

	// Calculates key's hash and adds to table
	void Add(const K &key, const T &value);
	// Calculates key's hash and removes from table
	// Removed element is assigned to "outValue"
	// Returns true if is removed
	// For rare use
	bool Remove(const K &key, T &outValue);

	// Finds chain by hash, finds by key element in chain
	// Assigns found value to "outValue"
	// Slow if "maxChainSize" is big
	bool Find(const K &key, T &outValue) const;

	// Destroy all data
	void Delete();
};

template<class K, class T>
inline HashTable<K, T>::HashTable()
{ 
	chains = NULL;
	chainSizes = NULL;
}

template<class K, class T>
inline HashTable<K, T>::~HashTable()
{
	Delete();
}

template<class K, class T>
inline void HashTable<K, T>::Init(UINT chainCount, UINT maxChainSize)
{
	ASSERT(chains == NULL);
	ASSERT(chainSizes == NULL);

	this->chainCount = chainCount;
	this->maxChainSize = maxChainSize;

	chainSizes = (UINT*)SYSALLOCATOR.Allocate(sizeof(UINT) * chainCount);

	chains = (HTElement<K, T>**)SYSALLOCATOR.Allocate(sizeof(HTElement<K, T>*) * chainCount);
	for (UINT i = 0; i < chainCount; i++)
	{
		// can't use malloc because there must be uninitialized keys and values 
		chains[i] = (HTElement<K, T>*)SYSALLOCATOR.CAllocate(maxChainSize, sizeof(HTElement<K, T>));
		chainSizes[i] = 0;
	}
}

template<class K, class T>
inline void HashTable<K, T>::DeclareHashFunction(HashFunction hashFunc)
{
	this->hashFunc = hashFunc;
}

template<class K, class T>
inline void HashTable<K, T>::Add(const K &key, const T &value)
{
	UINT hash = hashFunc(key) % chainCount;

	unsigned chainSize = chainSizes[hash];
	if (chainSize < maxChainSize)
	{
		HTElement<K, T> &elem = chains[hash][chainSize];
		elem.Key = key;
		elem.Value = value;

		chainSizes[hash]++;

		return;
	}

	Resize();
	Add(key, value);
}

template<class K, class T>
inline bool HashTable<K, T>::Remove(const K &key, T &outValue)
{
	UINT hash = hashFunc(key) % chainCount;
	
	HTElement<K, T> *chain = chains[hash];
	UINT chainSize = chainSizes[hash];
	
	for (UINT i = 0; i < chainSize; i++)
	{
		HTElement<K, T> &elem = chain[i];

		if (elem.Key == key)
		{
			outValue = elem.Value;
				
			// shift values
			for (UINT k = i; k < chainSize - 1; k++)
			{
				chain[k].Key = chain[k + 1].Key;
				chain[k].Value = chain[k + 1].Value;
			}

			chain[chainSize - 1].~HTElement<K, T>();

			chainSizes[hash]--;

			return true;
		}
	}
	
	return false;
}

template<class K, class T>
inline bool HashTable<K, T>::Find(const K &key, T &outValue) const
{
	UINT hash = hashFunc(key) % chainCount;

	HTElement<K, T> *chain = chains[hash];
	UINT chainSize = chainSizes[hash];

	for (UINT i = 0; i < chainSize; i++)
	{
		HTElement<K, T> &elem = chain[i];

		if (elem.Key == key)
		{
			outValue = elem.Value;
			return true;
		}
	}

	return false;
}

template<class K, class T>
inline void HashTable<K, T>::Resize()
{
	UINT prevCount = chainCount;
	chainCount = (UINT)(prevCount * HASH_TABLE_INC_MULT);

	// init for larger table
	UINT *tempSizes = (UINT*)SYSALLOCATOR.Allocate(sizeof(UINT) * chainCount);

	HTElement<K, T> **temp = (HTElement<K, T>**)SYSALLOCATOR.Allocate(sizeof(HTElement<K, T>*) * chainCount);
	for (UINT i = 0; i < chainCount; i++)
	{
		// can't use malloc because there must be uninitialized keys and values 
		temp[i] = (HTElement<K, T>*)SYSALLOCATOR.CAllocate(maxChainSize, sizeof(HTElement<K, T>));
		tempSizes[i] = 0;
	}

	// save old pointers
	HTElement<K, T> **oldChains = chains;
	UINT *oldSizes = chainSizes;

	// reassign to new
	chains = temp;
	chainSizes = tempSizes;

	for (UINT i = 0; i < prevCount; i++)
	{
		UINT sourceChainSize = oldSizes[i];
		HTElement<K, T> *sourceChain = oldChains[i];

		// for each element in source chain recalculate hash key
		for (UINT j = 0; j < sourceChainSize; j++)
		{
			UINT hash = hashFunc(sourceChain[j].Key);

			// add element to new chain with recalculated hash key
			Add(sourceChain[j].Key, sourceChain[j].Value);
		}
	}

	// delete old pointers
	SYSALLOCATOR.Free(oldChains);
	SYSALLOCATOR.Free(oldSizes);

	// reassign
	chains = temp;
	chainSizes = tempSizes;
}

template<class K, class T>
inline void HashTable<K, T>::Delete()
{
	for (UINT i = 0; i < chainCount; i++)
	{
		UINT chainSize = chainSizes[i];
		chainSizes[i] = 0;

		for (UINT j = 0; j < chainSize; j++)
		{
			chains[i][j].~HTElement<K, T>();
		}
	}

	chainCount = 0;
	SYSALLOCATOR.Free(chains);
	SYSALLOCATOR.Free(chainSizes);
}