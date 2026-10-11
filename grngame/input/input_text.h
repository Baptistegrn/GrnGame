#pragma once

#include "grngame/math/types.h"
#include "grngame/utils/memory.h"

#define DROP_FILE_PATH_MAX 1024 * BUFFER_SPACE

const char *InputTextGet();
void InputTextClear();
const char *InputDropFile();
void InputTextPutText(const char *text);
void InputTextRemoveTextInt(uint32 size);
void InputTextRemoveTextChar(const char *pattern);
void InputTextReplace(const char *pattern, const char *replace);
