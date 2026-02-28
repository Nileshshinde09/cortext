import cortex
import os
import pytest

TEST_DB = "./test_test.ctx"

@pytest.fixture
def db():
    """
    Setup a fresh database with multiple tables
    for testing joins and subqueries.
    """
    if os.path.exists(TEST_DB):
        os.remove(TEST_DB)

    db = cortex.connect(TEST_DB)

    # Create tables
    db.execute("CREATE TABLE users (id INTEGER, name TEXT)")
    db.execute("CREATE TABLE orders  (id INTEGER, user_id INTEGER, amount INTEGER)")

    yield db

    db.close()
    if os.path.exists(TEST_DB):
        os.remove(TEST_DB)


# -----------------------------
# BASIC TESTS
# -----------------------------

def test_insert_and_fetch_all(db):
    """Insert multiple users and fetch all records."""
    db.execute("INSERT INTO users VALUES (1, 'Alice')")
    db.execute("INSERT INTO users VALUES (2, 'Bob')")

    rows = db.fetch("SELECT * FROM users")

    assert len(rows) == 2
    assert rows[0]["name"] == "Alice"
    assert rows[1]["name"] == "Bob"


def test_empty_table(db):
    """Fetching from empty table should return empty list."""
    rows = db.fetch("SELECT * FROM users")
    assert rows == []


# -----------------------------
# JOIN TESTS
# -----------------------------

def test_inner_join(db):
    """Test INNER JOIN between users and orders."""
    db.execute("INSERT INTO users VALUES (1, 'Alice')")
    db.execute("INSERT INTO orders VALUES (101, 1, 500)")

    rows = db.fetch("""
        SELECT users.name, orders.amount
        FROM users
        INNER JOIN orders ON users.id = orders.user_id
    """)

    assert len(rows) == 1
    assert rows[0]["name"] == "Alice"
    assert rows[0]["amount"] == 500


def test_left_join(db):
    """Test LEFT JOIN to include users without orders."""
    db.execute("INSERT INTO users VALUES (1, 'Alice')")
    db.execute("INSERT INTO users VALUES (2, 'Bob')")
    db.execute("INSERT INTO orders VALUES (101, 1, 500)")

    rows = db.fetch("""
        SELECT users.name, orders.amount
        FROM users
        LEFT JOIN orders ON users.id = orders.user_id
    """)

    assert len(rows) == 2


def test_join_no_match(db):
    """Join where no matching foreign key exists."""
    db.execute("INSERT INTO users VALUES (1, 'Alice')")
    db.execute("INSERT INTO orders VALUES (101, 99, 500)")

    rows = db.fetch("""
        SELECT users.name, orders.amount
        FROM users
        INNER JOIN orders ON users.id = orders.user_id
    """)

    assert rows == []


# -----------------------------
# SUBQUERY TESTS
# -----------------------------

def test_subquery_where(db):
    """Subquery inside WHERE clause."""
    db.execute("INSERT INTO users VALUES (1, 'Alice')")
    db.execute("INSERT INTO users VALUES (2, 'Bob')")
    db.execute("INSERT INTO orders VALUES (101, 1, 500)")

    rows = db.fetch("""
        SELECT name FROM users
        WHERE id IN (
            SELECT user_id FROM orders WHERE amount > 100
        )
    """)

    assert len(rows) == 1
    assert rows[0]["name"] == "Alice"


def test_subquery_scalar(db):
    """Scalar subquery returning single value."""
    db.execute("INSERT INTO orders VALUES (101, 1, 500)")
    db.execute("INSERT INTO orders VALUES (102, 1, 300)")

    rows = db.fetch("""
        SELECT (SELECT MAX(amount) FROM orders) as max_amount
    """)

    assert rows[0]["max_amount"] == 500


def test_subquery_no_result(db):
    """Subquery returning no rows should behave correctly."""
    rows = db.fetch("""
        SELECT * FROM users
        WHERE id IN (SELECT user_id FROM orders)
    """)

    assert rows == []


