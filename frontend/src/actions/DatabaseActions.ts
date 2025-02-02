import sqlite3 from 'sqlite'
import { open } from 'sqlite'

export async function openDb () {
    return open({
      filename: '~/project/synapsense/network_logger.db',
      driver: sqlite3.Database
    })
}

export async function getRow() {
    const db = await openDb()
    const result = await db.get("SELECT * FROM ")
    console.log(result)
}
