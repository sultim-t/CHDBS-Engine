#pragma once
#include "IAllocator.h"

class StackAllocator : IAllocator
{
private:
	void *startPtr = nullptr;
	UINT offset;

public:
	StackAllocator(UINT totalSize);
	~StackAllocator();

	virtual void Init() override;
	virtual void *Allocate(const UINT size) override;
	virtual void Free(void *address) override;
	void FreeAll();
};