#pragma once

#ifdef __cplusplus
#define BEGIN_DECLARATIONS                                                                                             \
    extern "C"                                                                                                         \
    {
#define END_DECLARATIONS }
#else
#define BEGIN_DECLARATIONS
#define END_DECLARATIONS
#endif