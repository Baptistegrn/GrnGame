#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/data/data.h"
#include "grngame/utils/attributes.h"
#include "wren.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static void db_allocate(WrenVM *vm)
{
    sqlite3 **slot = (sqlite3 **)wrenSetSlotNewForeign(vm, 0, 0, sizeof(sqlite3 *));
    *slot = NULL;
}

static void db_stmt_allocate(WrenVM *vm)
{
    DbStmt *stmt = (DbStmt *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(DbStmt));
    stmt->stmt = NULL;
}

static void db_finalize(void *data)
{
    sqlite3 **db = (sqlite3 **)data;
    if (*db)
    {
        DbClose(*db);
        *db = NULL;
    }
}

static void db_stmt_finalize(void *data)
{
    DbStmt *stmt = (DbStmt *)data;
    if (stmt->stmt)
        DbStmtFree(stmt);
}

static void db_open(WrenVM *vm)
{
    const char *name = wrenGetSlotString(vm, 1);
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    *db = DbCreate(name);
}

static void db_close(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    if (*db)
    {
        DbClose(*db);
        *db = NULL;
    }
}

static void db_exists(WrenVM *vm)
{
    const char *name = wrenGetSlotString(vm, 1);
    wrenSetSlotBool(vm, 0, DbExists(name));
}

static void db_fetch(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    const char *sql = wrenGetSlotString(vm, 1);
    DataFetchWren(*db, sql, vm);
}

static void db_write(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    const char *sql = wrenGetSlotString(vm, 1);
    wrenSetSlotBool(vm, 0, DataWrite(*db, sql));
}

static void db_begin(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    DbBegin(*db);
}

static void db_commit(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    DbCommit(*db);
}

static void db_rollback(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    DbRollback(*db);
}

static void db_prepare(WrenVM *vm)
{
    sqlite3 **db = (sqlite3 **)wrenGetSlotForeign(vm, 0);
    const char *sql = wrenGetSlotString(vm, 1);

    DbStmt prepared = DbStmtPrepare(*db, sql);
    if (!prepared.stmt)
    {
        wrenSetSlotNull(vm, 0);
        return;
    }

    wrenEnsureSlots(vm, 2);
    wrenGetVariable(vm, "std/wren/data/db", "DbStmt", 1);
    DbStmt *stmt = (DbStmt *)wrenSetSlotNewForeign(vm, 0, 1, sizeof(DbStmt));
    *stmt = prepared;
}

static void db_stmt_run(WrenVM *vm)
{
    DbStmt *stmt = (DbStmt *)wrenGetSlotForeign(vm, 0);

    DbArg *args = NULL;
    int count = 0;

    if (wrenGetSlotType(vm, 1) == WREN_TYPE_LIST)
    {
        count = wrenGetListCount(vm, 1);
        if (count > 0)
        {
            args = (DbArg *)malloc(count * sizeof(DbArg));
        }

        for (int i = 0; i < count; i++)
        {
            wrenGetListElement(vm, 1, i, 2);
            DbArg arg;
            memset(&arg, 0, sizeof(DbArg));

            switch (wrenGetSlotType(vm, 2))
            {
            case WREN_TYPE_BOOL:
                arg.type = INTEGER;
                arg.value.i = wrenGetSlotBool(vm, 2) ? 1 : 0;
                break;
            case WREN_TYPE_NUM: {
                double value = wrenGetSlotDouble(vm, 2);
                if (value == (double)(long long)value)
                {
                    arg.type = INTEGER;
                    arg.value.i = (int)value;
                }
                else
                {
                    arg.type = FLOAT_;
                    arg.value.f = value;
                }
                break;
            }
            case WREN_TYPE_STRING:
                arg.type = TEXT;
                arg.value.s = wrenGetSlotString(vm, 2);
                break;
            case WREN_TYPE_NULL:
            default:
                arg.type = VOID;
                break;
            }

            args[i] = arg;
        }
    }

    wrenSetSlotBool(vm, 0, DbStmtRun(stmt, args, count));

    if (args != NULL)
    {
        free(args);
    }
}

static void db_stmt_free(WrenVM *vm)
{
    DbStmt *stmt = (DbStmt *)wrenGetSlotForeign(vm, 0);
    DbStmtFree(stmt);
}

void RegisterDbModule()
{
    const char *module_db = "std/wren/data/db";
    const char *module_stmt = "std/wren/data/stmt";
    RegisterClass(module_db, "Db", db_allocate, db_finalize);
    RegisterClass(module_stmt, "DbStmt", db_stmt_allocate, db_stmt_finalize);

    RegisterMethod(module_db, "Db", false, "open(_)", db_open);
    RegisterMethod(module_db, "Db", false, "prepare(_)", db_prepare);
    RegisterMethod(module_db, "Db", false, "close()", db_close);
    RegisterMethod(module_db, "Db", false, "fetch(_)", db_fetch);
    RegisterMethod(module_db, "Db", false, "write(_)", db_write);
    RegisterMethod(module_db, "Db", false, "begin()", db_begin);
    RegisterMethod(module_db, "Db", false, "commit()", db_commit);
    RegisterMethod(module_db, "Db", false, "rollback()", db_rollback);
    RegisterMethod(module_db, "Db", true, "exists(_)", db_exists);

    RegisterMethod(module_stmt, "DbStmt", false, "run(_)", db_stmt_run);
    RegisterMethod(module_stmt, "DbStmt", false, "free()", db_stmt_free);
}