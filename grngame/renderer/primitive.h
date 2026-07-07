#pragma once

#include "../math/types.h"

#define COLOR_DEFAULT_PRIMITIVE_PALETTE_EMPTY 0, 255, 0

void PixelDraw(float32 x, float32 y, int32 c_idx, uint8 a);
void LineDraw(float32 x0, float32 y0, float32 x1, float32 y1, int32 c_idx, uint8 a);
void RectDraw(float32 x, float32 y, float32 w, float32 h, int32 c_idx, uint8 a);
void RectDrawFill(float32 x, float32 y, float32 w, float32 h, int32 c_idx, uint8 a);
void CircleDraw(float32 xc, float32 yc, float32 radius, int32 c_idx, uint8 a);
void CircleDrawFill(float32 xc, float32 yc, float32 radius, int32 c_idx, uint8 a);