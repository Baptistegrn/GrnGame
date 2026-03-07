#include "bump_allocator.h"

#include <stdlib.h>

BumpAllocator BumpAllocatorInit(void* base, size_t size)
{
    return (BumpAllocator){ .base = base, .size = size, .offset = 0 };
}

BumpAllocator BumpAllocatorInitMalloc(size_t size)
{
    return (BumpAllocator){ .base = malloc(size), .size = size, .offset = 0 };
}

void* BumpAllocatorPushAligned(BumpAllocator* allocator, size_t size, size_t align)
{
    // round offset to the next multiple of align because it works better with the cpu
    // of arnaud virazel
    size_t aligned = (allocator->offset + (align - 1)) & ~(align - 1);
    if(aligned + size > allocator->size)
    {
        return NULL;
    }
    allocator->offset = aligned + size;
    return (char *)allocator->base + aligned;
}

void* BumpAllocatorPush(BumpAllocator* allocator, size_t size)
{
    return BumpAllocatorPushAligned(allocator, size, 8);
}

void BumpAllocatorReset(BumpAllocator* allocator)
{
    allocator->offset = 0;
}

void BumpAllocatorFree(BumpAllocator* allocator)
{
    free(allocator->base);
    allocator->base = NULL;
}

void *BumpAllocatorHead(BumpAllocator *allocator)
{
    return allocator->base + allocator->offset;
}

bool BumpAllocatorIncrement(BumpAllocator* allocator, size_t size, size_t align)
{
    size_t aligned = (allocator->offset + (align - 1)) & ~(align - 1);
    if (aligned + size > allocator->size)
        return false;
    allocator->offset = aligned + size;
    return true;
}