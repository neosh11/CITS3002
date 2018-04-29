#!/usr/bin/env python
 
import string,cgi,time
import controller.index
import controller.api

from os import curdir, path
from pathlib import Path
from http.server import BaseHTTPRequestHandler, HTTPServer
from socketserver import ThreadingMixIn
 
CURRENT_DIR = path.dirname(__file__)
controller.index.public_directory = CURRENT_DIR+ "/public"
controller.index.views_directory = CURRENT_DIR+ "/views"

# Multithreaded server declaration
class ThreadingHTTPServer(ThreadingMixIn, HTTPServer):
  pass

# HTTPRequestHandler class
class testHTTPServer_RequestHandler(BaseHTTPRequestHandler):
 
  # GET
  def do_GET(self):
    # If public what it looks like
    x = controller.index.public_directory+self.path        

    # Security for the filesystem
    # If x is not contained within public folder
    # Get absolute path of x and compare to absolute path of public directory

    # Controllers
    path_fix = str(Path(self.path))
    if( path_fix =="/api/test"):
      controller.api.test(self)

    #Routing when not in ^^
    elif(self.path == "/"):
      controller.index.getHome(self)
    elif(self.path == "/login"):
      controller.index.getLogin(self)
    elif(Path(x).is_file()):
      controller.index.getPublic(self)
    else:
      controller.index.getError(self)
    
    print(Path(self.path).parts)
    return

def run():
  print('starting server...')
  # Server settings
  server_address = ('0.0.0.0', 3000)
  httpd = ThreadingHTTPServer(server_address, testHTTPServer_RequestHandler)
  print('running server...')
  httpd.serve_forever()
 
 
run()