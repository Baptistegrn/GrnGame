#pragma once

#include "grngame/data/data.h"
#include "grngame/math/types.h"

typedef struct
{
    sqlite3 *db;
    DbStmt *stmt;
    int64 file_count;
    int64 asset_count;
} DataEmbeddedBd;

void create_embedded_structure(int num_dirs, const char **dirs, const char *output_header);
