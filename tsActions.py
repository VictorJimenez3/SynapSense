import psycopg2

conn = psycopg2.connect(host="localhost", user="postgres", password="hackru", database="postgres")

cursor = conn.cursor()

cursor.execute("SHOW TABLES")

conn.commit()

conn.close()
