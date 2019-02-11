#include "FreeListAllocator.h"
#include "SystemAllocator.h"

#define BLOCK_MIN_SIZE 8

FreeListAllocator::FreeListAllocator(const UINT totalSize)
{
	this->totalSize = totalSize;
	this->usedSize = 0;
}

FreeListAllocator::~FreeListAllocator()
{
	SYSALLOCATOR.Free(startPtr);
	startPtr = nullptr;
}

void FreeListAllocator::Init()
{
	ASSERT(startPtr != nullptr);
	startPtr = SYSALLOCATOR.Allocate(totalSize);

	Reset();
}

void * FreeListAllocator::Allocate(const UINT size)
{
	UINT aligned = ALIGN(size);

	Block *found, *prev;
	FindFirst(found, prev, aligned);
	ASSERT(found == nullptr);

	UINT requiredSize = aligned + sizeof(Block);
	UINT rest = found->blockSize - requiredSize;

	if (rest >= BLOCK_MIN_SIZE)
	{
		// create new block
		Block *newBlock = (Block*)((UINT)found + requiredSize);
		newBlock->blockSize = rest;

		// insert new block after found one
		Block *foundNext = (Block*)found->nextBlock;
		found->nextBlock = newBlock;
		newBlock->nextBlock = foundNext;
	}

	// remove found from list
	prev->nextBlock = found->nextBlock;

	void *data = (BYTE*)found + sizeof(Block);

	usedSize += requiredSize;
	return data;
}

void FreeListAllocator::Free(void * address)
{
	Block *toFree = (Block*)((BYTE*)address - sizeof(Block));
	
	Block *cur = listHead;
	Block *prev = nullptr;

	while (cur != nullptr)
	{
		if (toFree < cur)
		{
			break;
		}

		prev = cur;
		cur = (Block*)cur->nextBlock;
	}

	// insert freed block between
	prev->nextBlock = toFree;
	toFree = cur;

	TryMerge(prev, toFree);

	usedSize -= toFree->blockSize;
}

void FreeListAllocator::FindFirst(Block* &found, Block* &prevFound, const UINT size)
{
	Block *cur = listHead;
	Block *prev = nullptr;

	while (cur != nullptr)
	{
		if (cur->blockSize - sizeof(Block) >= size)
		{
			break;
		}

		cur = (Block*)cur->nextBlock;
	}

	found = cur;
	prevFound = prev;
}

void FreeListAllocator::TryMerge(Block * prev, Block * cur)
{
	if (cur->nextBlock != nullptr)
	{
		UINT next = (UINT)cur + cur->blockSize;
		Block *nextBlock = (Block*)cur->nextBlock;

		// try to merge cur and next
		if (next == (UINT)nextBlock)
		{
			cur->blockSize += nextBlock->blockSize;
		
			// delete next block
			cur->nextBlock = nextBlock->nextBlock;
		}
	}

	if (prev != nullptr)
	{
		UINT next = (UINT)prev + prev->blockSize;
		Block *nextBlock = (Block*)prev->nextBlock;

		// try to merge prev and cur
		if (next == (UINT)cur)
		{
			prev->blockSize += cur->blockSize;

			// delete cur block
			prev->nextBlock = cur->nextBlock;
		}
	}
}

void FreeListAllocator::Reset()
{
	usedSize = 0;

	listHead = (Block*)startPtr;
	listHead->blockSize = totalSize;
	listHead->nextBlock = nullptr;
}
