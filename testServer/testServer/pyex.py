#!/usr/bin/env python
 
import string,cgi,time
from os import curdir
from pathlib import Path
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

        # If public what it look like
        x = curdir+"/public"+self.path

        # Controllers
        if(self.path == "/home"):
          # Send headers
          self.send_header('Content-type', "text/html")
          self.end_headers()
          f = open(curdir + "/home.html", 'rb')
          self.wfile.write(f.read())
          f.close()
        elif(self.path == "/login"):
          # Send headers
          self.send_header('Content-type', "text/html")
          self.end_headers()
          f = open(curdir + "/login.html", 'rb')
          self.wfile.write(f.read())
          f.close()
        elif(self.path == "/haha"):
          # Send headers
          self.send_header('Content-type', "text/html")
          self.end_headers()
          self.wfile.write(bytes("HAHA YOU SUCK :)", "utf-8"))

        elif(Path(x).is_file() ):
          #Check public folder for resources.
          self.send_header('Content-type', "text/html")
          self.end_headers()
          self.wfile.write(bytes("IN PUBLIC FOLDER MANN", "utf-8"))
        else:
          self.send_header('Content-type', "text/html")
          self.end_headers()
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