# -----------------------------
# EDGE CASES
# -----------------------------

def test_duplicate_insert(db):
    """Insert duplicate values (no constraint case)."""
    db.execute("INSERT INTO users VALUES (1, 'Alice')")
    db.execute("INSERT INTO users VALUES (1, 'Alice')")

    rows = db.fetch("SELECT * FROM users")

    assert len(rows) == 2


def test_invalid_query(db):
    """Invalid SQL should raise an error."""
    with pytest.raises(Exception):
        db.execute("SELECT * FORM users")  # typo: FORM


def test_null_values(db):
    """Insert NULL values and verify handling."""
    db.execute("INSERT INTO users VALUES (1, NULL)")

    rows = db.fetch("SELECT * FROM users")

    assert rows[0]["name"] is None


def test_large_insert(db):
    """Test performance with multiple inserts."""
    for i in range(100):
        db.execute(f"INSERT INTO users VALUES ({i}, 'User{i}')")

    rows = db.fetch("SELECT * FROM users")

    assert len(rows) == 100

@pytest.mark.parametrize("user_id,name", [
    (i, f"User{i}") for i in range(50)
])
def test_bulk_insert_users(db, user_id, name):
    """Test inserting multiple users dynamically."""
    db.execute(f"INSERT INTO users VALUES ({user_id}, '{name}')")

    result = db.fetchone(f"SELECT * FROM users WHERE id = {user_id}")

    assert result["id"] == user_id
    assert result["name"] == name


# -----------------------------
# BULK ORDER TESTS (20 cases)
# -----------------------------

@pytest.mark.parametrize("order_id,user_id,amount", [
    (i, i % 5, i * 100) for i in range(20)
])
def test_bulk_orders(db, order_id, user_id, amount):
    """Test inserting multiple orders."""
    db.execute(f"INSERT INTO users VALUES ({user_id}, 'User{user_id}')")
    db.execute(f"INSERT INTO orders VALUES ({order_id}, {user_id}, {amount})")

    result = db.fetchone(f"SELECT * FROM orders WHERE id = {order_id}")

    assert result["amount"] == amount


# -----------------------------
# JOIN VARIATIONS (10 cases)
# -----------------------------

@pytest.mark.parametrize("uid,amount", [
    (i, i * 10) for i in range(10)
])
def test_join_variations(db, uid, amount):
    """Test JOIN behavior with multiple combinations."""
    db.execute(f"INSERT INTO users VALUES ({uid}, 'User{uid}')")
    db.execute(f"INSERT INTO orders VALUES ({uid}, {uid}, {amount})")

    rows = db.fetch("""
        SELECT users.name, orders.amount
        FROM users
        INNER JOIN orders ON users.id = orders.user_id
    """)

    assert any(r["amount"] == amount for r in rows)


# -----------------------------
# SUBQUERY VARIATIONS (10 cases)
# -----------------------------

@pytest.mark.parametrize("amount", [i * 50 for i in range(10)])
def test_subquery_amount_filter(db, amount):
    """Test subquery filters with different amounts."""
    db.execute("INSERT INTO users VALUES (1, 'Alice')")
    db.execute(f"INSERT INTO orders VALUES (1, 1, {amount})")

    rows = db.fetch(f"""
        SELECT name FROM users
        WHERE id IN (
            SELECT user_id FROM orders WHERE amount >= {amount}
        )
    """)

    assert len(rows) == 1


# -----------------------------
# EDGE CASES (10 cases)
# -----------------------------

@pytest.mark.parametrize("value", [
    None, "", "A", "VeryLongName123456789", "🔥", "NULL", "123", "   ", "Name", "Test"
])
def test_edge_name_values(db, value):
    """Test edge cases for name values."""
    val = "NULL" if value is None else f"'{value}'"
    db.execute(f"INSERT INTO users VALUES (1, {val})")

    rows = db.fetch("SELECT * FROM users")

    assert len(rows) == 1