import cortex

# Connect to database
db = cortex.connect("test.ctx")

# Create table
db.execute("CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT)")

# Insert data
db.execute("INSERT INTO users VALUES (1, 'Alice')")
db.execute("INSERT INTO users VALUES (2, 'Bob')")

print("Data inserted successfully!")

# Close
db.close()