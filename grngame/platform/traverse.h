#pragma once

typedef void (*FileCallback)(const char* path, void* userdata);

void WalkDir(const char* dir_path, FileCallback callback, void* userdata);