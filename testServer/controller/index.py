import json
import sys
from controller import questionServe
if(sys.argv.__contains__("ip")):
  ip = input("Enter IP: ")
  port = input("Enter Port: ")
  questionServe.IP = ip
  questionServe.PORT = port
  questionServe.server_address = (ip, int(port))

import controller.auth


public_directory = ""
views_directory = ""

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

def getPQuestionPage(obj):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', "text/html")
    obj.end_headers()
    f = open(views_directory + "/pQuestionPage.html", 'rb')
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
          rev = questionServe.getQuestion(qnum["qnum"])
          jsonrev = json.loads(rev)
          jsonrev["tries"] = qnum["tries"]
          jsonrev["ans"] = qnum["ans"]
          jsonrev["correct"] = qnum["correct"]
          jsonrev["totalQ"] = controller.auth.CHOOSE_Q
          rev = json.dumps(jsonrev)
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
          if(qnum["correct"] or qnum["tries"] >=3):
              jsonrev = {}
              jsonrev["tries"] = qnum["tries"]
              jsonrev["ans"] = qnum["ans"]
              jsonrev["correct"] = qnum["correct"]
              rev = json.dumps(jsonrev)
          else:
            rev = questionServe.markQuestion(qnum["qnum"], post_data["option"])
            jsonrev = json.loads(rev)
            if(jsonrev["value"] == "T"):
                jsonrev["correct"] = qnum["correct"] = True
            qnum["tries"] += 1
            
            qnum["ans"] = post_data["option"]
            jsonrev["tries"] = qnum["tries"]
            jsonrev["ans"] = qnum["ans"]
            rev = json.dumps(jsonrev)
           
    except:
      rev = '{"question": null}'
    obj.wfile.write(bytes(rev, 'utf-8'))

def moveQuestion(obj, USER_MAP):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', "application/json")
    obj.end_headers()
    rev = '{"status": "fail"}'
    try:
      content_length = int(obj.headers['Content-Length']) # <--- Gets the size of data
      post_data = json.loads(obj.rfile.read(content_length).decode()) # <--- Gets the data

      ##Authorization
      if(controller.auth.verify(USER_MAP, post_data["uname"], post_data["token"])):
          numQuestions = controller.auth.CHOOSE_Q
          rev = "{}"
          jsonrev = json.loads(rev)
          if(post_data["val"] >= 0):
            USER_MAP[post_data["uname"]].currentQuestion = (USER_MAP[post_data["uname"]].currentQuestion+post_data["val"])%numQuestions
            jsonrev["status"] = "success"
          elif(post_data["val"] < 0):
            USER_MAP[post_data["uname"]].currentQuestion = (numQuestions +USER_MAP[post_data["uname"]].currentQuestion+ (post_data["val"]%numQuestions))%numQuestions
            jsonrev["status"] = "success"
          else:
              jsonrev["status"] = "fail"
          rev = json.dumps(jsonrev)
    except:
      rev = '{"status": "fail"}'

    obj.wfile.write(bytes(rev, 'utf-8'))

#################################################################################################################################################################
def getPQuestionData(obj, USER_MAP):
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
          i = USER_MAP[post_data["uname"]].currentPQuestion
          qnum = USER_MAP[post_data["uname"]].pQuestionSet[i]
          rev = questionServe.getPQuestion(qnum["qnum"])
          jsonrev = json.loads(rev)
          jsonrev["tries"] = qnum["tries"]
          jsonrev["ans"] = qnum["ans"]
          jsonrev["correct"] = qnum["correct"]
          jsonrev["totalQ"] = controller.auth.CHOOSE_Q
          rev = json.dumps(jsonrev)
    except:
      rev = '{"question": null}'
    obj.wfile.write(bytes(rev, 'utf-8'))

