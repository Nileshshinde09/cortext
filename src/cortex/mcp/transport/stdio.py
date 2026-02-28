import asyncio
import threading
from mcp.server import Server
from mcp.server.stdio import stdio_server


def start_stdio(app: Server):
    async def run():
        async with stdio_server() as (read_stream, write_stream):
            await app.run(
                read_stream,
                write_stream,
                app.create_initialization_options()
            )

    def run_in_thread():
        asyncio.run(run())

    thread = threading.Thread(target=run_in_thread, daemon=True)
    thread.start()
    return thread