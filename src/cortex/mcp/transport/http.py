import asyncio
import threading
import uvicorn
from mcp.server import Server
from mcp.server.sse import SseServerTransport
from starlette.applications import Starlette
from starlette.requests import Request
from starlette.responses import JSONResponse, Response
from starlette.routing import Route
from starlette.middleware.base import BaseHTTPMiddleware
from ..auth.apikey import APIKeyAuth


class AuthMiddleware(BaseHTTPMiddleware):
    def __init__(self, app, auth: APIKeyAuth):
        super().__init__(app)
        self.auth = auth

    async def dispatch(self, request: Request, call_next):
        if request.url.path == "/health":
            return await call_next(request)
        if not self.auth.authenticate(dict(request.headers)):
            return JSONResponse(
                {"error": "Unauthorized - Invalid or missing API key"},
                status_code=401
            )
        return await call_next(request)


def create_http_app(mcp_server: Server, auth: APIKeyAuth):
    sse_transport = SseServerTransport("/messages/")

    async def handle_sse(request: Request):
        async with sse_transport.connect_sse(
            request.scope,
            request.receive,
            request._send,
        ) as (read_stream, write_stream):
            await mcp_server.run(
                read_stream,
                write_stream,
                mcp_server.create_initialization_options(),
            )
        return Response()

    async def handle_messages(request: Request):
        await sse_transport.handle_post_message(
            request.scope,
            request.receive,
            request._send,
        )
        return Response(status_code=202)

    async def health(request: Request):
        return JSONResponse({"status": "ok", "server": "cortex"})

    async def root(request: Request):
        return JSONResponse({
            "server": "cortex",
            "version": "0.1.0",
            "endpoints": {
                "health": "/health",
                "sse": "/sse",
                "messages": "/messages/"
            }
        })

    routes = [
        Route("/", root, methods=["GET"]),
        Route("/health", health, methods=["GET"]),
        Route("/sse", handle_sse, methods=["GET"]),
        Route("/messages/", handle_messages, methods=["POST"]),
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