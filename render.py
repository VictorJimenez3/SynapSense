from mitmproxy import http

def response(flow: http.HTTPFlow):
    
    if flow.response and flow.response.headers:
        
        if "text/html" in flow.response.headers.get("content-type", ""):
            html = flow.response.content if flow.response.content else None
            url = flow.request.pretty_url if flow.request.pretty_url else None
            print(f"I AM AN HTML FILE")

            if html and b"<!DOCTYPE html>" in html: #some text/html files aren't actual html files :(, TRICKERY!
                print(f"good html content {type(html)} {len(html)}, URL: {url}")
                with open("website.html", "w") as f:
                    f.write("HELLOOOO")
                    file.close()
        elif flow.request.pretty_url: 
            url = flow.request.pretty_url if flow.request.pretty_url else None
            if "/short" in url:
                print(f"{flow.response.headers.get('content-type', '')} on URL: {url}")
        else:
            try:
                print(f'{flow.response.headers.get("content-type", "")} on URL: {flow.request.url}')
            except:
                pass

