#include "data.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include "kvec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool DbExists(const char *name)
{
    FILE *file = fopen(name, "rb");
    if (!file)
        return false;
    fclose(file);
    return true;
}

sqlite3 *DbCreate(const char *name)
{
    sqlite3 *db = NULL;
    if (UNLIKELY(sqlite3_open(name, &db) != SQLITE_OK))
    {
        LOG_ERROR("Impossible to create DB named: %s (%s)", name, db ? sqlite3_errmsg(db) : "unknown error");
        if (db)
            sqlite3_close(db);
        return NULL;
    }
    return db;
}

void DbClose(sqlite3 *db)
{
    sqlite3_close(db);
}

static bool DbExec(sqlite3 *db, const char *sql)
{
    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        LOG_ERROR("SQL prepare error: %s", sqlite3_errmsg(db));
        return false;
    }

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW)
    {
        LOG_ERROR("SQL execution error: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

static sqlite3_stmt *DbPrepare(sqlite3 *db, const char *sql)
{
    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        LOG_ERROR("Prepare error: %s", sqlite3_errmsg(db));
        return NULL;
    }

    return stmt;
}

static int DbStep(sqlite3_stmt *stmt)
{
    return sqlite3_step(stmt);
}

static void DbFinalize(sqlite3_stmt *stmt)
{
    sqlite3_finalize(stmt);
}

// todo moove in correct file
static char *StrDupSafe(const char *s)
{
    if (!s)
        return NULL;
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    memcpy(copy, s, len);
    return copy;
}

static DbResult DbResultGet(sqlite3_stmt *stmt)
{
    DbResult result;
    kv_init(result.rows);

    int colCount = sqlite3_column_count(stmt);
    int rc;

    while ((rc = DbStep(stmt)) == SQLITE_ROW)
    {
        DbRow row;
        kv_init(row.cols);

        for (int i = 0; i < colCount; i++)
        {
            DbValue v = {0};

            v.name = StrDupSafe(sqlite3_column_name(stmt, i));
            int type = sqlite3_column_type(stmt, i);

            switch (type)
            {
            case SQLITE_INTEGER:
                v.type = INTEGER;
                v.value.i = sqlite3_column_int(stmt, i);
                break;

            case SQLITE_FLOAT:
                v.type = FLOAT;
                v.value.f = sqlite3_column_double(stmt, i);
                break;

            case SQLITE_TEXT:
                v.type = TEXT;
                v.value.s = StrDupSafe((const char *)sqlite3_column_text(stmt, i));
                break;

            case SQLITE_BLOB:
                v.type = DATA;
                const void *blob = sqlite3_column_blob(stmt, i);
                int size = sqlite3_column_bytes(stmt, i);
                void *copy = malloc(size);
                memcpy(copy, blob, size);
                v.value.blob.data = copy;
                v.value.blob.size = size;
                break;

            case SQLITE_NULL:
                v.type = DATA;
                v.value.s = NULL;
                break;
            }

            kv_push(DbValue, row.cols, v);
        }

        kv_push(DbRow, result.rows, row);
    }

    if (rc != SQLITE_DONE)
    {
        LOG_ERROR("SQL step error: %s", sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }

    return result;
}

// debug
void DbResultPrint(DbResult *res)
{
    for (size_t r = 0; r < kv_size(res->rows); r++)
    {
        DbRow row = kv_A(res->rows, r);

        printf("Row %zu:\n", r);

        for (size_t c = 0; c < kv_size(row.cols); c++)
        {
            DbValue v = kv_A(row.cols, c);

            printf("  %s = ", v.name);

            switch (v.type)
            {
            case INTEGER:
                printf("%d\n", v.value.i);
                break;

            case FLOAT:
                printf("%f\n", v.value.f);
                break;

            case TEXT:
                printf("%s\n", v.value.s);
                break;

            case DATA:
                // we only print size of the data
                printf("[BLOB size=%d]", v.value.blob.size);
                break;
                break;
            }
        }
    }
}

void DbResultFree(DbResult *res)
{
    for (size_t r = 0; r < kv_size(res->rows); r++)
    {
        DbRow row = kv_A(res->rows, r);

        for (size_t c = 0; c < kv_size(row.cols); c++)
        {
            DbValue v = kv_A(row.cols, c);
            if (v.name)
                free((void *)v.name);
            if (v.type == TEXT && v.value.s)
                free((void *)v.value.s);
            if (v.type == DATA && v.value.blob.data)
                free(v.value.blob.data);
        }

        kv_destroy(row.cols);
    }

    kv_destroy(res->rows);
}

DbResult DataFetch(sqlite3 *db, const char *sql)
{
    sqlite3_stmt *stmt = DbPrepare(db, sql);
    if (!stmt)
        return (DbResult){0};
    DbResult res = DbResultGet(stmt);
    DbFinalize(stmt);
    return res;
}

bool DataWrite(sqlite3 *db, const char *sql)
{
    return DbExec(db, sql);
}
