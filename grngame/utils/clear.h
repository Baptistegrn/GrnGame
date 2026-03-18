#pragma once

#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define CLEAR_PTR(x) memset((x), 0, sizeof(*(x)))

void ClearAll();