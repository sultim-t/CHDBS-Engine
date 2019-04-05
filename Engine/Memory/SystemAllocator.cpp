#include "SystemAllocator.h"
#include <cstdlib>

SystemAllocator::SystemAllocator()
{
	allocatedSize = 0;
	allocCalls = freeCalls = reallocCalls = 0;
}

void * SystemAllocator::Allocate(UINT size)
{
	void *allocated = malloc(size);
	ASSERT(allocated != nullptr);

	Instance().allocCalls++;
	Instance().allocatedSize += size;

	return allocated;
}

void * SystemAllocator::CAllocate(UINT count, UINT size)
{
	void *allocated = calloc(count, size);
	ASSERT(allocated != nullptr);

	Instance().allocCalls++;
	Instance().allocatedSize += size * count;

	return allocated;
}

void * SystemAllocator::Reallocate(void * old, UINT oldSize, UINT newSize)
{
	void *reallocated = realloc(old, newSize);
	ASSERT(reallocated != nullptr);

	Instance().reallocCalls++;
	Instance().allocatedSize += newSize - oldSize;

	return reallocated;
}

void SystemAllocator::Free(void * address)
{
	free(address);
	Instance().freeCalls++;
}

UINT SystemAllocator::GetAllocatedSize()
{
	return allocatedSize;
}

UINT SystemAllocator::GetAllocateCallsCount()
{
	return allocCalls;
}

UINT SystemAllocator::GetReallocateCallsCount()
{
	return reallocCalls;
}

UINT SystemAllocator::GetFreeCallsCount()
{
	return freeCalls;
}

SystemAllocator &SystemAllocator::Instance()
{
	static SystemAllocator sysAllocator;
	return sysAllocator;
}
