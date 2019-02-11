#include "LinearAllocator.h"
#include "SystemAllocator.h"

LinearAllocator::LinearAllocator(const UINT totalSize)
{ 
	this->totalSize = totalSize;
	this->usedSize = 0;
}

LinearAllocator::~LinearAllocator()
{
	FreeAll();
}

void LinearAllocator::Init()
{
	ASSERT(startPtr != nullptr);

	startPtr = SYSALLOCATOR.Allocate(totalSize);
	offset = 0;
}

void * LinearAllocator::Allocate(const UINT size)
{
	UINT alignedSize = ALIGN(size);
	ASSERT(offset + alignedSize < totalSize);

	void *allocated = (BYTE*)startPtr + offset;
	offset += alignedSize;

	usedSize = offset;

	return allocated;
}

void LinearAllocator::Free(void * address)
{
	ASSERT(false);
}

void LinearAllocator::FreeAll()
{
	ASSERT(startPtr != nullptr);

	SYSALLOCATOR.Free(startPtr);
	startPtr = nullptr;
	usedSize = 0;
	totalSize = 0;
}