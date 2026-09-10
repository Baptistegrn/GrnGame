#pragma once

#define CLEAR(x, y) memset(&(x), y, sizeof(x))
#define CLEAR_PTR(x, y) memset((x), y, sizeof(*(x)))
#define CLEAR_ARRAY(x, y, count) memset((x), (y), sizeof(*(x)) * (count))

void ClearAll();