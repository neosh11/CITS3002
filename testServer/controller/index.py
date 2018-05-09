import json
import controller.auth
import controller.questionServe

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

def getQuestionPage(obj):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', "text/html")
    obj.end_headers()
    f = open(views_directory + "/questionPage.html", 'rb')
    obj.wfile.write(f.read())
    f.close()


# POST STUFF
def postLogin(obj, USER_MAP):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', "application/json")
    obj.end_headers()
    rev =""
    try:
      content_length = int(obj.headers['Content-Length']) # <--- Gets the size of data
      post_data = json.loads(obj.rfile.read(content_length).decode()) # <--- Gets the data 
      print(post_data)
      rev = controller.auth.login(USER_MAP,post_data["uname"], post_data["password"])
    except:
      rev = None
    m = {'val': rev}
    n = json.dumps(m)
    obj.wfile.write(bytes(n, 'utf-8'))

def setQuestion(obj, USER_MAP):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', "application/json")
    obj.end_headers()
    rev = False
    try:
      content_length = int(obj.headers['Content-Length']) # <--- Gets the size of data
      post_data = json.loads(obj.rfile.read(content_length).decode()) # <--- Gets the data

      ##Authorization
      print(controller.auth.verify(USER_MAP, post_data["uname"], post_data["token"]))
      if(controller.auth.verify(USER_MAP, post_data["uname"], post_data["token"])):
          USER_MAP[post_data["uname"]].setCurrentQuestion(post_data["question"])
          rev = True
    except:
      rev = False
    m = {'status': rev}
    n = json.dumps(m)
    obj.wfile.write(bytes(n, 'utf-8'))

def getQuestionData(obj, USER_MAP):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', "application/json")
    obj.end_headers()
    rev = '{"question": null}'
    try:
      content_length = int(obj.headers['Content-Length']) # <--- Gets the size of data
      post_data = json.loads(obj.rfile.read(content_length).decode()) # <--- Gets the data

      ##Authorization
      if(controller.auth.verify(USER_MAP, post_data["uname"], post_data["token"])):
          i = USER_MAP[post_data["uname"]].currentQuestion
          qnum = USER_MAP[post_data["uname"]].questionSet[i]
          rev = controller.questionServe.getQuestion(qnum["qnum"])
    except:
      rev = '{"question": null}'
    obj.wfile.write(bytes(rev, 'utf-8'))

def markQuestion(obj, USER_MAP):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', "application/json")
    obj.end_headers()
    rev = '{"question": null}'
    try:
      content_length = int(obj.headers['Content-Length']) # <--- Gets the size of data
      post_data = json.loads(obj.rfile.read(content_length).decode()) # <--- Gets the data

      ##Authorization
      if(controller.auth.verify(USER_MAP, post_data["uname"], post_data["token"])):
          i = USER_MAP[post_data["uname"]].currentQuestion
          qnum = USER_MAP[post_data["uname"]].questionSet[i]
          rev = controller.questionServe.markQuestion(qnum["qnum"], post_data["option"])
    except:
      rev = '{"question": null}'
    obj.wfile.write(bytes(rev, 'utf-8'))

# Error handling
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

