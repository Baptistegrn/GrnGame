#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef struct
{
    void*  base;
    size_t size;       // total reserved size
    size_t committed;  // how much is currently committed
    size_t offset;     // how much is currently used
} BumpAllocator;

/// Creates a bump allocator with the given virtual size reserved
/// (you can ask it for 1TB on a 8GB of RAM machine)
BumpAllocator BumpAllocatorInit(size_t virtual_size);

/// Allocates size bytes, aligned to align
void* BumpAllocatorPushAligned(BumpAllocator* allocator, size_t size, size_t align);

/// Allocates size bytes
void* BumpAllocatorPush(BumpAllocator* allocator, size_t size);

/// Makes the entire arena reusable without freeing the underlying memory
void BumpAllocatorReset(BumpAllocator* allocator);

/// Releases the virtual memory
void BumpAllocatorFree(BumpAllocator* allocator);

/// Gets the head of the bump allocator (base + offset)
void* BumpAllocatorHead(BumpAllocator* allocator);

/// Manually increments offset, returning false if we overran
bool BumpAllocatorIncrement(BumpAllocator* allocator, size_t size, size_t align);