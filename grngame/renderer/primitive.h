#pragma once

#include "../math/types.h"

void PixelDraw(float32 x, float32 y, uint8 r, uint8 g, uint8 b, uint8 a);
void LineDraw(float32 x0, float32 y0, float32 x1, float32 y1, uint8 r, uint8 g, uint8 b, uint8 a);
void RectDraw(float32 x, float32 y, float32 w, float32 h, uint8 r, uint8 g, uint8 b, uint8 a);
void RectDrawFill(float32 x, float32 y, float32 w, float32 h, uint8 r, uint8 g, uint8 b, uint8 a);
void CircleDraw(float32 xc, float32 yc, float32 radius, uint8 r, uint8 g, uint8 b, uint8 a);
void CircleDrawFill(float32 xc, float32 yc, float32 radius, uint8 r, uint8 g, uint8 b, uint8 a);
