#pragma once

#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"


BEGIN_DECLARATIONS

typedef void (*FileCallback)(const char *path, void *userdata);

/// Recursively traverses a directory, calling your callback on every regular file, with the given userdata
void DirWalk(const char *dir_path, FileCallback callback, void *userdata);

/// Counts number of regular files in a directory
int32 DirFileCount(const char *dir_path);

/// Counts number of asset files in a directory
int32 DirAssetFileCount(const char *dir_path);

/// Directory of the executable. Can be null but we may not want to handle it.
const char *DirOfExecutable();

END_DECLARATIONS