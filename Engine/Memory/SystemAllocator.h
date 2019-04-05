#pragma once

#define ALIGNMENT sizeof(INT)
#define ALIGN(size) (size) % ALIGNMENT == 0 ? (size) : (size + ALIGNMENT - ((size) % ALIGNMENT))

// Shortcut
#define SYSALLOCATOR SystemAllocator::Instance()

class SystemAllocator
{
private:
	SystemAllocator();

	void operator=(const SystemAllocator&) = delete;

	UINT allocatedSize;
	UINT allocCalls, freeCalls, reallocCalls;

public:
	static void *Allocate(UINT size);
	static void *CAllocate(UINT count, UINT size);
	static void *Reallocate(void *old, UINT oldSize, UINT newSize);
	static void Free(void *address);

	// Get size of all allocated memory (without freed)
	UINT GetAllocatedSize();
	// Count allocation calls
	UINT GetAllocateCallsCount();
	// Count reallocation calls
	UINT GetReallocateCallsCount();
	// Count freeing calls
	UINT GetFreeCallsCount();

	static SystemAllocator &Instance();
};