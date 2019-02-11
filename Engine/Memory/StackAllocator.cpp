#include "StackAllocator.h"
#include "SystemAllocator.h"

StackAllocator::StackAllocator(UINT totalSize)
{
	this->totalSize = totalSize;
	this->usedSize = usedSize;
}

StackAllocator::~StackAllocator()
{
	SYSALLOCATOR.Free(startPtr);
	startPtr = nullptr;
}

void StackAllocator::Init()
{
	ASSERT(startPtr != nullptr);

	startPtr = SYSALLOCATOR.Allocate(totalSize);
	offset = 0;
}

void *StackAllocator::Allocate(const UINT size)
{
	UINT alignedSize = ALIGN(size); // to store block size
	ASSERT(offset + alignedSize < totalSize);
	
	void *allocated = (BYTE*)startPtr + offset;
	offset += alignedSize;

	usedSize = offset;
	return allocated;
}

void StackAllocator::Free(void *address)
{
	offset = (UINT)address - (UINT)startPtr;
	usedSize = offset;
}

void StackAllocator::FreeAll()
{
	offset = 0;
	usedSize = 0;
}
