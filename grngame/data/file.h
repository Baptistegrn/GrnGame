#pragma once
// todo : make the same system of json
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

bool FileExist(const char *name);
char *ReturnFileString(const char *name);
bool WriteFileString(const char *name, const char *content, bool append);

END_DECLARATIONS
