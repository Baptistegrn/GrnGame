#include "virtual_memory.h"

#ifdef GRNGAME_WINDOWS
#include <Windows.h>

void *VirtualMemoryReserve(uint64 size)
{
    void *ptr = VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_NOACCESS);
    return ptr;
}

bool VirtualMemoryCommit(void *ptr, uint64 size)
{
    void *result = VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
    return result != NULL;
}

bool VirtualMemoryDecommit(void *ptr, uint64 size)
{
    return VirtualFree(ptr, size, MEM_DECOMMIT);
}

bool VirtualMemoryRelease(void *ptr, uint64 size)
{
    return VirtualFree(ptr, 0, MEM_RELEASE);
}

uint64 VirtualMemoryPageSize()
{
    static uint64 page_size = 0;
    if (page_size == 0)
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        page_size = si.dwPageSize;
    }
    return page_size;
}

#elif defined(GRNGAME_LINUX) || defined(GRNGAME_MACOS)
#include <sys/mman.h>
#include <unistd.h>

void *VirtualMemoryReserve(uint64 size)
{
    void *ptr = mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return ptr;
}

bool VirtualMemoryCommit(void *ptr, uint64 size)
{
    return mprotect(ptr, size, PROT_READ | PROT_WRITE) == 0;
}

bool VirtualMemoryDecommit(void *ptr, uint64 size)
{
    mprotect(ptr, size, PROT_NONE);
    madvise(ptr, size, MADV_FREE);
    return true;
}

bool VirtualMemoryRelease(void *ptr, uint64 size)
{
    return munmap(ptr, size) == 0;
}

uint64 VirtualMemoryPageSize(void)
{
    static uint64 page_size = 0;
    if (page_size == 0)
        page_size = sysconf(_SC_PAGESIZE);
    return page_size;
}

#endif
