#pragma once

// Returns a malloc'd string that is the filename stem of the path
// If you give it assets/boss.wav, it returns boss
// Remember to free
char* FileStem(const char* path);

const char* FileExtension(const char* path);