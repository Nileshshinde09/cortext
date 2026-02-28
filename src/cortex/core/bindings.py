from cffi import FFI
import os
import sys

ffi = FFI()

ffi.cdef("""
    typedef struct cortex cortex;
    typedef struct cortex_stmt cortex_stmt;

    int cortex_open(const char *path, cortex **db);
    int cortex_close(cortex *db);
    int cortex_exec(
        cortex *db,
        const char *sql,
        int (*callback)(void*, int, char**, char**),
        void *arg,
        char **errmsg
    );

    int cortex_prepare_v2(
        cortex *db,
        const char *sql,
        int nByte,
        cortex_stmt **ppStmt,
        const char **pzTail
    );
    int cortex_step(cortex_stmt *stmt);
    int cortex_finalize(cortex_stmt *stmt);

    int cortex_column_count(cortex_stmt *stmt);
    const char *cortex_column_name(cortex_stmt *stmt, int iCol);
    int cortex_column_type(cortex_stmt *stmt, int iCol);
    int cortex_column_int(cortex_stmt *stmt, int iCol);
    double cortex_column_double(cortex_stmt *stmt, int iCol);
    const char *cortex_column_text(cortex_stmt *stmt, int iCol);

    void cortex_free(void *ptr);
""")


def _load_library():
    if sys.platform == "win32" or sys.platform == "win64":
        lib_name = "libcortex.dll"
    elif sys.platform == "darwin":
        lib_name = "libcortex.dylib"
    else:
        lib_name = "libcortex.so"

    lib_path = os.path.join(os.path.dirname(__file__), lib_name)

    if not os.path.exists(lib_path):
        raise FileNotFoundError(
            f"Cortex library not found at {lib_path}"
        )

    return ffi.dlopen(lib_path)


lib = _load_library()