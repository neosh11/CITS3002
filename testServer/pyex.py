#!/usr/bin/env python
 
import string,cgi,time
from os import curdir, path
from pathlib import Path
from http.server import BaseHTTPRequestHandler, HTTPServer
from socketserver import ThreadingMixIn
 
CURRENT_DIR = path.dirname(__file__)

# Multithreaded server declaration
class ThreadingHTTPServer(ThreadingMixIn, HTTPServer):
  pass

# HTTPRequestHandler class
class testHTTPServer_RequestHandler(BaseHTTPRequestHandler):
 
  # GET
  def do_GET(self):

        # If public what it look like
        x = CURRENT_DIR+"/public"+self.path
        print(self.path)

        # Security for the filesystem
        # If x is not contained within public folder
        # Get absolute path of x and compare to absolute path of public directory

        # Controllers
        if(self.path == "/home"):
          self.send_response(200)
          # Send headers
          self.send_header('Content-type', "text/html")
          self.end_headers()
          f = open(CURRENT_DIR + "/home.html", 'rb')
          self.wfile.write(f.read())
          f.close()
        elif(self.path == "/login"):
          self.send_response(200)
          # Send headers
          self.send_header('Content-type', "text/html")
          self.end_headers()
          f = open(CURRENT_DIR + "/login.html", 'rb')
          self.wfile.write(f.read())
          
          f.close()
        elif(self.path == "/haha"):
          self.send_response(200)
          # Send headers
          self.send_header('Content-type', "text/html")
          self.end_headers()
          self.wfile.write(bytes("HAHA YOU SUCK :)", "utf-8"))

        elif(Path(x).is_file()):
          self.send_response(200)
          #Check public folder for resources.
          self.send_header('Content-type', "text/html")
          self.end_headers()
          self.wfile.write(bytes("IN PUBLIC FOLDER MANN", "utf-8"))
        else:
          self.send_response(404)
          self.send_header('Content-type', "text/html")
          self.end_headers()
          self.wfile.write(bytes("Not found :(", "utf-8"))

        return

def run():
  print('starting server...')
  # Server settings
  server_address = ('0.0.0.0', 3000)
  httpd = ThreadingHTTPServer(server_address, testHTTPServer_RequestHandler)
  print('running server...')
  httpd.serve_forever()
 
 
run()