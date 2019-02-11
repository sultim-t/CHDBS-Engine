#pragma once
#include "IAllocator.h"

class FreeListAllocator : IAllocator
{
private:
	struct Block
	{
		// Size for header and data
		UINT blockSize;
		void *nextBlock;
		// BYTE padding; -- doesnt need padding because allocated size is aligned
	};

	void *startPtr = nullptr;
	Block *listHead;

private:
	void FindFirst(Block* &found, Block* &prev, const UINT size);
	void TryMerge(Block* prev, Block *cur);
	void Reset();

public:
	FreeListAllocator(const UINT totalSize);
	~FreeListAllocator();

	virtual void Init() override;
	virtual void *Allocate(const UINT size) override;
	virtual void Free(void * address) override;
};