def markPQuestion(obj, USER_MAP):
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

          i = USER_MAP[post_data["uname"]].currentPQuestion
          qnum = USER_MAP[post_data["uname"]].pQuestionSet[i]
          if(qnum["correct"] or qnum["tries"] >=3):
              jsonrev = {}
              jsonrev["tries"] = qnum["tries"]
              jsonrev["ans"] = qnum["ans"]
              jsonrev["correct"] = qnum["correct"]
              rev = json.dumps(jsonrev)
          else:
            rev = questionServe.markPQuestion(qnum["qnum"], post_data["code"])
            jsonrev = json.loads(rev)
            if(jsonrev["value"] == "T"):
                jsonrev["correct"] = qnum["correct"] = True
            qnum["tries"] += 1
            qnum["ans"] = post_data["code"]
            jsonrev["tries"] = qnum["tries"]
            jsonrev["ans"] = qnum["ans"]
            rev = json.dumps(jsonrev)
           
    except:
      rev = '{"question": null}'
    obj.wfile.write(bytes(rev, 'utf-8'))

def movePQuestion(obj, USER_MAP):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', "application/json")
    obj.end_headers()
    rev = '{"status": "fail"}'
    try:
      content_length = int(obj.headers['Content-Length']) # <--- Gets the size of data
      post_data = json.loads(obj.rfile.read(content_length).decode()) # <--- Gets the data

      ##Authorization
      if(controller.auth.verify(USER_MAP, post_data["uname"], post_data["token"])):
          numQuestions = controller.auth.CHOOSE_P_Q
          rev = "{}"
          jsonrev = json.loads(rev)
          if(post_data["val"] >= 0):
            USER_MAP[post_data["uname"]].currentPQuestion = (USER_MAP[post_data["uname"]].currentPQuestion+post_data["val"])%numQuestions
            jsonrev["status"] = "success"
          elif(post_data["val"] < 0):
            USER_MAP[post_data["uname"]].currentPQuestion = (numQuestions +USER_MAP[post_data["uname"]].currentPQuestion+ (post_data["val"]%numQuestions))%numQuestions
            jsonrev["status"] = "success"
          else:
              jsonrev["status"] = "fail"
          rev = json.dumps(jsonrev)
    except:
      rev = '{"status": "fail"}'

    obj.wfile.write(bytes(rev, 'utf-8'))

    ##########################################################################################################################################

def getResults(obj, USER_MAP):
    obj.send_response(200)
    # Send headers
    obj.send_header('Content-type', "application/json")
    obj.end_headers()
    rev = '{"status": "fail"}'
    try:
      content_length = int(obj.headers['Content-Length']) # <--- Gets the size of data
      post_data = json.loads(obj.rfile.read(content_length).decode()) # <--- Gets the data

      ##Authorization
      if(controller.auth.verify(USER_MAP, post_data["uname"], post_data["token"])):
          questions = USER_MAP[post_data["uname"]].questionSet
          pquestions = USER_MAP[post_data["uname"]].pQuestionSet
          rev = "{}"
          jsonrev = json.loads(rev)
          done = []
          marks = []
          for q in questions:
              if(q["tries"] < 3 and not q["correct"]):
                  done.append(False)
              else:
                  done.append(True)
              if(q["correct"]):
                  marks.append(4-q["tries"])
              else:
                  marks.append(0)

          jsonrev["done"] = done
          jsonrev["marks"] = marks
          jsonrev["total"] = sum(marks)
          jsonrev["out"] = 3*controller.auth.CHOOSE_Q



          pdone = []
          pmarks = []
          for q in pquestions:
              if(q["tries"] < 3 and not q["correct"]):
                  pdone.append(False)
              else:
                  pdone.append(True)
              if(q["correct"]):
                  pmarks.append(4-q["tries"])
              else:
                  pmarks.append(0)

          jsonrev["pdone"] = pdone
          jsonrev["pmarks"] = pmarks
          jsonrev["ptotal"] = sum(pmarks)
          jsonrev["pout"] = 3*controller.auth.CHOOSE_P_Q


          rev = json.dumps(jsonrev)
    except:
      rev = '{"status": "fail"}'

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

