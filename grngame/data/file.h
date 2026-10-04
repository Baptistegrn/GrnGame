#pragma once
#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

#define MAX_PATH_ 1024

BEGIN_DECLARATIONS

bool FileExist(const char *name);
unsigned char *ReturnFileString(const char *name, uint64 *size_out);
bool WriteFileString(const char *name, const char *content, bool append);

END_DECLARATIONS
