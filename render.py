from mitmproxy import http

def response(flow: http.HTTPFlow):
    if "text/html" in flow.response.headers.get("content-type", ""):
        with open("page.html", "wb") as f:
            f.write(flow.response.content)
