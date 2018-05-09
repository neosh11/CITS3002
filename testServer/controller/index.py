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

def getDash(obj):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', "text/html")
    obj.end_headers()
    f = open(views_directory + "/dash.html", 'rb')
    obj.wfile.write(f.read())
    f.close()
    
def getPublic(obj, path):
    obj.send_response(200)
    #Check public folder for resources.
    obj.send_header('Content-type', headerType(path))
    obj.end_headers()
    f = open(public_directory + "/" + path, 'rb')
    obj.wfile.write(f.read())
    f.close()

def getError(obj):
    obj.send_response(404)
    obj.send_header('Content-type', "application/json")
    obj.end_headers()
    obj.wfile.write(bytes("{'message': 'not Found'}", 'utf-8'))

# Returns MIME type (Multipurpose Internet Mail Extensions) of a file 
def headerType(str):
    if(str.endswith('.html')):
        return "text/html"
    elif(str.endswith('.ico')):
        return "application/ico"
    elif(str.endswith('.js')):
        return "application/javascript"
