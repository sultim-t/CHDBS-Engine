#include "MemoryPool.h"
#include "SystemAllocator.h"

MemoryPool::MemoryPool(const UINT totalSize, const UINT blockSize)
{
	ASSERT(totalSize % blockSize == 0);

	this->totalSize = totalSize;
	this->blockSize = blockSize;
	this->usedSize = 0;
}

MemoryPool::~MemoryPool()
{
	if (startPtr != nullptr)
	{
		SYSALLOCATOR.Free(startPtr);
	}
}

void MemoryPool::Init()
{
	ASSERT(startPtr != nullptr);

	startPtr = SYSALLOCATOR.Allocate(totalSize);
	Reset();
}

void *MemoryPool::Allocate(const UINT size)
{
	ASSERT(size <= blockSize);

	// pop from free stack
	void *block = freeList.Pop();

	ASSERT(block != nullptr);

	usedSize += blockSize;
	return block;
}

void MemoryPool::Free(void *address)
{
	// just push to free stack
	freeList.Push((Node*)address);
	usedSize -= blockSize;
}

void MemoryPool::Reset()
{
	usedSize = 0;
	const UINT blocksCount = totalSize / blockSize;

	for (UINT i = 0; i < blocksCount; i++)
	{
		UINT block = (UINT)startPtr + i * blockSize;
		freeList.Push((Node*)block);
	}
}
