"""
Example Usage of Cortex Database

This script demonstrates how to:
1. Connect to a Cortex database
2. Create a table
3. Insert records
4. Fetch data
5. Close the connection

Author: (optional)
"""

import cortex
import os
db_path_url="./test.ctx"
def main():
    """
    Main function demonstrating basic CRUD operations
    using the Cortex embedded database.
    """

    # -----------------------------
    # Connect to database
    # -----------------------------
    db = cortex.connect(db_path_url)
    print("Connected to database.")

    # -----------------------------
    # Create table
    # -----------------------------
    db.execute(
        "CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT)"
    )
    print("Table 'users' ensured.")

    # -----------------------------
    # Insert data
    # -----------------------------
    db.execute("INSERT INTO users VALUES (3, 'Ajay')")
    db.execute("INSERT INTO users VALUES (4, 'Varun')")
    print("Data inserted successfully!")

    # -----------------------------
    # Fetch data
    # -----------------------------
    result = db.execute("SELECT * FROM users")

    print("\nFetched Data:")
    print(result)

    print("Data fetched successfully!")

    # -----------------------------
    # Close connection
    # -----------------------------
    db.close()
    print("Database connection closed.")

    # -----------------------------
    # Deleting db file
    # -----------------------------
    if os.path.exists(db_path_url):
        os.remove(db_path_url)

if __name__ == "__main__":
    main()