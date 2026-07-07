#include "data.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/file.h"
#include "grngame/utils/strdup.c"
#include "kvec.h"
#include "wren.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static sqlite3_stmt *DbPrepareInternal(sqlite3 *db, const char *sql)
{
    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        LOG_ERROR("Prepare error: %s", sqlite3_errmsg(db));
    return stmt;
}

static void DbBindArgs(sqlite3_stmt *stmt, DbArg *args, int32 argc)
{
    for (int32 i = 0; i < argc; i++)
    {
        switch (args[i].type)
        {
        case INTEGER:
            sqlite3_bind_int(stmt, i + 1, args[i].value.i);
            break;
        case FLOAT_:
            sqlite3_bind_double(stmt, i + 1, args[i].value.f);
            break;
        case TEXT:
            sqlite3_bind_text(stmt, i + 1, args[i].value.s, -1, SQLITE_TRANSIENT);
            break;
        case DATA:
            sqlite3_bind_blob(stmt, i + 1, args[i].value.blob.data, args[i].value.blob.size, SQLITE_TRANSIENT);
            break;
        default:
            sqlite3_bind_null(stmt, i + 1);
            break;
        }
    }
}

bool DbExists(const char *name)
{
    return FileExist(name);
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

void DbBegin(sqlite3 *db)
{
    DataWrite(db, "BEGIN TRANSACTION;");
}
void DbCommit(sqlite3 *db)
{
    DataWrite(db, "COMMIT;");
}
void DbRollback(sqlite3 *db)
{
    DataWrite(db, "ROLLBACK;");
}

bool DataWrite(sqlite3 *db, const char *sql)
{
    sqlite3_stmt *stmt = DbPrepareInternal(db, sql);
    if (!stmt)
        return false;

    int32 rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE && rc != SQLITE_ROW)
    {
        LOG_ERROR("SQL execution error: %s", sqlite3_errmsg(db));
        return false;
    }
    return true;
}

static DbResult DbResultGet(sqlite3_stmt *stmt)
{
    DbResult result;
    kv_init(result.rows);

    int32 colCount = sqlite3_column_count(stmt);
    int32 rc;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        DbRow row;
        kv_init(row.cols);

        for (int32 i = 0; i < colCount; i++)
        {
            DbValue v = {0};
            v.name = StrDupSafe(sqlite3_column_name(stmt, i));

            switch (sqlite3_column_type(stmt, i))
            {
            case SQLITE_INTEGER:
                v.type = INTEGER;
                v.value.i = sqlite3_column_int(stmt, i);
                break;
            case SQLITE_FLOAT:
                v.type = FLOAT_;
                v.value.f = sqlite3_column_double(stmt, i);
                break;
            case SQLITE_TEXT:
                v.type = TEXT;
                v.value.s = StrDupSafe((const char *)sqlite3_column_text(stmt, i));
                break;
            case SQLITE_BLOB: {
                v.type = DATA;
                int32 size = sqlite3_column_bytes(stmt, i);
                v.value.blob.data = malloc(size);
                memcpy(v.value.blob.data, sqlite3_column_blob(stmt, i), size);
                v.value.blob.size = size;
                break;
            }
            case SQLITE_NULL:
                v.type = VOID;
                break;
            }

            kv_push(DbValue, row.cols, v);
        }

        kv_push(DbRow, result.rows, row);
    }

    if (rc != SQLITE_DONE)
        LOG_ERROR("SQL step error: %s", sqlite3_errmsg(sqlite3_db_handle(stmt)));

    return result;
}

DbResult DataFetch(sqlite3 *db, const char *sql)
{
    sqlite3_stmt *stmt = DbPrepareInternal(db, sql);
    if (!stmt)
        return (DbResult){0};
    DbResult res = DbResultGet(stmt);
    sqlite3_finalize(stmt);
    return res;
}

void DbResultPrint(DbResult *res)
{
    for (uint64 r = 0; r < kv_size(res->rows); r++)
    {
        DbRow row = kv_A(res->rows, r);
        printf("Row %zu:\n", r);
        for (uint64 c = 0; c < kv_size(row.cols); c++)
        {
            DbValue v = kv_A(row.cols, c);
            printf("  %s = ", v.name);
            switch (v.type)
            {
            case INTEGER:
                printf("%d\n", v.value.i);
                break;
            case FLOAT_:
                printf("%f\n", v.value.f);
                break;
            case TEXT:
                printf("%s\n", v.value.s);
                break;
            case DATA:
                printf("[BLOB size=%d]\n", v.value.blob.size);
                break;
            case VOID:
                printf("NULL\n");
                break;
            }
        }
    }
}

void DbResultFree(DbResult *res)
{
    for (uint64 r = 0; r < kv_size(res->rows); r++)
    {
        DbRow row = kv_A(res->rows, r);
        for (uint64 c = 0; c < kv_size(row.cols); c++)
        {
            DbValue v = kv_A(row.cols, c);
            free((void *)v.name);
            if (v.type == TEXT)
                free((void *)v.value.s);
            if (v.type == DATA)
                free(v.value.blob.data);
        }
        kv_destroy(row.cols);
    }
    kv_destroy(res->rows);
}

DbStmt DbStmtPrepare(sqlite3 *db, const char *sql)
{
    DbStmt s = {0};
    if (sqlite3_prepare_v2(db, sql, -1, &s.stmt, NULL) != SQLITE_OK)
        LOG_ERROR("Prepare error: %s", sqlite3_errmsg(db));
    return s;
}

bool DbStmtRun(DbStmt *s, DbArg *args, int32 argc)
{
    sqlite3_clear_bindings(s->stmt);
    DbBindArgs(s->stmt, args, argc);

    int32 rc = sqlite3_step(s->stmt);
    sqlite3_reset(s->stmt);

    if (rc != SQLITE_DONE && rc != SQLITE_ROW)
    {
        LOG_ERROR("SQL step error: %s", sqlite3_errmsg(sqlite3_db_handle(s->stmt)));
        return false;
    }
    return true;
}

void DbStmtFree(DbStmt *s)
{
    if (s->stmt)
    {
        sqlite3_finalize(s->stmt);
        s->stmt = NULL;
    }
}

void DataFetchWren(sqlite3 *db, const char *sql, WrenVM *vm)
{
    wrenEnsureSlots(vm, 4);
    wrenSetSlotNewList(vm, 0);

    sqlite3_stmt *stmt = DbPrepareInternal(db, sql);
    if (!stmt)
        return;

    int32 colCount = sqlite3_column_count(stmt);
    int32 rc;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        wrenSetSlotNewMap(vm, 1);

        for (int32 i = 0; i < colCount; i++)
        {
            wrenSetSlotString(vm, 2, sqlite3_column_name(stmt, i));

            switch (sqlite3_column_type(stmt, i))
            {
            case SQLITE_INTEGER:
                wrenSetSlotDouble(vm, 3, (float64)sqlite3_column_int64(stmt, i));
                break;
            case SQLITE_FLOAT:
                wrenSetSlotDouble(vm, 3, sqlite3_column_double(stmt, i));
                break;
            case SQLITE_TEXT:
                wrenSetSlotString(vm, 3, (const char *)sqlite3_column_text(stmt, i));
                break;
            default:
                wrenSetSlotNull(vm, 3);
                break; // BLOB / NULL
            }

            wrenSetMapValue(vm, 1, 2, 3);
        }

        wrenInsertInList(vm, 0, -1, 1);
    }

    if (rc != SQLITE_DONE)
        LOG_ERROR("SQL step error: %s", sqlite3_errmsg(db));

    sqlite3_finalize(stmt);
}