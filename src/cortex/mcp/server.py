from mcp.server import Server
from mcp.types import Tool, TextContent

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
    global _db

    if _db is None:
        return [TextContent(type="text", text="Error: No database connected")]

    try:
        if name == "cortex_query":
            rows = _db.fetch(arguments.get("sql", ""))
            if not rows:
                return [TextContent(type="text", text="No results found")]
            result = "\n".join(str(row) for row in rows)
            return [TextContent(type="text", text=result)]

        elif name == "cortex_execute":
            _db.execute(arguments.get("sql", ""))
            return [TextContent(type="text", text="Executed successfully")]

        elif name == "cortex_tables":
            rows = _db.fetch(
                "SELECT name FROM cortex_master WHERE type='table'"
            )
            if not rows:
                return [TextContent(type="text", text="No tables found")]
            tables = [row["name"] for row in rows]
            return [TextContent(type="text", text="\n".join(tables))]

        elif name == "cortex_schema":
            table = arguments.get("table", None)
            if table:
                rows = _db.fetch(f"PRAGMA table_info({table})")
            else:
                rows = _db.fetch(
                    "SELECT sql FROM cortex_master WHERE type='table'"
                )
            if not rows:
                return [TextContent(type="text", text="No schema found")]
            result = "\n".join(str(row) for row in rows)
            return [TextContent(type="text", text=result)]

        else:
            return [TextContent(type="text", text=f"Unknown tool: {name}")]

    except Exception as e:
        return [TextContent(type="text", text=f"Error: {str(e)}")]