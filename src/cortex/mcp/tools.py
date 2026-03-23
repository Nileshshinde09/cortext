"""
Cortex MCP tool implementations.

Each function maps 1-to-1 with a registered MCP tool name.
server.py stays a thin dispatcher — all logic lives here.
"""


def cortex_query(db, sql: str) -> str:
    """Run a SELECT query and return results."""
    rows = db.fetch(sql)
    if not rows:
        return "No results found"
    return "\n".join(str(row) for row in rows)


def cortex_execute(db, sql: str) -> str:
    """Execute INSERT, UPDATE, DELETE or CREATE statements."""
    db.execute(sql)
    return "Executed successfully"


def cortex_tables(db) -> str:
    """List all tables in the database."""
    rows = db.fetch("SELECT name FROM cortex_master WHERE type='table'")
    if not rows:
        return "No tables found"
    return "\n".join(row["name"] for row in rows)


def cortex_schema(db, table: str | None = None) -> str:
    """Get schema of a specific table, or all tables if none specified."""
    if table:
        rows = db.fetch(f"PRAGMA table_info({table})")
    else:
        rows = db.fetch("SELECT sql FROM cortex_master WHERE type='table'")
    if not rows:
        return "No schema found"
    return "\n".join(str(row) for row in rows)