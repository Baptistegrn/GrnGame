#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "grngame/math/types.h"

/// reserves a contiguous virtual address range without allocating RAM
void *VirtualMemoryReserve(uint64 size);

/// commits pages in [ptr, ptr+size[, backing them with real RAM
bool VirtualMemoryCommit(void *ptr, uint64 size);

/// decommits pages, freeing RAM but keeping the address range reserved
bool VirtualMemoryDecommit(void *ptr, uint64 size);

/// releases the entire reserved range, ptr must be the original reserve base
bool VirtualMemoryRelease(void *ptr, uint64 size);

/// Page size
uint64 VirtualMemoryPageSize();