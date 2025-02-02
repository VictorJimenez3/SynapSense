"use server"
const sqlite3 = require('sqlite3').verbose()
const db = new sqlite3.Database("~/SynapSense/network_logger.db")


export async function getRow() {
    const row = await db.get("SELECT * FROM table")
    console.log(row)
}
