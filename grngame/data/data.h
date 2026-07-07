#pragma once

#include "grngame/math/types.h"
#include "grngame/utils/c_cpp.h"
#include "wren.h"
#include <kvec.h>
#include <sqlite3.h>
#include <stdbool.h>


BEGIN_DECLARATIONS

typedef enum
{
    INTEGER = 0,
    FLOAT_ = 1,
    TEXT = 2,
    DATA = 3,
    VOID = 4
} DbType;

typedef struct
{
    const char *name; // null for DbArg
    DbType type;
    union {
        int32 i;
        float64 f;
        const char *s;
        struct
        {
            void *data;
            int32 size;
        } blob; // not usable in wren / sql
    } value;
} DbValue;

typedef struct
{
    kvec_t(DbValue) cols;
} DbRow;

typedef struct
{
    kvec_t(DbRow) rows;
} DbResult;

typedef struct
{
    sqlite3_stmt *stmt;
} DbStmt;

typedef DbValue DbArg;

sqlite3 *DbCreate(const char *name);
void DbClose(sqlite3 *db);
bool DbExists(const char *name);

void DbResultPrint(DbResult *res);
void DbResultFree(DbResult *res);

bool DataWrite(sqlite3 *db, const char *sql);
DbResult DataFetch(sqlite3 *db, const char *sql);

// wrap in begin/commit for bulk writes
void DbBegin(sqlite3 *db);
void DbCommit(sqlite3 *db);
void DbRollback(sqlite3 *db);

DbStmt DbStmtPrepare(sqlite3 *db, const char *sql);
bool DbStmtRun(DbStmt *s, DbArg *args, int32 argc);
void DbStmtFree(DbStmt *s);

// bypass DbResult to populate Wren list directly in one pass
void DataFetchWren(sqlite3 *db, const char *sql, WrenVM *vm);

END_DECLARATIONS