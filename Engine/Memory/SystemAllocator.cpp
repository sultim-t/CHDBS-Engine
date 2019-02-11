#include "SystemAllocator.h"
#include <cstdlib>
#include <cstring>

SystemAllocator::SystemAllocator()
{
	allocatedSize = 0;
	allocCalls = freeCalls = reallocCalls = 0;
}

SystemAllocator::~SystemAllocator()
{ 
	if (allocCalls - freeCalls > 0)
	{
		Logger::Print("Memory leak");
	}
}

void * SystemAllocator::Allocate(UINT size)
{
	void *allocated = malloc(size);
	ASSERT(allocated != nullptr);

	allocCalls++;
	allocatedSize += size;

	return allocated;
}

void * SystemAllocator::Reallocate(void * old, UINT oldSize, UINT newSize)
{
	void *reallocated = realloc(old, newSize);
	ASSERT(reallocated != nullptr);

	reallocCalls++;
	allocatedSize += newSize - oldSize;

	return reallocated;
}

void SystemAllocator::Free(void * address)
{
	free(address);
	freeCalls++;
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

SystemAllocator & SystemAllocator::Instance()
{
	static SystemAllocator sysAllocator;
	return sysAllocator;
}
