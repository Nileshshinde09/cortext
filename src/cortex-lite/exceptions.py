"""
Cortex exception hierarchy — PEP 249 compliant.

Raise and catch these instead of bare Exception so callers
can handle Cortex-specific errors with precision.

Usage:
    from cortex.exception import CortexOperationalError, CortexProgrammingError
"""


class CortexError(Exception):
    """
    Base class for all Cortex exceptions.
    Catch this when you want to handle any Cortex-related error.
    """
    pass


# ── Connection / environment errors ───────────────────────────────────────────

class CortexInterfaceError(CortexError):
    """
    Error related to the Cortex driver interface itself, not the database.
    Examples: cursor used after connection is closed, bad API usage.
    """
    pass


class CortexOperationalError(CortexError):
    """
    Error related to the database's operation that is not necessarily
    under the programmer's control.
    Examples: failed to open a .ctx file, disk full, connection lost.
    """
    pass


# ── SQL / query errors ─────────────────────────────────────────────────────────

class CortexDatabaseError(CortexError):
    """
    Error related to the database engine during query execution.
    Examples: constraint violation, cortex_exec returned non-zero.
    """
    def __init__(self, message: str, sql: str | None = None, rc: int | None = None):
        self.sql = sql
        self.rc  = rc
        detail   = f" [rc={rc}]" if rc is not None else ""
        detail  += f"\nSQL: {sql}" if sql else ""
        super().__init__(message + detail)


class CortexProgrammingError(CortexError):
    """
    Error caused by incorrect use of the API or bad SQL syntax.
    Examples: .ctx extension missing, prepare statement failed on bad SQL,
              fetchone/fetch called with a non-SELECT statement.
    """
    pass


class CortexDataError(CortexDatabaseError):
    """
    Error due to problems with the data being processed.
    Examples: type mismatch, value out of range.
    """
    pass


# ── Transaction errors ─────────────────────────────────────────────────────────

class CortexNotSupportedError(CortexError):
    """
    Raised when a feature or method is not supported by Cortex.
    Examples: calling .callproc() which has no meaning in a SQLite backend.
    """
    pass