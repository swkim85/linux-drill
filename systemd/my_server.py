
from http.server import BaseHTTPRequestHandler, HTTPServer
import sys

hostName = "localhost"
default_serverPort = 8080


class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        self.wfile.write(bytes("Hello\n", "utf-8"))

if __name__ == "__main__":
  port = default_serverPort

  if len(sys.argv) > 1:
    port = int(sys.argv[1])
    if (port <= 1024): port = default_serverPort
    print("port : ", port)

  webServer = HTTPServer((hostName, port), MyServer)
  print("Server started http://%s:%s" % (hostName, port))

  try:
    webServer.serve_forever()
  except KeyboardInterrupt:
    pass

  webServer.server_close()


