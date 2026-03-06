#pragma once
#include <stdbool.h>
#include <stddef.h>

/// reserves a contiguous virtual address range without allocating RAM
void* VirtualMemoryReserve(size_t size);

/// commits pages in [ptr, ptr+size[, backing them with real RAM
bool VirtualMemoryCommit(void* ptr, size_t size);

/// decommits pages, freeing RAM but keeping the address range reserved
bool VirtualMemoryDecommit(void* ptr, size_t size);

/// releases the entire reserved range, ptr must be the original reserve base
bool VirtualMemoryRelease(void* ptr, size_t size);
