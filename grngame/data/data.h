#pragma once

#include <kvec.h>
#include <sqlite3.h>
#include <stdbool.h>

typedef enum
{
    INTEGER = 0,
    FLOAT = 1,
    TEXT = 2,
    DATA = 3
} Type;

typedef struct
{
    const char *name;
    Type type;
    union {
        int i;
        double f;
        const char *s;
        // for data (not use in wren and cant be use in sql)
        struct
        {
            void *data;
            int size;
        } blob;
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

sqlite3 *DbCreate(const char *name);
void DbClose(sqlite3 *db);
bool DbExists(const char *name);

void DbResultPrint(DbResult *res);
void DbResultFree(DbResult *res);

bool DataWrite(sqlite3 *db, const char *sql);
DbResult DataFetch(sqlite3 *db, const char *sql);

// before long insert
void DbBegin(sqlite3 *db);
void DbCommit(sqlite3 *db);
void DbRollback(sqlite3 *db);
