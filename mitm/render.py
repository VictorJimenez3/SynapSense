import os
from mitmproxy import http

def response(flow: http.HTTPFlow):
    
    if flow.response and flow.response.headers:
        
        if "text/html" in flow.response.headers.get("content-type", ""):
            html = flow.response.content if flow.response.content else None
            url = flow.request.pretty_url if flow.request.pretty_url else None

            if html and b"<!DOCTYPE html>" in html: #some text/html files aren't actual html files :(, TRICKERY!
                #at this point, both URL & html are valid
                print(f"URL: {url}")
                file_path = os.path.join("/tmp/loggedFiles", f"{url.replace('/', '_').replace(':','_').replace('?','_').replace('&', '_')}.html")
                print("FILE_PATH: ", file_path)

                with open(file_path, "w") as f:
                    f.write(html.decode("utf-8"))
