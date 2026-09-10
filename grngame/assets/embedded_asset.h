#pragma once

#include "grngame/data/data.h"
#include "grngame/math/types.h"

typedef struct
{
    sqlite3 *db;
    DbStmt *stmt;
    uint64 file_count;
    uint64 asset_count;
} DataEmbeddedBd;

void create_embedded_structure(int32 num_dirs, const char **dirs, const char *output_header);
