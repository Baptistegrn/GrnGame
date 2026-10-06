#include "embedded_file_manager.h"
#include "grngame/core/app.h"
#include "grngame/data/data.h"
#include "grngame/dev/logging.h"
#include "grngame/embedded/embedded_file_generator.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/clear.h"
#include "grngame/utils/string_compat.h"
#include "khash.h"
#include "kvec.h"
#include <stdlib.h>
#include <string.h>

const char *EmbedKindTable(EmbedKind kind)
{
    return EMBED_TABLE_NAMES[kind];
}

static void EmbeddedFileFree(EmbeddedFile *file)
{
    if (!file)
        return;

    free((char *)file->name);
    free(file->data);
    free(file);
}

/* put an embedded file in memory */
static void HashPutOwned(EmbeddedFile *File)
{
    EmbeddedFileManager *mgr = &g_app.embedded_file_manager;
    int ret;
    khiter_t k = kh_put(EmbeddedFileHash, mgr->hash, File->name, &ret);

    if (ret == 0) /* replace if key already exist */
    {
        EmbeddedFile *old = kh_value(mgr->hash, k);
        // need to update numbers of files
        mgr->counts[old->kind]--;
        mgr->total_count--;

        EmbeddedFileFree(old);
    }

    kh_key(mgr->hash, k) = File->name;
    kh_value(mgr->hash, k) = File;

    mgr->counts[File->kind]++;
    mgr->total_count++;
}

/* remove a file from memory */
static bool HashRemove(const char *path)
{
    EmbeddedFileManager *mgr = &g_app.embedded_file_manager;
    khiter_t k = kh_get(EmbeddedFileHash, mgr->hash, path);
    if (k == kh_end(mgr->hash))
        return false;

    EmbeddedFile *File = kh_value(mgr->hash, k);

    mgr->counts[File->kind]--;
    mgr->total_count--;

    kh_del(EmbeddedFileHash, mgr->hash, k);
    EmbeddedFileFree(File);
    return true;
}

COLD EmbeddedFileManager EmbeddedFileManagerCreate(void)
{
    EmbeddedFileManager manager = {0};
    manager.hash = kh_init(EmbeddedFileHash);
    return manager;
}

COLD void EmbeddedFileManagerDestroy(EmbeddedFileManager *manager)
{
    for (khiter_t k = kh_begin(manager->hash); k != kh_end(manager->hash); ++k)
    {
        if (kh_exist(manager->hash, k))
        {
            EmbeddedFile *file = kh_value(manager->hash, k);
            EmbeddedFileFree(file);
        }
    }

    kh_destroy(EmbeddedFileHash, manager->hash);
    manager->hash = NULL;

    CLEAR(manager->counts, 0);
    manager->total_count = 0;
}

static bool LoadTable(EmbedKind kind)
{
    sqlite3 *db = g_app.info.file_db;
    char sql[128];
    snprintf(sql, sizeof(sql), "SELECT path, content FROM %s;", EmbedKindTable(kind));

    DbResult res = DataFetch(db, sql);

    for (uint64 i = 0; i < kv_size(res.rows); ++i)
    {
        DbRow *row = &kv_A(res.rows, i);

        if (UNLIKELY(kv_size(row->cols) < 2))
        {
            DbResultFree(&res);
            return false;
        }

        DbValue *path = &kv_A(row->cols, 0);
        DbValue *content = &kv_A(row->cols, 1);

        if (UNLIKELY(path->type != TEXT || content->type != DATA))
        {
            LOG_ERROR("Assets.pak: invalid structure '%s'", EmbedKindTable(kind));
            DbResultFree(&res);
            return false;
        }

        EmbeddedFile *file = malloc(sizeof(*file));
        file->name = strdup(path->value.s);
        file->data = malloc(content->value.blob.size);
        file->size = content->value.blob.size;
        file->kind = kind;
        memcpy(file->data, content->value.blob.data, content->value.blob.size);
        HashPutOwned(file);
    }

    DbResultFree(&res);
    return true;
}

static bool EmbeddedTableExists(const char *name)
{
    sqlite3 *db = g_app.info.file_db;
    char sql[256];
    snprintf(sql, sizeof(sql),
             "SELECT 1 FROM sqlite_master "
             "WHERE type = 'table' AND name = '%s' LIMIT 1;",
             name);

    DbResult res = DataFetch(db, sql);
    bool exists = kv_size(res.rows) > 0;

    DbResultFree(&res);
    return exists;
}

