#pragma once

#include "grngame/math/types.h"
#include <SDL3/SDL.h>

void RemoveTaskBarIconColor();

void SetTaskBarIconProgress(float32 progress);

void SetTaskBarIconPausedProgress(float32 progress);

void SetTaskBarIconErrorProgress(float32 progress);

void SetTaskBarIconIndeterminate();

SDL_ProgressState GetTaskBarIconState();

float32 GetTaskBarIconProgress();