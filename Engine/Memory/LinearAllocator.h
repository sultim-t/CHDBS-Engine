#pragma once
#include "IAllocator.h"

class LinearAllocator : IAllocator
{
private:
	void *startPtr = nullptr;
	UINT offset;

public:
	LinearAllocator(const UINT totalSize);
	~LinearAllocator();

	void Init() override;
	void *Allocate(const UINT size) override;
	// Does nothing
	void Free(void * address) override;
	// Frees all allocated memory
	void FreeAll();
};