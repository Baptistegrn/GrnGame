#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef struct
{
    void* base;
    size_t size;
    size_t offset;
} BumpAllocator;

/// Creates a bump allocator that uses the memory you give it
BumpAllocator BumpAllocatorInit(void* base, size_t size);

/// Creates a bump allocator that has its own memory
BumpAllocator BumpAllocatorInitMalloc(size_t size);

/// Allocates size bytes, aligned to align
void* BumpAllocatorPushAligned(BumpAllocator* allocator, size_t size, size_t align);

/// Allocates size bytes
void* BumpAllocatorPush(BumpAllocator* allocator, size_t size);

/// Makes the entire arena reusable without freeing the underlying memory
void BumpAllocatorReset(BumpAllocator* allocator);

/// Frees the memory of a bump allocator, this is for when you create it with BumpAllocatorInitMalloc
void BumpAllocatorFree(BumpAllocator* allocator);