from mitmproxy import http

def response(flow: http.HTTPFlow, number = []):
    if not len(number):
        number = 0
    
    number += 1

    with open(f"{number[0]}.html", "wb") as f:
        f.write(flow.response.raw_content)