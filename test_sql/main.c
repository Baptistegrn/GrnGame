#include "../grngame/data/data.h"
#include <stdio.h>

int main()
{
    const char *db_name = "game.db";
    bool exists = DbExists(db_name);
    sqlite3 *db = DbCreate(db_name);
    if (!db)
    {
        printf("Failed to open DB\n");
        return 1;
    }
    if (!exists)
    {
        DataWrite(db, "CREATE TABLE player ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "name TEXT,"
                      "score INTEGER);");

        DataWrite(db, "INSERT INTO player (name, score) VALUES ('Baptiste', 42);");

        DataWrite(db, "INSERT INTO player (name, score) VALUES ('Alex', 100);");
    }
    DbResult res = DataFetch(db, "SELECT id, name, score FROM player;");

    DbResultPrint(&res);
    DbResultFree(&res);
    DbClose(db);

    return 0;
}