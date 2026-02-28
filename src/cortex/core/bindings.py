from cffi import FFI
import os
import sys

ffi = FFI()

# Define the C API
ffi.cdef("""
    typedef struct cortex cortex;

    int cortex_open(const char *path, cortex **db);
    int cortex_close(cortex *db);
    int cortex_exec(
        cortex *db,
        const char *sql,
        int (*callback)(void*, int, char**, char**),
        void *arg,
        char **errmsg
    );
    void cortex_free(void *ptr);
""")


# Load the DLL
def _load_library():
    if sys.platform == "win32":
        lib_name = "libcortex.dll"
    elif sys.platform == "darwin":
        lib_name = "libcortex.dylib"
    else:
        lib_name = "libcortex.so"

    # Look for DLL next to this file
    lib_path = os.path.join(os.path.dirname(__file__), lib_name)

    if not os.path.exists(lib_path):
        raise FileNotFoundError(
            f"Cortex library not found at {lib_path}"
        )

    return ffi.dlopen(lib_path)


lib = _load_library()