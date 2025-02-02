import sqlite3, time, os
from datetime import datetime

# Expand the user directory
db_path = os.path.expanduser("~/project/SynapSense/network_logger.db")

def create_tables():
    # Connect to the SQLite database (or create it if it doesn't exist)

    # Ensure the directory exists
    os.makedirs(os.path.dirname(db_path), exist_ok=True)

    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    # Create the "Network Logger" table if it doesn't exist
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS NetworkLogger (
            URL TEXT NOT NULL CHECK(length(URL) <= 300),
            PATH TEXT NOT NULL CHECK(length(PATH) <= 150),
            TIME TEXT NOT NULL
        )
    ''')

    # Create the "Check-ins" table if it doesn't exist
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS CheckIns (
            USER_SCORE INTEGER NOT NULL CHECK(INTEGER BETWEEN 1 AND 5),
            TIME TEXT NOT NULL
        )
    ''')

    # Commit changes and close the connection
    conn.commit()
    conn.close()

def insert_network_items(website_url: str, path: str, time_original: float):
    create_tables()
    # Connect to the SQLite database
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    # Insert each path associated with the URL
    cursor.execute('''
        INSERT INTO NetworkLogger (URL, PATH, TIME)
        VALUES (?, ?, ?)
    ''', (website_url, path, str(float(time.perf_counter()) - float(time_original))))

    # Commit changes and close the connection
    conn.commit()
    conn.close()

def insert_network_items(rating: int, time_original):
    create_tables()
    # Connect to the SQLite database
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    # Insert each path associated with the URL
    cursor.execute('''
        INSERT INTO CheckIns (USER_SCORE, TIME)
        VALUES (?, ?)
    ''', (int(rating), str(float(time.perf_counter()) - float(time_original))))

    # Commit changes and close the connection
    conn.commit()
    conn.close()

def find_network_items():
    create_tables()
    # Connect to the SQLite database
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    # Retrieve all entries from the "NetworkLogger" table
    cursor.execute('SELECT URL, PATH, TIME FROM NetworkLogger')
    rows = cursor.fetchall()

    # Close the connection
    conn.close()

    return rows

def find_user_scores():
    create_tables()
    # Connect to the SQLite database
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    # Retrieve all entries from the "CheckIns" table
    cursor.execute('SELECT USER_SCORE, TIME FROM CheckIns')
    rows = cursor.fetchall()

    # Close the connection
    conn.close()

    return rows

