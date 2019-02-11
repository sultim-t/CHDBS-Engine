#pragma once

#define ALIGNMENT sizeof(INT)
#define ALIGN(size) (size) % ALIGNMENT == 0 ? (size) : (size + ALIGNMENT - ((size) % ALIGNMENT))

// Shortcut
#define SYSALLOCATOR SystemAllocator::Instance()

class SystemAllocator
{
private:
	SystemAllocator();
	~SystemAllocator();

	void operator=(const SystemAllocator&) = delete;

	UINT allocatedSize;
	UINT allocCalls, freeCalls, reallocCalls;

public:
	void *Allocate(UINT size);
	void *Reallocate(void *old, UINT oldSize, UINT newSize);
	void Free(void *address);

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