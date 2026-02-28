from .connection import CortexConnection

def connect(path: str) -> CortexConnection:
    return CortexConnection(path)

__version__ = "0.1.0"
__all__ = ["connect", "CortexConnection"]