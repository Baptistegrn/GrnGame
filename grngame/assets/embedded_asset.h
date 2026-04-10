#pragma once

#include <stdio.h>

typedef struct EmbedContext
{
    FILE *out;
    char *registry;
} EmbedContext;

void create_embedded_structure(int num_dirs, const char **dirs, const char *output_header);
