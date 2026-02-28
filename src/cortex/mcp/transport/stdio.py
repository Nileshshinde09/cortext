import asyncio
import threading
from mcp.server import Server
from mcp.server.stdio import stdio_server


def start_stdio(app: Server):
    async def run():
        try:
            async with stdio_server() as (read_stream, write_stream):
                await app.run(
                    read_stream,
                    write_stream,
                    app.create_initialization_options()
                )
        except Exception:
            pass  # suppress shutdown errors

    def run_in_thread():
        try:
            asyncio.run(run())
        except Exception:
            pass  # suppress shutdown errors

    thread = threading.Thread(target=run_in_thread, daemon=True)
    thread.name = "cortex-stdio"
    thread.start()
    return thread