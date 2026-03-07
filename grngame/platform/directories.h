#pragma once

typedef void (*FileCallback)(const char* path, void* userdata);

/// Recursively traverses a directory, calling your callback on every regular file, with the given userdata
void DirWalk(const char* dir_path, FileCallback callback, void* userdata);

/// Counts number of regular files in a directory
int DirFileCount(const char* dir_path);