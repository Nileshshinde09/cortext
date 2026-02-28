#include "./lib/libcortex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Simple testing macros
*/
#define ASSERT_TRUE(condition, message) \
    if (!(condition)) { \
        printf("❌ Test Failed: %s\n", message); \
        exit(1); \
    } else { \
        printf("✅ %s\n", message); \
    }

/*
    Test: Database open & close
*/
void test_open_close() {
    cortex *db;

    int rc = cortex_open("test.ctx", &db);
    ASSERT_TRUE(rc == 0, "Database opened successfully");

    rc = cortex_close(db);
    ASSERT_TRUE(rc == 0, "Database closed successfully");
}

/*
    Test: Table creation
*/
void test_create_table() {
    cortex *db;
    cortex_open("test.ctx", &db);

    int rc = cortex_exec(
        db,
        "CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT)",
        0, 0, 0
    );

    ASSERT_TRUE(rc == 0, "Table created successfully");

    cortex_close(db);
}

/*
    Test: Insert query
*/
void test_insert() {
    cortex *db;
    cortex_open("test.ctx", &db);

    cortex_exec(db, "CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT)", 0, 0, 0);

    int rc = cortex_exec(
        db,
        "INSERT INTO users VALUES (1, 'John')",
        0, 0, 0
    );

    ASSERT_TRUE(rc == 0, "Insert executed successfully");

    cortex_close(db);
}

/*
    Test: Invalid query
*/
void test_invalid_query() {
    cortex *db;
    cortex_open("test.ctx", &db);

    int rc = cortex_exec(db, "INSER INTO users VALUES (1, 'John')", 0, 0, 0);

    ASSERT_TRUE(rc != 0, "Invalid query should fail");

    cortex_close(db);
}

/*
    MAIN TEST RUNNER
*/
int main() {
    printf("🚀 Running Cortex C Tests...\n\n");

    test_open_close();
    test_create_table();
    test_insert();
    test_invalid_query();

    printf("\n🎉 All tests passed!\n");
    return 0;
}