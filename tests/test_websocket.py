# import pytest
# import threading
# import time
# import websocket
# import json
# import cortex
#
# WS_URL = "ws://localhost:5173/ws"
#
#
# # ---- Start server in background ----
# def start_server():
#     db = cortex.connect("./test.ctx", transport="websocket", port=5173)
#
#     db.execute("CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT)")
#     db.execute("DELETE FROM users")  # clean state
#     db.execute("INSERT INTO users VALUES (1, 'Alice')")
#
#     # keep server alive
#     while True:
#         time.sleep(1)
# def receive_until_data(ws, timeout=5):
#     """Keep receiving until meaningful data arrives"""
#     start = time.time()
#
#     while time.time() - start < timeout:
#         try:
#             msg = ws.recv()
#             if msg:
#                 data = json.loads(msg)
#
#                 # ignore connection event
#                 if data.get("event") != "connected":
#                     return data
#         except Exception:
#             pass
#
#     return None
#
#
# @pytest.fixture(scope="module", autouse=True)
# def server():
#     thread = threading.Thread(target=start_server, daemon=True)
#     thread.start()
#
#     # give server time to start
#     time.sleep(2)
#
#     yield
#     # no teardown (daemon thread ends with process)
#
#
# # ---- Test DB via WebSocket ----
# def test_websocket_connection():
#     ws = websocket.WebSocket()
#     ws.connect(WS_URL)
#
#     assert ws.connected is True
#     ws.close()
#
# def test_fetch_users():
#     ws = websocket.WebSocket()
#     ws.connect(WS_URL)
#     ws.settimeout(3)
#
#     # consume initial event
#     try:
#         init_msg = ws.recv()
#         print("INIT:", init_msg)
#     except Exception:
#         pass
#
#     # send query
#     ws.send(json.dumps({
#         "type": "query",
#         "sql": "SELECT * FROM users"
#     }))
#
#     try:
#         response = ws.recv()
#         print("RESPONSE:", response)
#     except Exception as e:
#         pytest.fail(f"No response from server: {e}")
#
#     assert response
#     ws.close()
# def test_multiple_connections():
#     for _ in range(3):
#         ws = websocket.WebSocket()
#         ws.connect(WS_URL)
#
#         ws.recv()  # consume "connected" event
#
#         ws.send(json.dumps({
#             "type": "query",
#             "sql": "SELECT * FROM users"
#         }))
#
#         response = ws.recv()
#         assert "Alice" in response
#
#         ws.close()