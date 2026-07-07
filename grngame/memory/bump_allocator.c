#include "bump_allocator.h"
#include "grngame/platform/virtual_memory.h"
#include "grngame/math/types.h"

static uint64 RoundUpToPage(uint64 size);
static bool EnsureCommitted(BumpAllocator *allocator, uint64 needed);

BumpAllocator BumpAllocatorInit(uint64 virtual_size)
{
    void *base = VirtualMemoryReserve(virtual_size);
    return (BumpAllocator){
        .base = base,
        .size = virtual_size,
        .committed = 0,
        .offset = 0,
    };
}

void *BumpAllocatorPushAligned(BumpAllocator *allocator, uint64 size, uint64 align)
{
    uint64 aligned = (allocator->offset + (align - 1)) & ~(align - 1);
    if (!EnsureCommitted(allocator, aligned + size))
        return NULL;

    allocator->offset = aligned + size;
    return (char *)allocator->base + aligned;
}

void *BumpAllocatorPush(BumpAllocator *allocator, uint64 size)
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

bool BumpAllocatorIncrement(BumpAllocator *allocator, uint64 size, uint64 align)
{
    uint64 aligned = (allocator->offset + (align - 1)) & ~(align - 1);
    if (!EnsureCommitted(allocator, aligned + size))
        return false;

    allocator->offset = aligned + size;
    return true;
}

static uint64 RoundUpToPage(uint64 size)
{
    return (size + VirtualMemoryPageSize() - 1) & ~(VirtualMemoryPageSize() - 1);
}

static bool EnsureCommitted(BumpAllocator *allocator, uint64 needed)
{
    if (needed <= allocator->committed)
        return true;
    if (needed > allocator->size)
        return false;

    uint64 to_commit = RoundUpToPage(needed - allocator->committed);
    void *ptr = (char *)allocator->base + allocator->committed;
    if (!VirtualMemoryCommit(ptr, to_commit))
        return false;

    allocator->committed += to_commit;
    return true;
}