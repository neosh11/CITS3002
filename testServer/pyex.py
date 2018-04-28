#!/usr/bin/env python
 
import string,cgi,time
from os import curdir
from http.server import BaseHTTPRequestHandler, HTTPServer
from socketserver import ThreadingMixIn
 

# Multitreaded server declaration
class ThreadingHTTPServer(ThreadingMixIn, HTTPServer):
  pass


# HTTPRequestHandler class
class testHTTPServer_RequestHandler(BaseHTTPRequestHandler):
 
  # GET
  def do_GET(self):
        # Send response status code
        self.send_response(200)
 
        # Send headers
        self.send_header('Content-type', "text/html")
        self.end_headers()

        # Controllers
        print(self.path)
        if(self.path == "/home"):
          f = open(curdir + "/home.html", 'rb')
          self.wfile.write(f.read())
          f.close()
        elif(self.path == "/login"):
          f = open(curdir + "/login.html", 'rb')
          self.wfile.write(f.read())
          f.close()
        elif(self.path == "/haha"):
          self.wfile.write(bytes("HAHA YOU SUCK :)", "utf-8"))
        else:
          self.wfile.write(bytes("HAHA YOU SUCK :)", "utf-8"))
        return

def run():
  print('starting server...')
  # Server settings
  server_address = ('127.0.0.1', 80)
  httpd = ThreadingHTTPServer(server_address, testHTTPServer_RequestHandler)
  print('running server...')
  httpd.serve_forever()
 
 
run()