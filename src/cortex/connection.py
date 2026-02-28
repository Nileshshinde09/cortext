from .core.bindings import ffi, lib

class CortexConnection:
    def __init__(self, path: str):
        if not path.endswith(".ctx"):
            raise ValueError("Cortex database file must have .ctx extension")

        self._db = ffi.new("cortex **")
        rc = lib.cortex_open(path.encode(), self._db)

        if rc != 0:
            raise ConnectionError(f"Failed to open database: {path}")

        self._conn = self._db[0]
        print(f"Cortex connected to {path}")

    def execute(self, sql: str):
        errmsg = ffi.new("char **")
        rc = lib.cortex_exec(
            self._conn,
            sql.encode(),
            ffi.NULL,
            ffi.NULL,
            errmsg
        )
        if rc != 0:
            error = ffi.string(errmsg[0]).decode()
            lib.cortex_free(errmsg[0])
            raise Exception(f"SQL Error: {error}")

        return rc

    def close(self):
        if self._conn:
            lib.cortex_close(self._conn)
            self._conn = None
            print("Cortex connection closed")

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()