#include "bump_allocator.h"
#include "grngame/platform/virtual_memory.h"

static size_t RoundUpToPage(size_t size);
static bool EnsureCommitted(BumpAllocator *allocator, size_t needed);

BumpAllocator BumpAllocatorInit(size_t virtual_size)
{
    void *base = VirtualMemoryReserve(virtual_size);
    return (BumpAllocator){
        .base = base,
        .size = virtual_size,
        .committed = 0,
        .offset = 0,
    };
}

void *BumpAllocatorPushAligned(BumpAllocator *allocator, size_t size, size_t align)
{
    size_t aligned = (allocator->offset + (align - 1)) & ~(align - 1);
    if (!EnsureCommitted(allocator, aligned + size))
        return NULL;

    allocator->offset = aligned + size;
    return (char *)allocator->base + aligned;
}

void *BumpAllocatorPush(BumpAllocator *allocator, size_t size)
{
    return BumpAllocatorPushAligned(allocator, size, 8);
}

void BumpAllocatorReset(BumpAllocator *allocator)
{
    allocator->offset = 0; // no need to decommit
}

void BumpAllocatorFree(BumpAllocator *allocator)
{
    VirtualMemoryRelease(allocator->base, allocator->size);
    allocator->base = NULL;
    allocator->committed = 0;
    allocator->offset = 0;
}

void *BumpAllocatorHead(BumpAllocator *allocator)
{
    return (char *)allocator->base + allocator->offset;
}

bool BumpAllocatorIncrement(BumpAllocator *allocator, size_t size, size_t align)
{
    size_t aligned = (allocator->offset + (align - 1)) & ~(align - 1);
    if (!EnsureCommitted(allocator, aligned + size))
        return false;

    allocator->offset = aligned + size;
    return true;
}

static size_t RoundUpToPage(size_t size)
{
    return (size + VirtualMemoryPageSize() - 1) & ~(VirtualMemoryPageSize() - 1);
}

static bool EnsureCommitted(BumpAllocator *allocator, size_t needed)
{
    if (needed <= allocator->committed)
        return true;
    if (needed > allocator->size)
        return false;

    size_t to_commit = RoundUpToPage(needed - allocator->committed);
    void *ptr = (char *)allocator->base + allocator->committed;
    if (!VirtualMemoryCommit(ptr, to_commit))
        return false;

    allocator->committed += to_commit;
    return true;
}