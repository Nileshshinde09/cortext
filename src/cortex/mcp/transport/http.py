import asyncio
import threading
import json
from starlette.applications import Starlette
from starlette.requests import Request
from starlette.responses import JSONResponse
from starlette.routing import Route, Mount
from starlette.middleware.base import BaseHTTPMiddleware
from sse_starlette.sse import EventSourceResponse
import uvicorn
from mcp.server import Server
from ..auth.apikey import APIKeyAuth


class AuthMiddleware(BaseHTTPMiddleware):
    def __init__(self, app, auth: APIKeyAuth):
        super().__init__(app)
        self.auth = auth

    async def dispatch(self, request: Request, call_next):
        if not self.auth.authenticate(dict(request.headers)):
            return JSONResponse(
                {"error": "Unauthorized - Invalid or missing API key"},
                status_code=401
            )
        return await call_next(request)


def create_http_app(mcp_server: Server, auth: APIKeyAuth):
    async def mcp_endpoint(request: Request):
        try:
            body = await request.json()
            response = await mcp_server.handle_request(body)
            return JSONResponse(response)
        except Exception as e:
            return JSONResponse({"error": str(e)}, status_code=500)

    async def sse_endpoint(request: Request):
        async def event_generator():
            yield {
                "event": "connected",
                "data": json.dumps({"status": "Cortex MCP Server connected"})
            }
            # Keep connection alive
            while True:
                await asyncio.sleep(30)
                yield {
                    "event": "ping",
                    "data": json.dumps({"status": "alive"})
                }

        return EventSourceResponse(event_generator())

    async def health(request: Request):
        return JSONResponse({"status": "ok", "server": "cortex"})

    async def root(request: Request):
        return JSONResponse({
            "server": "cortex",
            "version": "0.1.0",
            "endpoints": {
                "health": "/health",
                "mcp": "/mcp",
                "sse": "/sse"
            }
        })

    routes = [
        Route("/", root, methods=["GET"]),
        Route("/health", health, methods=["GET"]),
        Route("/mcp", mcp_endpoint, methods=["POST"]),
        Route("/sse", sse_endpoint, methods=["GET"]),
    ]

    app = Starlette(routes=routes)

    if auth.enabled:
        app.add_middleware(AuthMiddleware, auth=auth)

    return app


def start_http(mcp_server: Server, port: int, auth: APIKeyAuth):
    app = create_http_app(mcp_server, auth)

    def run_in_thread():
        uvicorn.run(
            app,
            host="0.0.0.0",
            port=port,
            log_level="error"
        )

    thread = threading.Thread(target=run_in_thread, daemon=True)
    thread.name = "cortex-http"
    thread.start()
    return thread