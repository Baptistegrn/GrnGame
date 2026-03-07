#pragma once
#include <stdbool.h>

// Returns a malloc'd string that is the filename stem of the path
// If you give it assets/boss.wav, it returns boss
// Remember to free
char* FileStem(const char* path);

const char* FileExtension(const char* path);

/// Returns true if the file is an image that COULD be loaded with SDL_Image
bool FileIsLoadableImage(const char* file);

/// Returns true if the file is an image that COULd be loaded with Soloud's Wav_stream
bool FileIsLoadableAudio(const char* file);