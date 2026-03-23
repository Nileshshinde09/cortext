import pytest
import os
import time
import httpx
import json
import threading
import cortex
from cortex.connection import CortexConnection

TEST_DB = "./test_http.ctx"
HTTP_PORT = 5182


def cleanup():
    if os.path.exists(TEST_DB):
        os.remove(TEST_DB)


# ── SSE Session Helper ─────────────────────
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
                            # DO NOT break — keep reading to keep connection alive
            except Exception:
                pass

        self._thread = threading.Thread(target=run, daemon=True)
        self._thread.start()
        self._ready.wait(timeout=5)
        return self.session_id

    def stop(self):
        self._stop.set()

# ── Helpers ────────────────────────────────
def get_session_id(port) -> str:
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
    print("*"*100)
    print(payload)
    print("*" * 100)
    return httpx.post(
        f"http://localhost:{port}/messages/?session_id={session_id}",
        json=payload,
        timeout=5
    )


# ── Fixture ────────────────────────────────
@pytest.fixture(scope="module")
def http_db():
    cleanup()
    db = cortex.connect(TEST_DB, transport="http", port=HTTP_PORT)
    db.execute("CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT, email TEXT)")
    db.execute("INSERT INTO users VALUES (1, 'Alice', 'alice@email.com')")
    db.execute("INSERT INTO users VALUES (2, 'Bob', 'bob@email.com')")
    db.execute("INSERT INTO users VALUES (3, 'Charlie', 'charlie@email.com')")
    time.sleep(1.5)
    yield db
    db.close()
    cleanup()



def get_session_id(port) -> str:
    """Connect to SSE and extract session_id from the endpoint URL"""
    session_id = None
    with httpx.stream("GET", f"http://localhost:{port}/sse", timeout=5) as r:
        for line in r.iter_lines():
            # SSE sends: data: /messages/?session_id=xxxxx
            if line.startswith("data:"):
                value = line[5:].strip()
                # Extract session_id from URL
                if "session_id=" in value:
                    session_id = value.split("session_id=")[-1].strip()
                    break
                # Or try parsing as JSON
                try:
                    data = json.loads(value)
                    if "session_id" in data:
                        session_id = data["session_id"]
                        break
                except json.JSONDecodeError:
                    continue
    return session_id


def send_mcp(port, session_id, method, params=None):
    """Send MCP JSON-RPC request to messages endpoint"""
    payload = {
        "jsonrpc": "2.0",
        "id": 1,
        "method": method,
        "params": params or {}
    }
    response = httpx.post(
        f"http://localhost:{port}/messages/?session_id={session_id}",
        json=payload,
        timeout=5
    )
    return response


# ─────────────────────────────────────────
# FLOW 1 — Server Health
# ─────────────────────────────────────────

class TestHTTPServerHealth:

    def test_server_starts(self, http_db):
        response = httpx.get(f"http://localhost:{HTTP_PORT}/health")
        assert response.status_code == 200

    def test_health_returns_ok(self, http_db):
        response = httpx.get(f"http://localhost:{HTTP_PORT}/health")
        data = response.json()
        assert data["status"] == "ok"
        assert data["server"] == "cortex"

    def test_root_endpoint(self, http_db):
        response = httpx.get(f"http://localhost:{HTTP_PORT}/")
        assert response.status_code == 200

    def test_sse_endpoint_reachable(self, http_db):
        with httpx.stream("GET", f"http://localhost:{HTTP_PORT}/sse", timeout=3) as r:
            assert r.status_code == 200


# ─────────────────────────────────────────
# FLOW 2 — Session ID
# ─────────────────────────────────────────

class TestSSESession:

    def test_sse_returns_session_id(self, http_db):
        """SSE connection returns a session_id"""
        session_id = get_session_id(HTTP_PORT)
        assert session_id is not None
        assert len(session_id) > 0

    def test_session_id_is_unique(self, http_db):
        """Each SSE connection gets a unique session_id"""
        session1 = get_session_id(HTTP_PORT)
        session2 = get_session_id(HTTP_PORT)
        assert session1 != session2


# ─────────────────────────────────────────
# FLOW 3 — MCP Protocol
# ─────────────────────────────────────────

