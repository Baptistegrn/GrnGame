#pragma once

#include "grngame/math/types.h"
#include <stdio.h>


typedef struct
{
    FILE *out;
    char *registry;
    uint64 registry_len;
} EmbedContext;

void create_embedded_structure(int num_dirs, const char **dirs, const char *output_header);
