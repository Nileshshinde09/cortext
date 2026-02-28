import cortex
import time

db = cortex.connect("./test.ctx", transport="http", port=5173)

db.execute("CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT)")
db.execute("INSERT INTO users VALUES (1, 'Alice')")

rows = db.fetch("SELECT * FROM users")
for row in rows:
    print(row)

# Keep alive so HTTP server stays running
print("\nHTTP server running at http://localhost:5173")
print("Press Ctrl+C to stop")
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    db.close()