import cortex

db = cortex.connect("test.ctx")

# Create table
db.execute("CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT)")

# Insert data
db.execute("INSERT INTO users VALUES (1, 'Alice')")
db.execute("INSERT INTO users VALUES (2, 'Bob')")
print("Data inserted successfully!")

# Fetch all rows
rows = db.fetch("SELECT * FROM users")
print("All users:")
for row in rows:
    print(row)

# Fetch one row
user = db.fetchone("SELECT * FROM users WHERE id = 1")
print(f"\nFetched one: {user}")

db.close()