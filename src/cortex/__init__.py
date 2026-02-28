from .connection import CortexConnection


def connect(
    path: str,
    transport: str = "stdio",
    port: int = 5173,
    api_key: str = None
) -> CortexConnection:
    return CortexConnection(path, transport=transport, port=port, api_key=api_key)


__version__ = "0.1.0"
__all__ = ["connect", "CortexConnection"]