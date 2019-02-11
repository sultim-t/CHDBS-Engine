#pragma once
#include "IAllocator.h"
#include <Engine/DataStructures/StackManual.h>

class MemoryPool : IAllocator
{
private:
	struct Empty { };

private:
	void *startPtr = nullptr;
	UINT blockSize;

	StackManual<Empty> freeList;

	// shortcut
	typedef StackManual<Empty>::SMNode Node;

public:
	MemoryPool(const UINT totalSize, const UINT blockSize);
	~MemoryPool();

	virtual void Init() override;
	virtual void *Allocate(const UINT size) override;
	virtual void Free(void * address) override;
	void Reset();
};