#!/usr/bin/env python
 

import controller.index
import controller.api

import string,cgi,time
from os import curdir, path
from pathlib import Path
from http.server import BaseHTTPRequestHandler, HTTPServer
from socketserver import ThreadingMixIn
import ssl
 
CURRENT_DIR = path.dirname(__file__)
controller.index.public_directory = CURRENT_DIR+ "/public"
controller.index.views_directory = CURRENT_DIR+ "/views"
PORT = 3000

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
      controller.index.getPublic(self, path_fix)
    else:
      controller.index.getError(self)
    
    print(Path(self.path).parts)
    return

def run():
  print('Trying to start server')
  # Server settings
  server_address = ('0.0.0.0', PORT)
  httpd = ThreadingHTTPServer(server_address, testHTTPServer_RequestHandler)
  #Add SSL, remove the line below if you have not created a certificate. This makes the connection secure using python's inbuilt openSSL wrappers
  #TODO known issue is that the certificate is not verified by 3rd party, `so shows Your connection is not private`
  httpd.socket = ssl.wrap_socket (httpd.socket, certfile= CURRENT_DIR+'/certificates/certificate.pem', server_side=True, keyfile=CURRENT_DIR+"/certificates/key.pem")
  print('Serving on port %d...'%(PORT))
  httpd.serve_forever()
 
 
run()