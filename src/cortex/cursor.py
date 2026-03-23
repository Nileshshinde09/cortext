"""
CortexCursor — PEP 249 compliant cursor for CortexDB.

A cursor is created by CortexConnection.cursor() and owns the full
statement lifecycle: prepare → step → column extraction → finalize.

Usage:
    with cortex.connect("mydb.ctx") as db:
        cur = db.cursor()
        cur.execute("INSERT INTO users (name) VALUES (?)", ("Alice",))
        cur.execute("SELECT * FROM users")
        rows = cur.fetchall()
        one  = cur.fetchone()
"""

from __future__ import annotations

from typing import TYPE_CHECKING, Any

from .exception import (
    CortexInterfaceError,
    CortexDatabaseError,
    CortexProgrammingError,
)

if TYPE_CHECKING:
    from .connection import CortexConnection

from .core.bindings import ffi, lib


# SQLite / Cortex step return codes (mirrors connection.py constants)
_CORTEX_INTEGER = 1
_CORTEX_FLOAT   = 2
_CORTEX_TEXT    = 3
_CORTEX_BLOB    = 4
_CORTEX_NULL    = 5
_CORTEX_ROW     = 100
_CORTEX_DONE    = 101


class CortexCursor:
    """
    Executes SQL statements against a CortexConnection.

    Attributes
    ----------
    description : list[tuple] | None
        Read-only sequence of 7-item tuples describing each column of the
        last SELECT result, following PEP 249.  None before any query is run
        or after a non-SELECT statement.
    rowcount : int
        Number of rows affected by the last INSERT / UPDATE / DELETE.
        -1 when not applicable (e.g. SELECT, or no statement run yet).
    lastrowid : int | None
        Row-id of the last successful INSERT, or None.
    arraysize : int
        Default number of rows fetched by fetchmany().  Defaults to 1.
    """

    def __init__(self, connection: "CortexConnection") -> None:
        self._conn       = connection
        self._rows: list[dict[str, Any]] = []
        self._pos: int   = 0           # current fetch position
        self._closed     = False

        # PEP 249 public attributes
        self.description: list[tuple] | None = None
        self.rowcount: int                   = -1
        self.lastrowid: int | None           = None
        self.arraysize: int                  = 1

    # ── Internal helpers ───────────────────────────────────────────────────────

    def _check_open(self) -> None:
        if self._closed:
            raise CortexInterfaceError("Cannot operate on a closed cursor.")
        if self._conn._conn is None:
            raise CortexInterfaceError("Cannot operate on a closed connection.")

    def _bind_params(self, stmt, params: tuple | list) -> None:
        """Bind positional parameters (?) to a prepared statement."""
        for idx, value in enumerate(params, start=1):
            if value is None:
                rc = lib.cortex_bind_null(stmt, idx)
            elif isinstance(value, bool):
                rc = lib.cortex_bind_int(stmt, idx, int(value))
            elif isinstance(value, int):
                rc = lib.cortex_bind_int64(stmt, idx, value)
            elif isinstance(value, float):
                rc = lib.cortex_bind_double(stmt, idx, value)
            elif isinstance(value, str):
                encoded = value.encode("utf-8")
                rc = lib.cortex_bind_text(stmt, idx, encoded, len(encoded), ffi.cast("void *", -1))
            elif isinstance(value, (bytes, bytearray)):
                rc = lib.cortex_bind_blob(stmt, idx, value, len(value), ffi.cast("void *", -1))
            else:
                raise CortexProgrammingError(
                    f"Unsupported parameter type at index {idx}: {type(value).__name__}"
                )

            if rc != 0:
                raise CortexDatabaseError(
                    f"Failed to bind parameter at index {idx}", rc=rc
                )

    def _extract_row(self, stmt, columns: list[str]) -> dict[str, Any]:
        """Extract a single row from a stepped statement into a dict."""
        row: dict[str, Any] = {}
        for i, col_name in enumerate(columns):
            col_type = lib.cortex_column_type(stmt, i)

            if col_type == _CORTEX_INTEGER:
                row[col_name] = lib.cortex_column_int64(stmt, i)
            elif col_type == _CORTEX_FLOAT:
                row[col_name] = lib.cortex_column_double(stmt, i)
            elif col_type == _CORTEX_TEXT:
                val = lib.cortex_column_text(stmt, i)
                row[col_name] = ffi.string(val).decode("utf-8") if val != ffi.NULL else None
            elif col_type == _CORTEX_BLOB:
                val = lib.cortex_column_blob(stmt, i)
                n   = lib.cortex_column_bytes(stmt, i)
                row[col_name] = bytes(ffi.buffer(val, n)) if val != ffi.NULL else None
            elif col_type == _CORTEX_NULL:
                row[col_name] = None
            else:
                row[col_name] = None
        return row

    def _build_description(self, stmt, col_count: int) -> list[tuple]:
        """
        Build PEP 249 description: list of 7-tuples
        (name, type_code, display_size, internal_size, precision, scale, null_ok)
        """
        desc = []
        for i in range(col_count):
            name = ffi.string(lib.cortex_column_name(stmt, i)).decode("utf-8")
            desc.append((name, None, None, None, None, None, None))
        return desc

    # ── PEP 249 public API ─────────────────────────────────────────────────────

    def execute(self, sql: str, parameters: tuple | list = ()) -> "CortexCursor":
        """
        Execute a single SQL statement with optional bound parameters.

        Parameters
        ----------
        sql : str
            SQL statement. Use ? placeholders for parameters.
        parameters : tuple | list
            Positional parameter values matching each ? in `sql`.

        Returns
        -------
        self — allows chaining: cursor.execute(...).fetchall()
        """
        self._check_open()
        self._rows = []
        self._pos  = 0
        self.description = None
        self.rowcount    = -1
        self.lastrowid   = None

        sql_stripped = sql.strip().upper()
        is_select    = sql_stripped.startswith("SELECT") or sql_stripped.startswith("WITH")

        with self._conn._lock:
            if is_select or parameters:
                # Prepared-statement path (supports param binding + SELECT)
                stmt_ptr = ffi.new("cortex_stmt **")
                rc = lib.cortex_prepare_v2(
                    self._conn._conn,
                    sql.encode("utf-8"),
                    -1,
                    stmt_ptr,
                    ffi.NULL,
                )
                if rc != 0:
                    raise CortexProgrammingError(
                        f"Failed to prepare statement.", sql=sql
                    )

                stmt = stmt_ptr[0]

                try:
                    if parameters:
                        self._bind_params(stmt, parameters)

                    if is_select:
                        col_count        = lib.cortex_column_count(stmt)
                        columns          = [
                            ffi.string(lib.cortex_column_name(stmt, i)).decode()
                            for i in range(col_count)
                        ]
                        self.description = self._build_description(stmt, col_count)

                        while True:
                            rc = lib.cortex_step(stmt)
                            if rc == _CORTEX_DONE:
                                break
                            if rc != _CORTEX_ROW:
                                raise CortexDatabaseError(
                                    "Error while stepping through rows.", sql=sql, rc=rc
                                )
                            self._rows.append(self._extract_row(stmt, columns))

                        self.rowcount = len(self._rows)
                    else:
                        # DML with parameters (INSERT/UPDATE/DELETE + ?)
                        rc = lib.cortex_step(stmt)
                        if rc not in (_CORTEX_DONE, _CORTEX_ROW):
                            raise CortexDatabaseError(
                                "Statement execution failed.", sql=sql, rc=rc
                            )
                        self.rowcount  = lib.cortex_changes(self._conn._conn)
                        self.lastrowid = lib.cortex_last_insert_rowid(self._conn._conn)
                finally:
                    lib.cortex_finalize(stmt)

            else:
                # Fast path for parameterless DDL / DML via cortex_exec
                errmsg = ffi.new("char **")
                rc = lib.cortex_exec(
                    self._conn._conn,
                    sql.encode("utf-8"),
                    ffi.NULL,
                    ffi.NULL,
                    errmsg,
                )
                if rc != 0:
                    error = ffi.string(errmsg[0]).decode()
                    lib.cortex_free(errmsg[0])
                    raise CortexDatabaseError(error, sql=sql, rc=rc)

                self.rowcount  = lib.cortex_changes(self._conn._conn)
                self.lastrowid = lib.cortex_last_insert_rowid(self._conn._conn)

        return self

    def executemany(self, sql: str, seq_of_parameters: list[tuple | list]) -> "CortexCursor":
        """
        Execute the same SQL statement for each set of parameters.
        Useful for bulk inserts.

        Parameters
        ----------
        sql : str
            SQL statement with ? placeholders.
        seq_of_parameters : list[tuple | list]
            Each item is a parameter tuple for one execution.
        """
        self._check_open()
        total_changes = 0
        for params in seq_of_parameters:
            self.execute(sql, params)
            if self.rowcount > 0:
                total_changes += self.rowcount
        self.rowcount = total_changes
        return self

    # ── Fetch methods ──────────────────────────────────────────────────────────

    def fetchone(self) -> dict[str, Any] | None:
        """Return the next row from the last SELECT, or None if exhausted."""
        self._check_open()
        if self._pos >= len(self._rows):
            return None
        row = self._rows[self._pos]
        self._pos += 1
        return row

    def fetchmany(self, size: int | None = None) -> list[dict[str, Any]]:
        """
        Return up to `size` rows (defaults to self.arraysize).
        Returns an empty list when no more rows are available.
        """
        self._check_open()
        size  = size if size is not None else self.arraysize
        chunk = self._rows[self._pos : self._pos + size]
        self._pos += len(chunk)
        return chunk

    def fetchall(self) -> list[dict[str, Any]]:
        """Return all remaining rows from the last SELECT."""
        self._check_open()
        remaining = self._rows[self._pos:]
        self._pos = len(self._rows)
        return remaining

    # ── Lifecycle ──────────────────────────────────────────────────────────────

    def close(self) -> None:
        """Mark the cursor as closed. Subsequent operations will raise CortexInterfaceError."""
        self._closed = True
        self._rows   = []

    def __iter__(self):
        """Iterate over result rows one by one (memory-friendly)."""
        self._check_open()
        while self._pos < len(self._rows):
            yield self._rows[self._pos]
            self._pos += 1

    def __enter__(self) -> "CortexCursor":
        return self

    def __exit__(self, exc_type, exc_val, exc_tb) -> None:
        self.close()

    def __repr__(self) -> str:
        state = "closed" if self._closed else f"{len(self._rows)} rows buffered"
        return f"<CortexCursor {state}>"