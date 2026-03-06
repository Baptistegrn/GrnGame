#pragma once

typedef void (*FileCallback)(const char* path, void* userdata);

/// Recursively traverses a directory, calling your callback on every file, with the given userdata
void WalkDir(const char* dir_path, FileCallback callback, void* userdata);