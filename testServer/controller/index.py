public_directory=""
views_directory=""

def getHome(obj):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', "text/html")
    obj.end_headers()
    f = open(views_directory + "/home.html", 'rb')
    obj.wfile.write(f.read())
    f.close()

def getLogin(obj):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', "text/html")
    obj.end_headers()
    f = open(views_directory + "/login.html", 'rb')
    obj.wfile.write(f.read())
    f.close()
    
def getPublic(obj):
    obj.send_response(200)
    #Check public folder for resources.
    obj.send_header('Content-type', "text/html")
    obj.end_headers()
    obj.wfile.write(bytes("IN PUBLIC FOLDER MANN", "utf-8"))

def getError(obj):
    obj.send_response(404)
    obj.send_header('Content-type', "text/html")
    obj.end_headers()
    obj.wfile.write(bytes("Not found :(", "utf-8"))