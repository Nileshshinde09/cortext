import os
import time
import httpx
import json
import threading
import cortex

TEST_DB = "./test_http.ctx"
HTTP_PORT = 5182


# ── Cleanup ───────────────────────────────
def cleanup():
    if os.path.exists(TEST_DB):
        os.remove(TEST_DB)


# ── SSE Session Helper ────────────────────
class SSESession:
    def __init__(self, port):
        self.session_id = None
        self.port = port
        self._thread = None
        self._ready = threading.Event()
        self._stop = threading.Event()

    def start(self):
        def run():
            try:
                with httpx.stream("GET", f"http://localhost:{self.port}/sse", timeout=60) as r:
                    for line in r.iter_lines():
                        if self._stop.is_set():
                            break
                        if line.startswith("data:"):
                            value = line[5:].strip()
                            if "session_id=" in value:
                                self.session_id = value.split("session_id=")[-1].strip()
                                self._ready.set()
            except Exception as e:
                print("SSE Error:", e)

        self._thread = threading.Thread(target=run, daemon=True)
        self._thread.start()
        self._ready.wait(timeout=5)
        return self.session_id

    def stop(self):
        self._stop.set()


# ── Helpers ───────────────────────────────
def get_session_id(port):
    with httpx.stream("GET", f"http://localhost:{port}/sse", timeout=5) as r:
        for line in r.iter_lines():
            if line.startswith("data:"):
                value = line[5:].strip()

                if "session_id=" in value:
                    return value.split("session_id=")[-1].strip()

                try:
                    data = json.loads(value)
                    if "session_id" in data:
                        return data["session_id"]
                except json.JSONDecodeError:
                    continue
    return None


def send_mcp(port, session_id, method, params=None):
    payload = {
        "jsonrpc": "2.0",
        "id": 1,
        "method": method,
        "params": params or {}
    }

    print("\nRequest:", payload)

    response = httpx.post(
        f"http://localhost:{port}/messages/?session_id={session_id}",
        json=payload,
        timeout=5
    )

    print("Response status:", response.status_code)
    return response


# ── Main Example Flow ─────────────────────
def main():
    cleanup()

    print("Starting Cortex HTTP DB...")
    db = cortex.connect(TEST_DB, transport="http", port=HTTP_PORT)

    # Setup DB
    db.execute("CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT, email TEXT)")
    db.execute("INSERT INTO users VALUES (1, 'Alice', 'alice@email.com')")
    db.execute("INSERT INTO users VALUES (2, 'Bob', 'bob@email.com')")
    db.execute("INSERT INTO users VALUES (3, 'Charlie', 'charlie@email.com')")

    time.sleep(1.5)

    # Step 1 — Health check
    print("\nChecking server health...")
    r = httpx.get(f"http://localhost:{HTTP_PORT}/health")
    print(r.json())

    # Step 2 — Get session
    print("\nGetting session ID...")
    session_id = get_session_id(HTTP_PORT)
    print("Session ID:", session_id)

    # Step 3 — Initialize MCP
    send_mcp(HTTP_PORT, session_id, "initialize", {
        "protocolVersion": "2024-11-05",
        "capabilities": {},
        "clientInfo": {"name": "example-client", "version": "1.0"}
    })

    # Step 4 — List tools
    send_mcp(HTTP_PORT, session_id, "tools/list")

    # Step 5 — Query data
    send_mcp(HTTP_PORT, session_id, "tools/call", {
        "name": "cortex_query",
        "arguments": {"sql": "SELECT * FROM users"}
    })

    # Step 6 — Insert data
    send_mcp(HTTP_PORT, session_id, "tools/call", {
        "name": "cortex_execute",
        "arguments": {
            "sql": "INSERT INTO users VALUES (99, 'Agent', 'agent@ai.com')"
        }
    })

    time.sleep(0.5)

    # Step 7 — Verify
    row = db.fetchone("SELECT * FROM users WHERE id = 99")
    print("\nInserted Row:", row)

    # Cleanup
    db.close()
    cleanup()
    print("\nDone.")


if __name__ == "__main__":
    main()