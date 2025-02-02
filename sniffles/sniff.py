import os, time, subprocess
from mitmproxy import http

t_o = time.perf_counter()
def response(flow: http.HTTPFlow):
    
    if flow.response and flow.response.headers:

        if "text/html" in flow.response.headers.get("content-type", ""):
            html = flow.response.content if flow.response.content else None
            url = flow.request.pretty_url if flow.request.pretty_url else None

            if html and b"<!DOCTYPE html>" in html: #some text/html files aren't actual html files :(, TRICKERY!
                #at this point, both URL & html are valid
                print(f"URL: {url}")

                max_filename_length = 90 - len("/tmp/loggedFiles/") - len(".html")

                # Replace characters in the URL to make it filesystem-friendly
                safe_url = url.replace('/', '_').replace(':', '_').replace('?', '_').replace('&', '_')

                # Truncate the safe_url if it exceeds the maximum filename length
                if len(safe_url) > max_filename_length:
                    safe_url = safe_url[:max_filename_length]

                # Construct the full file path
                file_path = os.path.join("/tmp/loggedFiles", f"{safe_url}.html")

                while os.path.isfile(file_path):
                    file_path = file_path + "1"
                
                with open(file_path, "w") as f:
                    f.write(html.decode("utf-8"))
                    # Construct the command and arguments as a list

                    command = [
                        "/home/mjt58/project/SynapSense/venv/bin/python3",
                        "../database.py",
                        "--network-items",
                        "--insert",
                        "--url", safe_url,
                        "--path", file_path,
                        "--time", str(t_o)
                    ]

                    try:
                        # Run the subprocess and capture stdout and stderr
                        result = subprocess.run(
                            command,
                            capture_output=True,  # Captures both stdout and stderr
                            text=True,            # Returns output as strings instead of bytes
                            check=True            # Raises CalledProcessError for non-zero exit codes
                        )
                        # Print the standard output
                        if result.stdout:
                            print("Subprocess output:", result.stdout)
                    
                    except subprocess.CalledProcessError as e:
                        # Print the standard error
                        if e.stderr:
                            print("Subprocess error:", e.stderr)
                        else:
                            print("Subprocess failed with return code:", e.returncode)
