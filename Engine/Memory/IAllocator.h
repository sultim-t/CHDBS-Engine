#pragma once

class IAllocator
{
protected:
	UINT totalSize;
	UINT usedSize;

public:
	virtual void Init() = 0;
	virtual void *Allocate(const UINT size) = 0;
	virtual void Free(void *address) = 0;
};