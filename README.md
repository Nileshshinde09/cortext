cortex/
├── .github/
│   └── workflows/
│       └── ci.yml                 # GitHub Actions CI
│
├── src/
│   └── cortex/
│       ├── __init__.py            # cortex.connect() lives here
│       ├── connection.py          # Connection class, auto-starts MCP
│       ├── cursor.py              # Cursor for executing queries
│       ├── mcp/
│       │   ├── __init__.py
│       │   ├── server.py          # MCP server setup
│       │   └── tools.py           # All MCP tool definitions
│       ├── core/
│       │   ├── __init__.py
│       │   └── bindings.py        # ctypes bindings to libcortex.so
│       └── exceptions.py          # CortexError, ConnectionError etc
│
├── c/
│   ├── libcortex.c                # Your C core
│   ├── libcortex.h                # Header file
│   └── CMakeLists.txt             # Build config for C library
│
├── tests/
│   ├── test_connection.py
│   ├── test_query.py
│   └── test_mcp.py
│
├── examples/
│   ├── basic_usage.py             # Simple connect and query example
│   └── ai_agent_demo.py           # AI agent talking to Cortex via MCP
│
├── docs/
│   └── index.md
│
├── pyproject.toml                 # uv project config
├── .python-version                # Python version for uv
├── .gitignore
├── LICENSE
└── README.md