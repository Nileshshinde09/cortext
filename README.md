# Cortex 🧠

> **The first embedded database built natively for AI agents**

[![CI](https://github.com/nileshshinde09/cortexdb/actions/workflows/ci.yml/badge.svg)](https://github.com/nileshshinde09/cortexdb/actions)
[![PyPI](https://img.shields.io/pypi/v/cortexdb-mcp)](https://pypi.org/project/cortexdb-mcp/)
[![Python](https://img.shields.io/pypi/pyversions/cortexdb-mcp)](https://pypi.org/project/cortexdb-mcp/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

Cortex is a lightweight, file-based embedded database with a **built-in MCP (Model Context Protocol) server**. Just like SQLite revolutionized local data storage, Cortex does the same for the AI agent era.

One line of code. Your database is ready. Your AI agent can talk to it instantly.

```python
import cortex

db = cortex.connect("myapp.ctx")
# ✅ Database ready
# ✅ MCP server running
# ✅ AI agents can connect immediately
```

---

## Why Cortex?

Today, connecting an AI agent to a local database requires:
- Setting up a database separately
- Finding or writing an MCP server wrapper
- Configuring and running two separate processes
- Keeping everything in sync

**Cortex removes all of that.** One library. Zero configuration. Everything just works.

---

## Features

- **Embedded** — no server, no setup, just a `.ctx` file
- **MCP Native** — built-in MCP server, auto-starts on connect
- **Three Transports** — stdio, HTTP+SSE, and WebSocket
- **Optional Auth** — API key authentication for HTTP and WebSocket
- **Multi-language** — Python now, JavaScript and Java coming soon
- **AI-native** — works with Claude Desktop, Cursor, LangChain, CrewAI, AutoGen, n8n, and any MCP-compatible agent

---

## Installation

```bash
pip install cortexdb-mcp
```

---

## Quick Start

```python
import cortex

# Connect — MCP server starts automatically
db = cortex.connect("myapp.ctx")

# Use like a normal database
db.execute("CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT)")
db.execute("INSERT INTO users VALUES (1, 'Alice')")

# Fetch data
rows = db.fetch("SELECT * FROM users")
for row in rows:
    print(row)  # {'id': 1, 'name': 'Alice'}

# Fetch single row
user = db.fetchone("SELECT * FROM users WHERE id = 1")

# Close
db.close()
```

---

## MCP Transports

### stdio (default)
Best for local AI agents like Claude Desktop and Cursor.
```python
db = cortex.connect("myapp.ctx")
# or explicitly:
db = cortex.connect("myapp.ctx", transport="stdio")
```

### HTTP + SSE
Best for remote agents and agentic platforms like n8n, LangChain cloud, and AutoGen.
```python
db = cortex.connect("myapp.ctx", transport="http", port=5173)
# MCP available at: http://localhost:5173/mcp
# SSE available at: http://localhost:5173/sse
```

### WebSocket
Best for real-time streaming use cases.
```python
db = cortex.connect("myapp.ctx", transport="websocket", port=5173)
# WebSocket available at: ws://localhost:5173/ws
```

### All transports at once
```python
db = cortex.connect("myapp.ctx", transport="all", port=5173)
```

---

## Authentication

By default, Cortex runs without authentication. For HTTP and WebSocket transports, you can optionally add API key auth:

```python
# With API key auth
db = cortex.connect("myapp.ctx", transport="http", port=5173, api_key="your_secret_key")
```

Clients must include the key in request headers:
```
X-API-Key: your_secret_key
```

---

## MCP Tools

Cortex exposes these tools to AI agents via MCP:

| Tool | Description |
|------|-------------|
| `cortex_query` | Run SELECT queries and return results |
| `cortex_execute` | Run INSERT, UPDATE, DELETE, CREATE statements |
| `cortex_tables` | List all tables in the database |
| `cortex_schema` | Get schema of a table or all tables |

---

## Connect to Claude Desktop

Add this to your Claude Desktop config (`claude_desktop_config.json`):

```json
{
  "mcpServers": {
    "cortex": {
      "command": "python",
      "args": ["-m", "cortex", "--db", "myapp.ctx"]
    }
  }
}
```

Claude can now query your database directly in conversation.

---

## Connect to LangChain / LangGraph

```python
from langchain_mcp import MCPClient

client = MCPClient("http://localhost:5173/mcp", api_key="your_key")
# Use cortex tools in your LangChain agent
```

---

## Architecture

```
┌─────────────────────────────────┐
│         AI Agent / LLM          │
│  Claude, GPT, LangChain, etc.   │
└────────────┬────────────────────┘
             │ MCP Protocol
┌────────────▼────────────────────┐
│         Cortex MCP Server       │
│  stdio / HTTP+SSE / WebSocket   │
└────────────┬────────────────────┘
             │
┌────────────▼────────────────────┐
│         Python API              │
│  connect() / execute() / fetch()│
└────────────┬────────────────────┘
             │
┌────────────▼────────────────────┐
│         C Core (libcortex)      │
│     SQLite-based storage        │
│     .ctx file format            │
└─────────────────────────────────┘
```

---

## API Reference

### `cortex.connect(path, transport, port, api_key)`

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `path` | str | required | Path to `.ctx` database file |
| `transport` | str | `"stdio"` | Transport: `stdio`, `http`, `websocket`, `all` |
| `port` | int | `5173` | Port for HTTP/WebSocket transports |
| `api_key` | str | `None` | Optional API key for auth |

### `db.execute(sql)`
Run INSERT, UPDATE, DELETE, or CREATE statements.

### `db.fetch(sql)`
Run a SELECT query and return all rows as a list of dicts.

### `db.fetchone(sql)`
Run a SELECT query and return the first row as a dict.

### `db.close()`
Close the database connection.

---

## Roadmap

- [x] Core embedded database engine
- [x] Python library (`pip install cortexdb-mcp`)
- [x] MCP server with stdio transport
- [x] HTTP + SSE transport
- [x] WebSocket transport
- [x] Optional API key authentication
- [ ] JavaScript / Node.js bindings
- [ ] Java bindings
- [ ] Vector / semantic search
- [ ] Natural language querying
- [ ] Cortex PG (PostgreSQL backend)
- [ ] Cortex Mongo (MongoDB backend)
- [ ] Docker support
- [ ] Admin dashboard UI

---

## Contributing

Contributions are welcome! Please open an issue first to discuss what you'd like to change.

```bash
git clone https://github.com/nileshshinde09/cortexdb
cd cortexdb
uv sync
uv run pytest tests/
```

---

## License

MIT License — see [LICENSE](LICENSE) for details.

---

## About

Cortex is built by [@nileshshinde09](https://github.com/nileshshinde09).

If you find Cortex useful, please ⭐ star the repo — it helps others discover it.

> *"The first embedded database built natively for AI agents"*