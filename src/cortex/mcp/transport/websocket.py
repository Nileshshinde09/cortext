import asyncio
import threading
import json
from starlette.applications import Starlette
from starlette.routing import WebSocketRoute
from starlette.websockets import WebSocket
import uvicorn
from mcp.server import Server
from ..auth.apikey import APIKeyAuth


def create_websocket_app(mcp_server: Server, auth: APIKeyAuth):
    async def ws_endpoint(websocket: WebSocket):
        await websocket.accept()

        # Auth check
        if auth.enabled:
            headers = dict(websocket.headers)
            if not auth.authenticate(headers):
                await websocket.close(code=4001, reason="Unauthorized")
                return

        await websocket.send_json({
            "event": "connected",
            "data": {"status": "Cortex MCP Server connected"}
        })

        try:
            while True:
                data = await websocket.receive_text()
                body = json.loads(data)
                response = await mcp_server.handle_request(body)
                await websocket.send_json(response)
        except Exception:
            await websocket.close()

    routes = [
        WebSocketRoute("/ws", ws_endpoint),
    ]

    return Starlette(routes=routes)


def start_websocket(mcp_server: Server, port: int, auth: APIKeyAuth):
    app = create_websocket_app(mcp_server, auth)

    def run_in_thread():
        uvicorn.run(app, host="0.0.0.0", port=port, log_level="error")

    thread = threading.Thread(target=run_in_thread, daemon=True)
    thread.start()
    return thread