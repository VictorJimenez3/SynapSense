import sqlite3, sys
from sqliteAdapter import create_tables, insert_network_items, find_network_items

create_tables()

arguments = sys.argv[1:]

if "--network-items" in arguments and "--insert" in arguments:
    URLIndex = arguments.index("--url") + 1
    URL = arguments[URLIndex]

    pathIndex = arguments.index("--path") + 1
    path = arguments[pathIndex]

    timeIndex = arguments.index("--time") + 1
    t_o = arguments[timeIndex]

    insert_network_items(URL, path, t_o)

elif "--network-items" in arguments and "--find" in arguments:
    results = find_network_items()
    print(results)
    
    



