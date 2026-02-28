from .server import app, set_connection
from .transport.stdio import start_stdio
from .transport.http import start_http
from .transport.websocket import start_websocket
from .auth.apikey import APIKeyAuth
import sys


def start_mcp(db, transport: str = "stdio", port: int = 5173, api_key: str = None):
    set_connection(db)
    auth = APIKeyAuth(api_key)

    _print_banner(transport, port, auth)

    if transport == "stdio":
        # Only start stdio if being called as MCP server
        # Not when running as a regular Python script
        if not sys.stdin.isatty():
            start_stdio(app)

    elif transport == "http":
        start_http(app, port, auth)

    elif transport == "websocket":
        start_websocket(app, port, auth)

    elif transport == "all":
        if not sys.stdin.isatty():
            start_stdio(app)
        start_http(app, port, auth)
        start_websocket(app, port + 1, auth)


def _print_banner(transport: str, port: int, auth: APIKeyAuth):
    print("━" * 40)
    print("  Cortex MCP Server")
    print("━" * 40)

    if transport in ("http", "all"):
        print(f"  HTTP+SSE  : http://localhost:{port}/mcp")
        print(f"  SSE       : http://localhost:{port}/sse")

    if transport in ("websocket", "all"):
        ws_port = port + 1 if transport == "all" else port
        print(f"  WebSocket : ws://localhost:{ws_port}/ws")

    if transport == "stdio":
        print("  Transport : stdio")

    if auth.enabled:
        print(f"  Auth      : enabled")
        print(f"  API Key   : {auth.api_key}")
    else:
        print("  Auth      : disabled")

    print("━" * 40)