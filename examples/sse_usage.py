import cortex
import time

db = cortex.connect("./test.ctx", transport="websocket", port=5173)

db.execute("CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT)")
db.execute("INSERT INTO users VALUES (1, 'Alice')")

rows = db.fetch("SELECT * FROM users")
for row in rows:
    print(row)

print("\nWebSocket server running at ws://localhost:5173/ws")
print("Press Ctrl+C to stop")
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    db.close()