from .core.bindings import ffi, lib

# Column type constants
CORTEX_INTEGER = 1
CORTEX_FLOAT   = 2
CORTEX_TEXT    = 3
CORTEX_BLOB    = 4
CORTEX_NULL    = 5

CORTEX_ROW     = 100
CORTEX_DONE    = 101

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

    def fetch(self, sql: str):
        # Prepare statement
        stmt_ptr = ffi.new("cortex_stmt **")
        rc = lib.cortex_prepare_v2(
            self._conn,
            sql.encode(),
            -1,
            stmt_ptr,
            ffi.NULL
        )
        if rc != 0:
            raise Exception(f"Failed to prepare statement: {sql}")

        stmt = stmt_ptr[0]
        col_count = lib.cortex_column_count(stmt)

        # Get column names
        columns = []
        for i in range(col_count):
            name = ffi.string(lib.cortex_column_name(stmt, i)).decode()
            columns.append(name)

        # Fetch rows
        rows = []
        while True:
            rc = lib.cortex_step(stmt)
            if rc == CORTEX_DONE:
                break
            if rc != CORTEX_ROW:
                lib.cortex_finalize(stmt)
                raise Exception(f"Error fetching row: {rc}")

            row = {}
            for i in range(col_count):
                col_type = lib.cortex_column_type(stmt, i)
                col_name = columns[i]

                if col_type == CORTEX_INTEGER:
                    row[col_name] = lib.cortex_column_int(stmt, i)
                elif col_type == CORTEX_FLOAT:
                    row[col_name] = lib.cortex_column_double(stmt, i)
                elif col_type == CORTEX_TEXT:
                    val = lib.cortex_column_text(stmt, i)
                    row[col_name] = ffi.string(val).decode() if val != ffi.NULL else None
                elif col_type == CORTEX_NULL:
                    row[col_name] = None
                else:
                    row[col_name] = None

            rows.append(row)

        lib.cortex_finalize(stmt)
        return rows

    def fetchone(self, sql: str):
        results = self.fetch(sql)
        return results[0] if results else None

    def close(self):
        if self._conn:
            lib.cortex_close(self._conn)
            self._conn = None
            print("Cortex connection closed")

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()