bool EmbeddedDbHasValidSchema()
{
    for (int32 i = 0; i < EMBED_KIND_COUNT; ++i)
    {
        if (!EmbeddedTableExists(EmbedKindTable(i)))
        {
            LOG_ERROR("Assets.pak: missing table '%s'", EmbedKindTable(i));
            return false;
        }
    }

    return true;
}

bool EmbeddedFileManagerLoadDb()
{
    if (!EmbeddedDbHasValidSchema())
    {
        LOG_ERROR("%s", "Assets.pak structure isn't correct");
        return false;
    }

    for (int32 i = 0; i < EMBED_KIND_COUNT; ++i)
    {
        if (!LoadTable((EmbedKind)i))
            return false;
    }

    return true;
}

const EmbeddedFile *EmbeddedFileGet(const char *path)
{
    EmbeddedFileManager *mgr = &g_app.embedded_file_manager;

    khiter_t k = kh_get(EmbeddedFileHash, mgr->hash, path);
    return (k == kh_end(mgr->hash)) ? NULL : kh_value(mgr->hash, k);
}

bool EmbeddedFileExists(const char *path)
{
    return EmbeddedFileGet(path) != NULL;
}

uint64 EmbeddedFileManagerGetCounts(EmbedKind kind)
{
    return g_app.embedded_file_manager.counts[kind];
}

uint64 EmbeddedFileManagerGetTotalCount()
{
    return g_app.embedded_file_manager.total_count;
}

static bool EmbeddedDbWrite(EmbedKind kind, const char *path, const void *data, uint64 size)
{
    sqlite3 *db = g_app.info.file_db;

    char sql[255];
    snprintf(sql, sizeof(sql),
             "INSERT INTO %s (path, content) VALUES (?, ?) "
             "ON CONFLICT(path) DO UPDATE SET content = excluded.content;",
             EmbedKindTable(kind));

    DbStmt stmt = DbStmtPrepare(db, sql);

    DbArg args[2] = {
        {
            .name = NULL,
            .type = TEXT,
            .value.s = path,
        },
        {
            .name = NULL,
            .type = DATA,
            .value.blob =
                {
                    .data = (void *)data,
                    .size = size,
                },
        },
    };

    bool ok = DbStmtRun(&stmt, args, 2);
    DbStmtFree(&stmt);

    return ok;
}

static bool EmbeddedDbDelete(EmbedKind kind, const char *path)
{
    sqlite3 *db = g_app.info.file_db;

    char sql[128];
    snprintf(sql, sizeof(sql), "DELETE FROM %s WHERE path = ?;", EmbedKindTable(kind));

    DbStmt stmt = DbStmtPrepare(db, sql);

    DbArg arg;
    arg.type = TEXT;
    arg.value.s = path;

    bool ok = DbStmtRun(&stmt, &arg, 1);
    DbStmtFree(&stmt);
    return ok;
}

// to put only in memory
void EmbeddedFilePutMemory(EmbedKind kind, const char *path, const void *data, uint64 size)
{

    EmbeddedFile *file = malloc(sizeof(*file));
    file->name = strdup(path);
    file->data = malloc(size);
    file->size = size;
    file->kind = kind;
    memcpy(file->data, data, size);
    HashPutOwned(file);
}

// to put only in disk ( sqlite)
bool EmbeddedFilePutDisk(EmbedKind kind, const char *path, const void *data, uint64 size)
{
    /*if file already exist we destroy it */
    const EmbeddedFile *old = EmbeddedFileGet(path);
    if (old && old->kind != kind)
    {
        if (!EmbeddedDbDelete(old->kind, path))
            return false;
    }

    if (!EmbeddedDbWrite(kind, path, data, size))
        return false;

    return true;
}

bool EmbeddedFilePut(EmbedKind kind, const char *path, const void *data, uint64 size)
{
    if (!EmbeddedFilePutDisk(kind, path, data, size))
        return false;

    EmbeddedFilePutMemory(kind, path, data, size);
    return true;
}

bool EmbeddedFileRemove(const char *path)
{
    const EmbeddedFile *File = EmbeddedFileGet(path);
    if (!File)
        return false;

    if (!EmbeddedDbDelete(File->kind, path))
        return false;

    return HashRemove(path);
}