"""
Usage:
  # in CortexConnection.__init__
  self.auth = APIKeyAuth(api_key)

  # issue token once on connect — hand this back to the MCP client
  token = self.auth.create_token(path)
  print(f"Cortex token: {token}")

  # in server.py call_tool — verify on every request
  if not _auth.authenticate(request_headers):
      return [TextContent(type="text", text="Error: Unauthorized")]
"""

import os
import jwt
from datetime import datetime, timedelta, timezone
from ...exceptions import CortexInterfaceError


class APIKeyAuth:
    def __init__(self, api_key: str | None = None):
        # Secret used to sign and verify JWT tokens.
        # Falls back to env var so it's never hardcoded in production.
        self.secret = api_key or os.environ.get("CORTEX_SECRET_KEY")
        self.enabled = self.secret is not None

    def create_token(self, db_path: str) -> str:
        """Issue a signed JWT tied to a specific database path."""
        if not self.enabled:
            raise CortexInterfaceError("Auth is disabled — no secret key provided")
        payload = {
            "sub": db_path,
            "iat": datetime.now(timezone.utc),
            "exp": datetime.now(timezone.utc) + timedelta(hours=1)
        }
        return jwt.encode(payload, self.secret, algorithm="HS256")

    def verify(self, token: str) -> dict:
        """Verify signature and expiry. Returns decoded payload if valid."""
        try:
            return jwt.decode(token, self.secret, algorithms=["HS256"])
        except jwt.ExpiredSignatureError:
            raise CortexInterfaceError("Token has expired")
        except jwt.InvalidTokenError:
            raise CortexInterfaceError("Invalid or tampered token")

    def extract_token(self, headers: dict) -> str | None:
        # Accepts both 'Authorization: Bearer <token>' and raw 'x-api-key' header
        auth_header = headers.get("Authorization") or headers.get("authorization")
        if auth_header and auth_header.startswith("Bearer "):
            return auth_header[len("Bearer "):]
        return headers.get("x-api-key") or headers.get("X-API-Key")

    def authenticate(self, headers: dict) -> bool:
        # __exit__ pattern — if disabled, always pass through
        if not self.enabled:
            return True
        token = self.extract_token(headers)
        if not token:
            return False
        self.verify(token)  # raises CortexInterfaceError if invalid
        return True