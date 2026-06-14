#include "embedded_asset.h"

#include "grngame/platform/directories.h"
#include "grngame/platform/paths.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grngame/data/data.h"

static int IsEmbeddableFile(const char *path)
{
    return FileIsLoadableScript(path) || FileIsLoadableAudio(path) || FileIsLoadableImage(path) ||
           FileIsLoadableText(path);
}

static unsigned char *LoadFileBinary(const char *path, long *out_size)
{
    FILE *file = fopen(path, "rb");
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    *out_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (*out_size <= 0)
    {
        fclose(file);
        return NULL;
    }

    unsigned char *data = malloc(*out_size);
    if (!data)
    {
        fclose(file);
        return NULL;
    }

    fread(data, 1, *out_size, file);
    fclose(file);
    return data;
}

static void EmbedCallback(const char *path, void *userdata)
{
    DataEmbeddedBd *info = (DataEmbeddedBd *)userdata;

    if (!IsEmbeddableFile(path))
        return;

    if (FileIsLoadableAudio(path) || FileIsLoadableImage(path))
        info->asset_count++;
    info->file_count++;

    long size;
    unsigned char *data = LoadFileBinary(path, &size);
    if (!data)
    {
        fprintf(stderr, "[EmbeddedAsset] Failed to load file: %s\n", path);
        return;
    }

    DbArg args[2];
    args[0].type = TEXT;
    args[0].value.s = path;
    args[1].type = DATA;
    args[1].value.blob.data = data;
    args[1].value.blob.size = size;
    DbStmtRun(info->stmt, args, 2);

    free(data);
}

void create_embedded_structure(int num_dirs, const char **dirs, const char *output_header)
{
    if (DbExists(output_header))
        remove(output_header);

    sqlite3 *db = DbCreate(output_header);
    if (!db)
    {
        fprintf(stderr, "[EmbeddedAsset] Failed to create database: %s\n", output_header);
        return;
    }

    DataWrite(db, "CREATE TABLE IF NOT EXISTS embedded_assets "
                  "(path TEXT PRIMARY KEY, data BLOB);");
    DataWrite(db, "CREATE TABLE IF NOT EXISTS embedded_assets_info "
                  "(key TEXT PRIMARY KEY, value INTEGER);");

    DataWrite(db, "PRAGMA journal_mode = OFF;");    /*skip fsync*/
    DataWrite(db, "PRAGMA synchronous  = OFF;");    /* no os confirmation disk*/
    DataWrite(db, "PRAGMA cache_size   = -65536;"); /*bigger cache*/

    DbStmt stmt = DbStmtPrepare(db, "INSERT INTO embedded_assets (path, data) VALUES (?, ?);");

    DataEmbeddedBd info = {db, &stmt, 0, 0};

    DbBegin(db);
    for (int i = 0; i < num_dirs; ++i)
        DirWalk(dirs[i], EmbedCallback, &info);

    char sql_buffer[256];
    snprintf(sql_buffer, sizeof(sql_buffer),
             "INSERT INTO embedded_assets_info (key, value) VALUES ('file_count', %lld);", info.file_count);
    DataWrite(db, sql_buffer);

    snprintf(sql_buffer, sizeof(sql_buffer),
             "INSERT INTO embedded_assets_info (key, value) VALUES ('asset_count', %lld);", info.asset_count);
    DataWrite(db, sql_buffer);

    DbCommit(db);
    DbStmtFree(&stmt);
    DbClose(db);
}