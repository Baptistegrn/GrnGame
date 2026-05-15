#pragma once

#include <stdio.h>

typedef struct
{
    FILE *out;
    char *registry;
    size_t registry_len;
} EmbedContext;

void create_embedded_structure(int num_dirs, const char **dirs, const char *output_header);
