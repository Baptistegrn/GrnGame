#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "grngame/math/types.h"

typedef struct
{
    void *base;
    uint64 size;      // total reserved size
    uint64 committed; // how much is currently committed
    uint64 offset;    // how much is currently used
} BumpAllocator;

/// Creates a bump allocator with the given virtual size reserved
/// (you can ask it for 1TB on a 8GB of RAM machine)
BumpAllocator BumpAllocatorInit(uint64 virtual_size);

/// Allocates size bytes, aligned to align
void *BumpAllocatorPushAligned(BumpAllocator *allocator, uint64 size, uint64 align);

/// Allocates size bytes
void *BumpAllocatorPush(BumpAllocator *allocator, uint64 size);

/// Makes the entire arena reusable without freeing the underlying memory
void BumpAllocatorReset(BumpAllocator *allocator);

/// Releases the virtual memory
void BumpAllocatorFree(BumpAllocator *allocator);

/// Gets the head of the bump allocator (base + offset)
void *BumpAllocatorHead(BumpAllocator *allocator);

/// Manually increments offset, returning false if we overran
bool BumpAllocatorIncrement(BumpAllocator *allocator, uint64 size, uint64 align);