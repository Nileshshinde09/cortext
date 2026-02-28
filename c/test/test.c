#include "./lib/libcortex.h"
#include <stdio.h>

int main() {
    cortex *db;

    cortex_open("test.ctx", &db);
    printf("Database opened\n");
    fflush(stdout);

    // Create table
    cortex_exec(db, "CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT)", 0, 0, 0);
    printf("Table created\n");
    fflush(stdout);

    // Insert data
    cortex_exec(db, "INSERT INTO users VALUES (1, 'John')", 0, 0, 0);
    printf("Data inserted\n");
    fflush(stdout);

    cortex_close(db);
    printf("Done\n");
    fflush(stdout);

    return 0;
}