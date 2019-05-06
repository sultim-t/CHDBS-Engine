#include "SystemAllocator.h"
#include <cstdlib>
#include <cstdio>

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
	
	//printf("Allocate: %d KB. Total: %d KB\n", size / 1024, Instance().allocatedSize / 1024);

	return allocated;
}

void * SystemAllocator::CAllocate(UINT count, UINT size)
{
	void *allocated = calloc(count, size);
	ASSERT(allocated != nullptr);

	Instance().allocCalls++;
	Instance().allocatedSize += size * count;

	//printf("CAllocate: %d KB. Total: %d KB\n", count * size / 1024, Instance().allocatedSize / 1024);

	return allocated;
}

void * SystemAllocator::Reallocate(void * old, UINT oldSize, UINT newSize)
{
	void *reallocated = realloc(old, newSize);
	ASSERT(reallocated != nullptr);

	Instance().reallocCalls++;
	Instance().allocatedSize += newSize - oldSize;

	//printf("Reallocate: %d KB. Total: %d KB\n", (newSize - oldSize) / 1024, Instance().allocatedSize / 1024);

	return reallocated;
}

void SystemAllocator::Free(void * address)
{
	free(address);
	Instance().freeCalls++;

	//printf("Freed %d times\n", Instance().freeCalls);
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
