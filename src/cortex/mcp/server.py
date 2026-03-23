"""
Cortex MCP server — transport and tool registration only.

Tool logic lives in tools.py. This file is a pure dispatcher:
register tools → receive call → delegate → return TextContent.
"""

from mcp.server import Server
from mcp.types import Tool, TextContent

from .tools import cortex_query, cortex_execute, cortex_tables, cortex_schema

app = Server("cortex")
_db = None


def set_connection(db):
    global _db
    _db = db


@app.list_tools()
async def list_tools() -> list[Tool]:
    return [
        Tool(
            name="cortex_query",
            description="Run a SELECT query and return results",
            inputSchema={
                "type": "object",
                "properties": {
                    "sql": {
                        "type": "string",
                        "description": "SELECT SQL statement"
                    }
                },
                "required": ["sql"]
            }
        ),
        Tool(
            name="cortex_execute",
            description="Execute INSERT, UPDATE, DELETE or CREATE statements",
            inputSchema={
                "type": "object",
                "properties": {
                    "sql": {
                        "type": "string",
                        "description": "SQL statement to execute"
                    }
                },
                "required": ["sql"]
            }
        ),
        Tool(
            name="cortex_tables",
            description="List all tables in the database",
            inputSchema={
                "type": "object",
                "properties": {}
            }
        ),
        Tool(
            name="cortex_schema",
            description="Get schema of a table or all tables",
            inputSchema={
                "type": "object",
                "properties": {
                    "table": {
                        "type": "string",
                        "description": "Table name (optional)"
                    }
                }
            }
        ),
    ]


@app.call_tool()
async def call_tool(name: str, arguments: dict) -> list[TextContent]:
    if _db is None:
        return [TextContent(type="text", text="Error: No database connected")]

    try:
        if name == "cortex_query":
            result = cortex_query(_db, arguments.get("sql", ""))

        elif name == "cortex_execute":
            result = cortex_execute(_db, arguments.get("sql", ""))

        elif name == "cortex_tables":
            result = cortex_tables(_db)

        elif name == "cortex_schema":
            result = cortex_schema(_db, arguments.get("table"))

        else:
            result = f"Unknown tool: {name}"

        return [TextContent(type="text", text=result)]

    except Exception as e:
        return [TextContent(type="text", text=f"Error: {str(e)}")]