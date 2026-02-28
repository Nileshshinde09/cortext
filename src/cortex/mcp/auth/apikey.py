class APIKeyAuth:
    def __init__(self, api_key: str = None):
        self.api_key = api_key
        self.enabled = api_key is not None

    def verify(self, request_key: str) -> bool:
        if not self.enabled:
            return True
        return request_key == self.api_key

    def extract_key(self, headers: dict) -> str:
        return headers.get("x-api-key") or headers.get("X-API-Key")

    def authenticate(self, headers: dict) -> bool:
        if not self.enabled:
            return True
        key = self.extract_key(headers)
        return self.verify(key)