class TestMCPHTTPFlow:

    def test_mcp_initialize(self, http_db):
        """MCP initialization handshake works"""
        session_id = get_session_id(HTTP_PORT)
        assert session_id is not None

        r = send_mcp(HTTP_PORT, session_id, "initialize", {
            "protocolVersion": "2024-11-05",
            "capabilities": {},
            "clientInfo": {"name": "test-client", "version": "1.0"}
        })
        assert r.status_code == 202

    def test_mcp_list_tools(self, http_db):
        """MCP returns list of tools"""
        session_id = get_session_id(HTTP_PORT)
        r = send_mcp(HTTP_PORT, session_id, "tools/list")
        assert r.status_code == 202

    def test_mcp_cortex_query(self, http_db):
        """AI agent queries database via MCP"""
        session_id = get_session_id(HTTP_PORT)
        r = send_mcp(HTTP_PORT, session_id, "tools/call", {
            "name": "cortex_query",
            "arguments": {"sql": "SELECT * FROM users"}
        })
        assert r.status_code == 202

    def test_mcp_cortex_execute(self, http_db):
        session = SSESession(HTTP_PORT)
        session_id = session.start()
        assert session_id is not None
        time.sleep(0.5)  # let SSE connection stabilize

        r = send_mcp(HTTP_PORT, session_id, "tools/call", {
            "name": "cortex_execute",
            "arguments": {
                "sql": "INSERT INTO users VALUES (4, 'Dave', 'dave@email.com')"
            }
        })
        assert r.status_code == 202
        time.sleep(0.5)
        row = http_db.fetchone("SELECT * FROM users WHERE id = 4")
        rows= http_db.fetch("SELECT * FROM users")
        print(rows)
        assert row is not None
        assert row["name"] == "Dave"

    def test_mcp_cortex_tables(self, http_db):
        """AI agent lists tables via MCP"""
        session_id = get_session_id(HTTP_PORT)
        r = send_mcp(HTTP_PORT, session_id, "tools/call", {
            "name": "cortex_tables",
            "arguments": {}
        })
        assert r.status_code == 202

    def test_mcp_cortex_schema(self, http_db):
        """AI agent gets schema via MCP"""
        session_id = get_session_id(HTTP_PORT)
        r = send_mcp(HTTP_PORT, session_id, "tools/call", {
            "name": "cortex_schema",
            "arguments": {"table": "users"}
        })
        assert r.status_code == 202


# ─────────────────────────────────────────
# FLOW 4 — Authentication
# ─────────────────────────────────────────

class TestHTTPAuth:

    @pytest.fixture(scope="class")
    def auth_db(self):
        if os.path.exists("./auth_test.ctx"):
            os.remove("./auth_test.ctx")
        db = cortex.connect(
            "./auth_test.ctx",
            transport="http",
            port=5183,
            api_key="test_secret_key"
        )
        time.sleep(1.5)
        yield db
        db.close()
        if os.path.exists("./auth_test.ctx"):
            os.remove("./auth_test.ctx")

    def test_no_key_rejected(self, auth_db):
        r = httpx.get("http://localhost:5183/sse", timeout=3)
        assert r.status_code == 401

    def test_wrong_key_rejected(self, auth_db):
        r = httpx.get(
            "http://localhost:5183/sse",
            headers={"X-API-Key": "wrong_key"},
            timeout=3
        )
        assert r.status_code == 401

    def test_correct_key_accepted(self, auth_db):
        with httpx.stream(
            "GET",
            "http://localhost:5183/sse",
            headers={"X-API-Key": "test_secret_key"},
            timeout=3
        ) as r:
            assert r.status_code == 200

    def test_health_always_accessible(self, auth_db):
        r = httpx.get("http://localhost:5183/health")
        assert r.status_code == 200


# ─────────────────────────────────────────
# FLOW 5 — Complete Agent Simulation
# ─────────────────────────────────────────

class TestCompleteAgentFlow:

    def test_full_agent_workflow(self, http_db):
        """
        Complete AI agent workflow:
        1. Connect via SSE → get session
        2. Initialize MCP
        3. List tools
        4. Query data
        5. Insert data
        6. Verify insertion
        """
        base = f"http://localhost:{HTTP_PORT}"

        # Step 1 — Get session
        session_id = get_session_id(HTTP_PORT)
        assert session_id is not None

        # Step 2 — Initialize
        r = send_mcp(HTTP_PORT, session_id, "initialize", {
            "protocolVersion": "2024-11-05",
            "capabilities": {},
            "clientInfo": {"name": "ai-agent", "version": "1.0"}
        })
        assert r.status_code == 202

        # Step 3 — List tools
        r = send_mcp(HTTP_PORT, session_id, "tools/list")
        assert r.status_code == 202

        # Step 4 — Query
        r = send_mcp(HTTP_PORT, session_id, "tools/call", {
            "name": "cortex_query",
            "arguments": {"sql": "SELECT * FROM users"}
        })
        assert r.status_code == 202

        # Step 5 — Insert
        r = send_mcp(HTTP_PORT, session_id, "tools/call", {
            "name": "cortex_execute",
            "arguments": {
                "sql": "INSERT INTO users VALUES (99, 'Agent', 'agent@ai.com')"
            }
        })
        assert r.status_code == 202

        # Step 6 — Verify
        row = http_db.fetchone("SELECT * FROM users WHERE id = 99")
        assert row is not None
        assert row["name"] == "Agent"